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
int algoFlag = FIFO;

sem_t sem_cpu;
sem_t sem_io;

pthread_mutex_t cpu;
pthread_mutex_t io;


typedef struct list
{
    PCB_st* ready_Q;
    PCB_st* io_Q;
} list;

list data;

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
    if(argc < 4)
    {
        fprintf(stderr, "ERROR: incorrect number of argument.\nUsage: /prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n\n");
        exit(1);
    }

    int i;
    int AlgoChecker = false;
    int flagChecker = false;
    int inputChecker = false;
    int quantumChecker = false;
    int flag = -1;
    char *inFileName;
    int quantum = -1;

    // check the argument
    for(i=0; i<argc; i++)
    {
        if (strcmp("-alg", argv[i]) == 0)
        {
            AlgoChecker = true;
        }

        if (strcmp("FIFO", argv[i]) == 0)
        {
            flag = FIFO;
            flagChecker = true;
        }

        if (strcmp("SJF", argv[i]) == 0)
        {
            flag = SJF;
            flagChecker = true;
        }

        if (strcmp("PR", argv[i]) == 0)
        {
            flag = PR;
            flagChecker = true;
        }

        if (strcmp("RR", argv[i]) == 0)
        {
            flag = RR;
            flagChecker = true;
        }

        if (strcmp("-quantum", argv[i]) == 0)
        {
            quantumChecker = true;
            quantum = atoi(argv[i+1]);
        }

        if (strcmp("-input", argv[i]) == 0)
        {
            inputChecker = true;
            inFileName = argv[i+1];
        }
    }

    if(flag == RR && quantumChecker == false )
    {
        fprintf(stderr, "Usage: [RR] [-quantum integer(ms)]\n\n");
        exit(0);
    }

    if(inputChecker == false || AlgoChecker == false || flagChecker == false)
    {
        fprintf(stderr, "Usage: /prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n\n");
        exit(0);
    }

    // create these new list
    data.ready_Q = newPCBlist();
    data.io_Q = newPCBlist();
    if(!data.ready_Q || !data.io_Q)
    {
        fprintf(stderr,"ERROR: main cannot allocate memory\n");
        return -1;
    }

    algoFlag = FIFO;

    // create the semaphore
    sem_init(&sem_cpu, 0, 1);
    sem_init(&sem_io, 0, 1);

    // create threads virable
    pthread_t file_read_thread;
    pthread_t cpu_sch_thread;
    pthread_t io_sys_thread;

    // create file read threads
    pthread_create(&file_read_thread, NULL, fileRead, NULL);

    // create cpu schedule threads
    pthread_create(&cpu_sch_thread, NULL, cpuSchedule, NULL);

    // create io system threads
    pthread_create(&io_sys_thread, NULL, ioSystem, NULL);

    // wait thread
    pthread_join(file_read_thread, NULL);
    pthread_join(cpu_sch_thread, NULL);
    pthread_join(io_sys_thread, NULL);

    printf("MAIN: All threads have been joined\n");
    printLL(data.ready_Q);
    printf("\n");
    printLL(data.io_Q);

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

            // start the clock
            clock_gettime(&newNode->ts_begin);

            // produce: add data to the buffer (ready queue)
            pthread_mutex_lock(&cpu);
            appendPCBlist(&data.ready_Q, newNode);
            pthread_mutex_unlock(&cpu);
            
            // free the CPU and IO burst list
            free(tempPCB.CPUBurst);
            free(tempPCB.IOBurst);

            sem_post(&sem_cpu);
        }
        // end of the proc command

        // if command is sleep
        else if(strcmp(command, "sleep") == 0)
        {
            usleep(tempPCB.ProcPR * 1000);
        }

        // if the command is stop
        else if(strcmp(command, "stop") == 0)
        {
            break;
        }

        // default 
        else break;
    }

    printf("read file done\n");
    file_read_done = 1;
    return NULL;
}




void *cpuSchedule(void *arg)
{
    // 1 sec timespec strcut
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 0;


    // while will go through the CPU schedule algorithm
    while(true)
    {
        //if(file_read_done == 1) break;
        if(isEmpty(data.ready_Q) == 1 && isEmpty(data.io_Q) == 1 && io_busy == 0 && cpu_busy == 0 && file_read_done == 1) break;
        if(algoFlag == FIFO)
        {
            int res = sem_timedwait(&sem_cpu, &sleep_time);
            if(res == -1 && errno == ETIMEDOUT) continue;
            else
            {
                // simulate the CPU burst
                cpu_busy = 1;

                // consume: remove data from the ready queue
                pthread_mutex_lock(&cpu);
                PCB_st *tempPCB = data.ready_Q->next;
                if(tempPCB == NULL) continue;
                removeFirst(&data.ready_Q);
                pthread_mutex_unlock(&cpu);
                

                // usleep for PCB->CPUBurst[PCB->cpuindex] (ms)
                usleep(tempPCB->CPUBurst[tempPCB->cpuIndex] * 1000);
                tempPCB->cpuIndex++;

                // this is the last CPU burst
                if(tempPCB->cpuIndex >= tempPCB->numCPUBurst)
                {
                    // calculate the time
                    clock_gettime(CLOCK_MONOTONIC, &tempPCB->ts_end);
                    int elapsed = tempPCB->ts_end.tv_sec - tempPCB->ts_begin.tv_sec;
                    elapsed += (tempPCB.ts_end.tv_nsec - tempPCB->ts_begin.tv_nsec) / 1000000000.0;
                    printf("turnaround time: %f ms\n", elapsed);

                    // free this PCB
                    pthread_mutex_lock(&cpu);
                    removeNode(&data.ready_Q, tempPCB);
                    pthread_mutex_unlock(&cpu);
                    cpu_busy = 0;
                }
                else
                {
                    // produce: add data to the IO queue;
                    pthread_mutex_lock(&io);
                    appendPCBlist(&data.io_Q, tempPCB);
                    pthread_mutex_unlock(&io);

                    cpu_busy = 0;
                    sem_post(&sem_io);
                }
            }
        }
    }

    printf("CPU done\n");
    cpu_busy = 0;
    cpu_sch_done = 1;
    return NULL;
}




void* ioSystem(void *arg)
{
    // 1 sec timespec strcut
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 0;

    while (true)
    {
        //if(cpu_sch_done == 1) break;
        if(isEmpty(data.ready_Q) == 1 && cpu_busy == 0 && isEmpty(data.io_Q) == 1 && file_read_done == 1) break;

        int res = sem_timedwait(&sem_io, &sleep_time);
        if(res == -1 && errno == ETIMEDOUT) continue;

        // simulate the IO burst
        io_busy = 1;

        // CONSUME: remove the node from the io queue
        pthread_mutex_lock(&io);
        PCB_st *pcbNode = data.io_Q->next;
        if(pcbNode == NULL) continue;
        removeFirst(&data.io_Q);
        pthread_mutex_unlock(&io);

        // IO sleep for the IO burst
        usleep(pcbNode->IOBurst[pcbNode->ioIndex] * 1000);
        pcbNode->ioIndex++;

        // PRODUCE: insert PCB back to the Ready_Q
        pthread_mutex_lock(&cpu);
        appendPCBlist(&data.ready_Q, pcbNode);
        pthread_mutex_unlock(&cpu);

        // finish the IO burst
        io_busy = 0;
        sem_post(&sem_cpu);
    }

    io_busy = 0;
    io_sys_done = 1;
    return NULL;
}