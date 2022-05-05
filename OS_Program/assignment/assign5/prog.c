#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

// for performance measures
double Total_Turnaround_Time = 0;
int num_of_process = 0;

double Total_Waiting_Time = 0;
int num_of_waiting_process = 0;

struct timespec thread_start, thread_end;
double Throughput = 0;

struct timespec cpu_start, cpu_end;
double utilization = 0;


sem_t sem_cpu;
sem_t sem_io;

sem_t ready_Q_semaphore;
sem_t io_Q_semaphore;

PCB_list* ready_Q;
PCB_list* io_Q;


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
    ready_Q = newPCBlist();
    io_Q = newPCBlist();
    if(!ready_Q || !io_Q)
    {
        fprintf(stderr,"ERROR: main cannot allocate memory\n");
        return -1;
    }

    algoFlag = FIFO;

    // create the semaphore
    sem_init(&sem_cpu, 0, 1);
    sem_init(&sem_io, 0, 1);

    sem_init(&ready_Q_semaphore, 0, 1);
    sem_init(&io_Q_semaphore, 0, 1);

    // create threads virable
    pthread_t file_read_thread;
    pthread_t cpu_sch_thread;
    pthread_t io_sys_thread;

    clock_gettime(CLOCK_MONOTONIC, &thread_start);

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

    clock_gettime(CLOCK_MONOTONIC, &thread_end);
    Throughput = (double)num_of_process / getElapsed(thread_start, thread_end);

    printf("MAIN: All threads have been joined\n");
    printf("CPU utilization: %.3lf %%\n", (utilization / getElapsed(thread_start, thread_end)) * 100);
    printf("Throughput: %.3lf processes/ms\n", Throughput);
    printf("Avg. Turnaround time: %.3lf ms\n", Total_Turnaround_Time / num_of_process);
    printf("Avg. Waiting time: %.3lf ms\n", Total_Waiting_Time / num_of_waiting_process);

    // free the list
    freeList(ready_Q);
    freeList(io_Q);

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
            num_of_process++;

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
            clock_gettime(CLOCK_MONOTONIC, &newNode->ts_begin);

            // produce: add data to the buffer (ready queue)
            sem_wait(&ready_Q_semaphore);
            clock_gettime(CLOCK_MONOTONIC, &newNode->time_enter_ready_Q);
            num_of_waiting_process++;
            appendList(ready_Q, newNode);
            sem_post(&ready_Q_semaphore);

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
        if(isEmpty(ready_Q) == 1 && isEmpty(io_Q) == 1 && io_busy == 0 && cpu_busy == 0 && file_read_done == 1) break;
        if(algoFlag == FIFO)
        {
            int res = sem_timedwait(&sem_cpu, &sleep_time);
            if(res == -1 && errno == ETIMEDOUT) continue;
            if(isEmpty(ready_Q) == 1) continue;
            
            // simulate the CPU burst
            cpu_busy = 1;
            clock_gettime(CLOCK_MONOTONIC, &cpu_start);

            // consume: remove data from the ready queue
            sem_wait(&ready_Q_semaphore);
            PCB_st *tempPCB = deleteList(ready_Q);
            clock_gettime(CLOCK_MONOTONIC, &tempPCB->time_leave_ready_Q);
            Total_Waiting_Time += getElapsed(tempPCB->time_enter_ready_Q, tempPCB->time_leave_ready_Q);
            sem_post(&ready_Q_semaphore);
            
            // usleep for PCB->CPUBurst[PCB->cpuindex] (ms)
            usleep(tempPCB->CPUBurst[tempPCB->cpuIndex] * 1000);
            tempPCB->cpuIndex++;

            if(tempPCB->cpuIndex >= tempPCB->numCPUBurst)
            {
                // calculate the time
                clock_gettime(CLOCK_MONOTONIC, &tempPCB->ts_end);
                Total_Turnaround_Time += getElapsed(tempPCB->ts_begin, tempPCB->ts_end);

                // free this PCB
                free(tempPCB->CPUBurst);
                free(tempPCB->IOBurst);
                free(tempPCB);
                
                cpu_busy = 0;
                clock_gettime(CLOCK_MONOTONIC, &cpu_end);
                utilization += getElapsed(cpu_start, cpu_end);
            }
            else
            {
                // produce: add data to the IO queue;
                sem_wait(&io_Q_semaphore);
                appendList(io_Q, tempPCB);
                sem_post(&io_Q_semaphore);

                cpu_busy = 0;
                clock_gettime(CLOCK_MONOTONIC, &cpu_end);
                utilization += getElapsed(cpu_start, cpu_end);
                
                sem_post(&sem_io);
            }
        }
    }

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
        if(isEmpty(ready_Q) == 1 && cpu_busy == 0 && isEmpty(io_Q) == 1 && file_read_done == 1) break;

        int res = sem_timedwait(&sem_io, &sleep_time);
        if(res == -1 && errno == ETIMEDOUT) continue;
        if(isEmpty(io_Q) == 1) continue;

        // simulate the IO burst
        io_busy = 1;

        // CONSUME: remove the node from the io queue
        sem_wait(&io_Q_semaphore);
        PCB_st *pcbNode = deleteList(io_Q);
        sem_post(&io_Q_semaphore);

        // IO sleep for the IO burst
        usleep(pcbNode->IOBurst[pcbNode->ioIndex] * 1000);
        pcbNode->ioIndex++;

        // PRODUCE: insert PCB back to the Ready_Q
        sem_wait(&ready_Q_semaphore);
        clock_gettime(CLOCK_MONOTONIC, &pcbNode->time_enter_ready_Q);
        appendList(ready_Q, pcbNode);
        sem_post(&ready_Q_semaphore);

        // finish the IO burst
        io_busy = 0;
        sem_post(&sem_cpu);
    }

    io_sys_done = 1;
    return NULL;
}