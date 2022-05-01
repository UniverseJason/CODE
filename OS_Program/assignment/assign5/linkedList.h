#ifndef linkedList_h
#define linkedList_h

typedef struct PCB_st
{
    int ProcId;
    int ProcPR;
    int numCPUBurst, numIOBurst;
    int *CPUBurst, *IOBurst;
    int cpuIndex, ioIndex;
    struct timespec ts_begin, ts_end;
    struct PCB_st *prev, *next;

    // fields for performance measures, this will use system time
    int waitingTime;
    int queueEnterClock;
    
} PCB_st;

PCB_st *newPCBnode(int pid, int pr, int numCPU, int numIO, int *CPU, int *IO);
PCB_st *newPCBlist();
void addNewPCB(PCB_st* pcb_list, PCB_st* node);
void freeList(PCB_st *list);
void printLL(PCB_st *list);
int isEmpty(PCB_st *list);
PCB_st *removeHead(PCB_st *list);

#endif