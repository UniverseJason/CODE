#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "linkedList.h"

int main(int argc, char* argv[])
{
    // this will read input file
    FILE *inFIle = fopen("input.txt", "r");
    if(!inFIle)
    {
        fprintf(stderr, "ERROR: cannot open input file.\n");
        exit(EXIT_FAILURE);
    }

    // malloc buffer
    char *buffer = (char*)malloc(128 * sizeof(char));
    if(!buffer)
    {
        fprintf(stderr, "ERROR: cannot allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // necessary file read variables
    char command[6];
    int priority, operation_length;
    int currentCPU, currentIO;
    int *CPU_burst_list, *IO_burst_list;
    
    // operation variables
    int file_read_done = 0;
    int cpu_sch_done   = 0;
    int io_sys_done    = 0;
    int cpu_busy       = 0;
    int io_busy        = 0;
    int sem_cpu        = 0;
    int sem_io         = 0;

    // query
    PCB_st* ready_queue;
    PCB_st* io_queue;
    
    // temp variables
    PCB_st *informationPCB;
    int i;



    return EXIT_SUCCESS;
}