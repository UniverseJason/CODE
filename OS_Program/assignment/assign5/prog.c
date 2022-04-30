#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include "linkedList.h"

int i = 0;

// this struct will store the arg data, this is critical section
typedef struct mainData
{
    int file_read_done;
    int cpu_sch_done;
    int io_sys_done;
    sem_t cpu_busy;
    sem_t io_busy;
    sem_t sem_cpu;
    sem_t sem_io;

    // double linked list query
    PCB_st* ready_queue;
    PCB_st* io_queue;

} mainData;

/*
This function will create a new node of the double linked list
@ param:
    arg: argument passed to the thread, this is the mainData struct
*/
void *fileRead(void *arg)
{
    // necessary file read variables
    int currPID = 0;
    char command[6];
    int operation_length;
    int currentCPU, currentIO;
    PCB_st tempPCB;

    // get the mainData struct
    mainData data = *((mainData*)arg);
    mainData *data_ptr = (mainData*)arg;

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
            sem_wait(&data.cpu_busy);
            addNewPCB(data.ready_queue, newNode);
            sem_post(&data.cpu_busy);

            // free the CPU and IO burst list
            free(tempPCB.CPUBurst);
            free(tempPCB.IOBurst);
        }
        // end of the proc command

        //sem_post(&data.sem_cpu);
    }

    data.file_read_done = 1;

    // return the mainData struct
    data_ptr = (mainData *)malloc(sizeof(mainData));
    if(data_ptr == NULL)
    {
        printf("ERROR: malloc failed\n");
        exit(1);
    }

    *data_ptr = data;
    pthread_exit((void *)data_ptr);
}


int main(int argc, char* argv[])
{
    // necessary variables initialization
    mainData data;
    data.ready_queue = newPCBlist();
    data.io_queue = newPCBlist();
    data.file_read_done = 0;
    data.cpu_sch_done = 0;
    data.io_sys_done = 0;

    // create the semaphore
    sem_init(&data.cpu_busy, 0, 1);
    sem_init(&data.io_busy, 0, 1);
    sem_init(&data.sem_cpu, 0, 0);
    sem_init(&data.sem_io, 0, 0);

    // create threads virable
    pthread_t file_read_thread;
    pthread_t cpu_sch_thread;
    pthread_t io_sys_thread;

    // create file read threads
    mainData *dataRet;
    pthread_create(&file_read_thread, NULL, fileRead, (void*)&data);

    // wait thread
    pthread_join(file_read_thread, (void**)&dataRet);

    printLL(dataRet->ready_queue);
    


    return EXIT_SUCCESS;
}