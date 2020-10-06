#ifndef __LAB_H
#define __LAB_H

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0
typedef struct {
   
    int left;
    int right;
    int depth;
	 int *array;
    } thread_data_input;

// function prototypes
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
void *parallel_mergesort(thread_data_input *data);

const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error

#ifdef __cplusplus
}
#endif
#endif //__LAB_H
