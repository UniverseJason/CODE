#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/times.h>
#include "linkedList.h"

#define FIFO 1
#define SJF 2
#define PR 3
#define RR 4

int i = 0;
int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;

sem_t sem_cpu;
sem_t sem_io;
pthread_mutex_t lock;

typedef struct list
{
    PCB_st* ready_Q;
    PCB_st* io_Q;
    int algoFlag;
} list;

/*
This function will create a new node of the double linked list
    @ param
    arg: the list structure
*/
void* fileRead(void *arg);

/*
This function will go through the CPU schedule algorithm for FIFO, SJF, RR, and PR
    @ param
    arg: the list structure
*/
void* cpuSchedule(void *arg);

/*
This function will simulate the IO system
    @ param
    arg: the list structure
*/
void* ioSystem(void *arg);


int main(int argc, char* argv[])
{
    // create these new list
    list *myList = (list *)malloc(sizeof(list));
    myList->ready_Q = newPCBlist();
    myList->io_Q = newPCBlist();
    if(!myList || !myList->ready_Q || !myList->io_Q)
    {
        fprintf(stderr,"ERROR: main cannot allocate memory\n");
        return -1;
    }

    myList->algoFlag = FIFO;

    // create the semaphore
    sem_init(&sem_cpu, 0, 0);
    sem_init(&sem_io, 0, 0);

    // create threads virable
    pthread_t file_read_thread;
    pthread_t cpu_sch_thread;
    pthread_t io_sys_thread;

    // create file read threads
    pthread_create(&file_read_thread, NULL, fileRead, (void*)&myList);

    // create cpu schedule threads
    pthread_create(&cpu_sch_thread, NULL, cpuSchedule, (void*)&myList);

    // create io system threads
    pthread_create(&io_sys_thread, NULL, ioSystem, (void*)&myList);

    // wait thread
    pthread_join(file_read_thread, (void**)&myList);
    pthread_join(cpu_sch_thread, (void**)&myList);
    pthread_join(io_sys_thread, (void**)&myList);

    printLL(myList->ready_Q);
    printLL(myList->io_Q);

    freeList(myList->ready_Q);
    freeList(myList->io_Q);
    free(myList);

    return EXIT_SUCCESS;
}




