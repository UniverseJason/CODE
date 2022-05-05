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
    struct timespec time_enter_ready_Q, time_leave_ready_Q;
    double total_time_in_ready_Q;
    
} PCB_st;

typedef struct PCB_list
{
    PCB_st *head, *tail;
} PCB_list;

PCB_st *newPCBnode(int pid, int pr, int numCPU, int numIO, int *CPU, int *IO);
PCB_list *newPCBlist();
void freeList(PCB_list *list);

void appendList(PCB_list *list, PCB_st *node);
PCB_st *deleteList(PCB_list *list);

int isEmpty(PCB_list *list);

PCB_st *getPCBNode(PCB_list *list, int pid);
void printLL(PCB_list *list);

double getElapsed(struct timespec begin, struct timespec end);

#endif