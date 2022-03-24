#ifndef _progFunction_h
#define _progFunction_h

typedef struct PCB_st 
{
    int ProcId; 
    int ProcPR; 
    int CPUburst; 
    int myReg[8]; 
    int queueEnterClock, waitingTime; 
    struct PCB_st *next;
} PCB_st;

// function for the Linked List
PCB_st *newPCB(int id, int pr, int burst);
PCB_st *newPCBCmp(int id, int pr, int burst, int enterClock, int waitT);
PCB_st *newList();
void EnList(PCB_st *list, PCB_st *node);
void freeList(PCB_st *list);
void printLL(PCB_st *list);

// function for the Algorithms
void contextSwitch(PCB_st *PCB);
void dataOPR(PCB_st *PCB);
void performance_metrics();
void FIFO_Scheduling();

// if char = 's', then use SJF, if char = 'p', then use PR
PCB_st *Get_next_item_SJF_or_PR(char flag);
void SJF_Scheduling();
void PR_Scheduling();
void RR_Scheduling(PCB_st *LinkedList, int quantum);

#endif