void *fileRead(void *arg)
{
    // necessary file read variables
    int currPID = 0;
    char command[6];
    int operation_length;
    int currentCPU, currentIO;
    PCB_st tempPCB;

    // get the list struct
    list data = *((list*)arg);
    list *data_ptr = (list*)arg;

    // open the file
    FILE *inFile = fopen("input.txt", "r");
    if(!inFile)
    {
        fprintf(stderr,"ERROR: fileRead cannot open file\n");
        return NULL;
    }

    // read the file line by line
    while(fscanf(inFile, "%s %d %d", command, &tempPCB.ProcPR, &operation_length) != EOF)
    {
        // if the command is proc
        if(strcmp(command, "proc") == 0)
        {
            // calculate the number of CPU and IO burst
            tempPCB.numCPUBurst = (operation_length / 2) + 1;
            tempPCB.numIOBurst = operation_length / 2;

            // allocate memory for CPU and IO burst list
            tempPCB.CPUBurst = (int*)malloc(tempPCB.numCPUBurst * sizeof(int));
            tempPCB.IOBurst = (int*)malloc(tempPCB.numIOBurst * sizeof(int));
            if(!tempPCB.CPUBurst || !tempPCB.IOBurst)
            {
                fprintf(stderr,"ERROR: fileRead cannot allocate memory\n");
                return NULL;
            }
            
            // read the CPU and IO burst list alternatively in one line
            tempPCB.cpuIndex = 0;
            tempPCB.ioIndex = 0;
            for(i = 0; i < operation_length; i++)
            {
                if(i % 2 == 0)
                {
                    fscanf(inFile, "%d", &currentCPU);
                    tempPCB.CPUBurst[tempPCB.cpuIndex] = currentCPU;
                    tempPCB.cpuIndex++;
                }
                else
                {
                    fscanf(inFile, "%d", &currentIO);
                    tempPCB.IOBurst[tempPCB.ioIndex] = currentIO;
                    tempPCB.ioIndex++;
                }
            }

            // create a new node for the double linked list
            PCB_st *newNode = newPCBnode(++currPID, tempPCB.ProcPR, tempPCB.numCPUBurst, tempPCB.numIOBurst, tempPCB.CPUBurst, tempPCB.IOBurst);

            // critical section: insert PCB into Ready_Q in the mainData struct
            pthread_mutex_lock(&lock);
            addNewPCB(data.ready_Q, newNode);
            pthread_mutex_unlock(&lock);

            // free the CPU and IO burst list
            free(tempPCB.CPUBurst);
            free(tempPCB.IOBurst);
        }
        // end of the proc command
    }

    file_read_done = 1;

    // return the list data struct
    data_ptr = (list*)malloc(sizeof(list));
    if(data_ptr == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data_ptr = data;
    pthread_exit((void *)data_ptr);
}



void *cpuSchedule(void *arg)
{
    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;

    // get the list struct
    list data = *((list*)arg);
    list *data_ptr = (list*)arg;

    // while will go through the CPU schedule algorithm
    while(true)
    {
        if(isEmpty(data.ready_Q) == 1 && isEmpty(data.io_Q) == 1 && io_busy == 0 && cpu_busy == 0 && file_read_done == 1) break;

        if(data.algoFlag == FIFO)
        {
            int res = sem_timedwait(&sem_cpu, &ts);
            if(res == -1 && errno == ETIMEDOUT) continue;
            
            // simulate the CPU burst
            cpu_busy = 1;
            PCB_st *pcbNode = removeHead(data.ready_Q);
            if(pcbNode == NULL) continue;
            usleep(pcbNode->CPUBurst[pcbNode->cpuIndex] * 1000);
            pcbNode->cpuIndex++;

            // if this idx is the last CPU burst
            if(pcbNode->cpuIndex >= pcbNode->numCPUBurst)
            {
                free(pcbNode->CPUBurst);
                free(pcbNode->IOBurst);
                free(pcbNode);
                cpu_busy = 0;
            }
            else
            {
                // critical section: insert PCB to the IO queue
                pthread_mutex_lock(&lock);
                addNewPCB(data.io_Q, pcbNode);
                pthread_mutex_unlock(&lock);

                cpu_busy = 0;
                sem_post(&sem_io);
            }
        }
    }

    cpu_sch_done = 1;

    // return the list data struct
    data_ptr = (list*)malloc(sizeof(list));
    if(data_ptr == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data_ptr = data;
    pthread_exit((void *)data_ptr);
}

void* ioSystem(void *arg)
{
    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;

    // get the list struct
    list data = *((list*)arg);
    list *data_ptr = (list*)arg;

    while (true)
    {
        if(isEmpty(data.ready_Q) == 1 && cpu_busy == 0 && isEmpty(data.io_Q) == 0 && file_read_done == 1) break;
        int res = sem_timedwait(&sem_cpu, &ts);
        if(res == -1 && errno == ETIMEDOUT) continue;

        io_busy = 1;
        PCB_st *pcbNode = removeHead(data.io_Q);
        if(pcbNode == NULL) continue;
        usleep(pcbNode->IOBurst[pcbNode->ioIndex] * 1000);
        pcbNode->ioIndex++;

        // insert PCB into Ready_Q
        pthread_mutex_lock(&lock);
        addNewPCB(data.ready_Q, pcbNode);
        pthread_mutex_unlock(&lock);

        io_busy = 0;
        sem_post(&sem_cpu);
    }

    io_sys_done = 1;
    
    // return the list data struct  
    data_ptr = (list*)malloc(sizeof(list));
    if(data_ptr == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data_ptr = data;
    pthread_exit((void *)data_ptr);
}