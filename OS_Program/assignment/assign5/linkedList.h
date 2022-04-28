#ifndef linkedList_h
#define linkedList_h
#include <time.h>

typedef struct PCB_st
{
    int ProcId;
    int ProcPR;
    int numCPUBurst, numIOBurst;
    int *CPUBurst, *IOBurst;
    int cpuIndex, ioIndex;
    struct timespec ts_begin, ts_end;
    struct PCB *prev, *next;

    // fields for performance measures, this will use system time
    int waitingTime;
    int queueEnterClock;
    
} PCB_st;

PCB_st *newPCBnode(int pid, int pr, int numCPU, int numIO, int *CPU, int *IO);

#endif