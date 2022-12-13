#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// this struct will ues to pass array and n to thread
typedef struct oneData{
    double *arr;
    int n;
} oneData;

// this function will sort an double thread array by using selection sort
void *selectionSort(void *arg)
{
    oneData myData = *((oneData *)arg);
    oneData *data = (oneData *)arg;

    int i = 0, j = 0, min_idx = 0;
    double temp;
    double *arr = myData.arr;
    int n = myData.n;

    // sort the array by selection sort
    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }

    // return struct
    data = (oneData *)malloc(sizeof(oneData));
    if(data == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data = myData;
    pthread_exit((void *)data);
}

// this struct will be use to pass the two double array to the thread
typedef struct arrayData
{
    double *arr1;
    double *arr2;
    double *finalArr;
    int n;
} arrayData;

// function mergeThread will merge two sorted array, then return struct arrayData
void *mergeThread(void *arg)
{
    arrayData myData = *((arrayData *)arg);
    arrayData *data = (arrayData *)arg;

    double *arr1 = myData.arr1;
    double *arr2 = myData.arr2;
    double *finalArr = myData.finalArr;
    int n = myData.n;

    // merge two sorted array in n time
    int i = 0, j = 0, k = 0;
    while (i < n && j < n)
    {
        if (arr1[i] < arr2[j])
            finalArr[k++] = arr1[i++];
        else
            finalArr[k++] = arr2[j++];
    }

    // return struct
    data = (arrayData *)malloc(sizeof(arrayData));
    if(data == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data = myData;
    pthread_exit((void *)data);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "ERROR: not enough cmd\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int n = atoi(argv[1]);

    struct timespec ts_begin, ts_end;
    double elapsed;

    // initialize A with random numbers with the range [1.0, 1000.0]
    double* A = (double*)malloc(n * sizeof(double));
    if(A == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }
    srand(time(NULL));
    for(i = 0; i < n; i++)
    {
        A[i] = (double)rand() / RAND_MAX * 1000.0 + 1.0;
    }

    // arrays B with the same size of A
    double* B = (double*)malloc(n * sizeof(double));
    if(B == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    /* -------------------- ONE THREAD CASE -------------------- */
    // copy A into B
    for(i = 0; i < n; i++)
    {
        B[i] = A[i];
    }

    // start time
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);

    // created a thread thB to sort array B, then join
    pthread_t thB;
    oneData dataB = {B, n}, ret1;
    oneData *dataRet;
    pthread_create(&thB, NULL, selectionSort, (void *)&dataB);
    pthread_join(thB, (void **)&dataRet);
    ret1 = *dataRet;

    // end time
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = ts_end.tv_sec - ts_begin.tv_sec; 
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    free(dataRet);

    // print out the elapsed time
    printf("Sorting is done in %lf ms when one thread is used\n", (elapsed * 1000));


    // Afirsthalf and AsecondHalf with the half size of A, and the final array
    double* Afirsthalf = (double*)malloc((n / 2) * sizeof(double));
    double* AsecondHalf = (double*)malloc((n / 2) * sizeof(double));
    double* final = (double*)malloc(n * sizeof(double));
    if(Afirsthalf == NULL || AsecondHalf == NULL || final == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    /* -------------------- TWO THREAD CASE -------------------- */
    // copy A into Afirsthalf and AsecondHalf
    for(i = 0; i < n / 2; i++)
    {
        Afirsthalf[i] = A[i];
        AsecondHalf[i] = A[i + n/2];
    }
    
    // start time
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);

    // create thA1 sortThread to sort Afirsthalf
    pthread_t thA1;
    oneData dataA1 = {Afirsthalf, n / 2}, ret2;
    oneData *dataRet2;
    pthread_create(&thA1, NULL, selectionSort, (void *)&dataA1);
    
    // create thA2 sortThread to sort AsecondHalf
    pthread_t thA2;
    oneData dataA2 = {AsecondHalf, n / 2}, ret3;
    oneData *dataRet3;
    pthread_create(&thA2, NULL, selectionSort, (void *)&dataA2);
    
    // join thA1 and thA2 and get return value
    pthread_join(thA1, (void **)&dataRet2);
    pthread_join(thA2, (void **)&dataRet3);
    ret2 = *dataRet2;
    ret3 = *dataRet3;

    // create thM to merge two sorted array
    pthread_t thM;
    arrayData data = {ret2.arr, ret3.arr, final, n / 2}, retFinal;
    arrayData *pArrayData;
    pthread_create(&thM, NULL, mergeThread, (void **)&data);
    pthread_join(thM, (void **)&pArrayData);
    retFinal = *pArrayData;

    // end time
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = ts_end.tv_sec - ts_begin.tv_sec;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    // print out the elapsed time
    printf("Sorting is done in %lf ms when two threads are used\n", (elapsed * 1000));

    // free all
    free(pArrayData);
    free(Afirsthalf);
    free(AsecondHalf);
    free(final);
    free(A);
    free(B);

    return 0;
}