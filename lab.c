/**
 * Name: Jane Doe
 * Email: janedoe@u.boisestate.edu
 * Github Username: jdoe
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h> //add thread library



/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}


/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q =  r + (p - r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);  
		merge(A, p, q, r);
 
	}
}

/*
 * parallel_mergesort(int A[], int p, int r):
 *
 * description: create multiple thread to run several serial_mergesort method at the same time , depth will limit the number of thread
 */
void *parallel_mergesort(thread_data_input *data) 
{
 //extract information form data input
        int p =data->left;
        int r =data->right;
        int depth =data->depth ;
        int *A =data->array;

	if(depth==0){ //if we reach the depth limit we will sort using the regular serial_mergesort (no more thread are available)
    serial_mergesort(A,p,r);
	}
 else{
	 
		int q = r + (p - r) / 2;
		//create 2 thread to process right and left part of the array at the same time and decrease the depth by 1 level
		// Data for thread 1
       thread_data_input * data_1;
		//allocate memory
		data_1 = (thread_data_input*)malloc(sizeof(thread_data_input));
		//initialize data
        data_1->left = p;
        data_1->right = q;
        data_1->depth = depth-1;
        data_1->array = A;
        // Create thread 1 
        pthread_t thread1; 
        pthread_create(&thread1,NULL,parallel_mergesort,data_1);

	// Data for thread 2
       thread_data_input * data_2;
		//allocate memory
		data_2 = (thread_data_input*)malloc(sizeof(thread_data_input));
		//initialize data
        data_2->left = q+1;
        data_2->right = r;
        data_2->depth = depth-1;
        data_2->array = A;
        // Create thread 2
        pthread_t thread2;
        pthread_create(&thread2,NULL,parallel_mergesort,data_2);

		//wait for both thread to be done before merging
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
  
		//merge left and right
		merge(A,p,q,r);

	
	//free the memory use -> not point
	free(data_1);
	free(data_2);
 	}

}


/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// now only at most one list has unprocessed elements.

	if (i <= q) {
		// copy remaining elements from the first list
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	// copy merged output from array B back to array A
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}
