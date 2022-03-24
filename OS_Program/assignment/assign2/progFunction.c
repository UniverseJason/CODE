
#include <stdio.h>
#include <stdlib.h>
#include "progFunction.h"

int CPUreg[8] = {0};
int CLOCK = 0;
int Total_waiting_time = 0;
int Total_turnaround_time = 0;
int Total_job = 0;

PCB_st *head = NULL;
PCB_st *tail = NULL;

// this function will created a new node for the linked list
PCB_st *newPCB(int id, int pr, int burst)
{
    PCB_st *pcb = (PCB_st*)malloc( sizeof(PCB_st) );
    if(!pcb)
    {
        fprintf(stderr,"ERROR: newPCB cannot allocate memory\n");
        return NULL;
    }

    // save the given data into corresponding fields of PCB
    pcb->ProcId = id;
    pcb->ProcPR = pr;
    pcb->CPUburst = burst;

    // set procress ID
    int i;
    for(i=0; i<8; i++)
    {
        pcb->myReg[i] = id;
    }

    // other properties
    pcb->queueEnterClock = 0;
    pcb->waitingTime = 0;

    // new node point to the next
    pcb->next = NULL;

    return pcb;
}

PCB_st *newPCBCmp(int id, int pr, int burst, int enterClock, int waitT)
{
    PCB_st *pcb = (PCB_st*)malloc( sizeof(PCB_st) );
    if(!pcb)
    {
        fprintf(stderr,"ERROR: newPCB cannot allocate memory\n");
        return NULL;
    }

    // save the given data into corresponding fields of PCB
    pcb->ProcId = id;
    pcb->ProcPR = pr;
    pcb->CPUburst = burst;

    // set procress ID
    int i;
    for(i=0; i<8; i++)
    {
        pcb->myReg[i] = id;
    }

    // other properties
    pcb->queueEnterClock = enterClock;
    pcb->waitingTime = waitT;

    // new node point to the next
    pcb->next = NULL;

    return pcb;
}

// this function will created a new list
PCB_st *newList()
{
    PCB_st *PCBList = (PCB_st*)malloc( sizeof(PCB_st) );
    if(!PCBList)
    {
        fprintf(stderr,"ERROR: PCB Linked list cannot allocate memory\n");
        return NULL;
    }

    head = NULL;
    tail = NULL;

    return PCBList;
}

// this function will insert a node in to the linked list
void EnList(PCB_st *list, PCB_st *node)
{
    if(head == NULL)
    {
        head = node;
        tail = node;
    }
    else
    {
        while(tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = node;
    }
}

// this function will free the linked list
void freeList(PCB_st *list)
{
    PCB_st *temp;

    if(head == NULL)
    {
        return;
    }
    while(head != NULL)
    {
        temp = head;
        head = list->next;
        free(temp);
    }
    free(list);
}

// this function will print all the node in the Linked list
void printLL(PCB_st *list)
{
    PCB_st *temp = head;
    if(head == NULL)
    {
        fprintf(stderr, "WARNING: list is empty, can not print,\n");
        return;
    }
    while (temp != NULL)
    {
        fprintf(stdout, "ID: %d  PR: %d  Burst: %d\n", temp->ProcId, temp->ProcPR, temp->CPUburst);
        temp = temp->next;
    }
}

void contextSwitch(PCB_st *PCB)
{
    int i = 0;
    for(i=0; i<8; i++)
    {
        CPUreg[i] = PCB->myReg[i];
        CPUreg[i] += 1;
    }

    for(i=0; i<8; i++)
    {
        PCB->myReg[i] = CPUreg[i];
    }
}

void dataOPR(PCB_st *PCB)
{
    PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
    Total_waiting_time = Total_waiting_time + PCB->waitingTime ;
    CLOCK = CLOCK + PCB->CPUburst;
    Total_turnaround_time = Total_turnaround_time + CLOCK;
    Total_job = Total_job + 1;
}

void performance_metrics()
{
    double waitTime = (double)Total_waiting_time / (double)Total_job;
    double trTime = (double)Total_turnaround_time / (double)Total_job;
    double jobTime = (double)Total_job / (double)CLOCK;

    printf("Average Waiting time = %.2lf ms\t\t(%d/%d)\n", waitTime, Total_waiting_time, Total_job);
    printf("Average Turnaround time = %.2lf ms\t(%d/%d)\n", trTime, Total_turnaround_time, Total_job);
    printf("Throughput = %.2lf jobs per ms\t\t(%d/%d)\n\n", jobTime, Total_job, CLOCK);
}

// This function will Do the FIFO
void FIFO_Scheduling()
{
    PCB_st* temp;
    int bst = 0;

    if(head == NULL)
    {
        return;
    }

    while(head != NULL)
    {
        temp = head;
        head = head->next;
        bst += temp->CPUburst;
        printf("Process %d is completed at %d ms\n", temp->ProcId, bst);
        contextSwitch(temp);
        dataOPR(temp);
        free(temp);
    }

    if(head == NULL)
    {
        tail = NULL;
    }
    performance_metrics();
}

// this function will DO the SJF
PCB_st *Get_next_item_SJF_or_PR(char flag)
{
    int bst = 0;
    PCB_st *temp, *prev, *target, *prev_i;
    if(head == NULL)
    {
        return NULL;
    }

    temp = head;
    prev = temp;
    target = head;

    // go through the linked list
    while(temp != NULL)
    {
        if(flag == 's' && temp->CPUburst <= target->CPUburst) // founded a shorter Cpu time
        {
            target = temp;
            prev_i = prev;
        }
        else if(flag == 'p' && temp->ProcPR >= target->ProcPR) // founded a larger PR
        {
            target = temp;
            prev_i = prev;
        }
        // go to the next
        prev = temp;
        temp = temp->next;
    }

    // if the min is the head, remove the head and return the head
    if(target == head)
    {
        head = head->next;
        target->next = NULL;
        if(head == NULL) // reach the end of the list
        {
            tail = NULL;
        }
    }
    else // min is in the middle of the linked list
    {
        prev_i->next = target->next;
        if(head == NULL) // reach the end of the list
        {
            tail = NULL;
        }
    }

    // calculation
    bst += target->CPUburst;
    printf("Process %d is completed at %d ms\n", target->ProcId, bst);
    contextSwitch(target);
    dataOPR(target);
    return target;
}
// this will loop the Linked list
void SJF_Scheduling()
{
    char flag = 's';
    PCB_st *target = head;
    if(target == NULL)
    {
        return;
    }

    while(target != NULL)
    {
        target = Get_next_item_SJF_or_PR(flag);
        free(target);
    }

    if(head == NULL)
    {
        tail = NULL;
    }
    performance_metrics();
}

// this will also go over the linked list, but use PR
void PR_Scheduling()
{
    char flag = 'p';
    PCB_st *target = head;
    if(target == NULL)
    {
        return;
    }

    while(target != NULL)
    {
        target = Get_next_item_SJF_or_PR(flag);
        free(target);
    }

    if(head == NULL)
    {
        tail = NULL;
    }
    performance_metrics();
}

// this will use RR to choose the result
void RR_Scheduling(PCB_st *LinkedList, int quantum)
{
    PCB_st *PCB, *temp;
    int bst = 0;

    if(head == NULL)
    {
        return;
    }

    while(head != NULL)
    {
        PCB = head;
        head = head->next;
        contextSwitch(PCB);
        if(PCB->CPUburst <= quantum)
        {
            PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
            Total_waiting_time = Total_waiting_time + PCB->waitingTime ;
            CLOCK = CLOCK + PCB->CPUburst;
            Total_turnaround_time = Total_turnaround_time + CLOCK;
            Total_job = Total_job + 1;
            bst += PCB->CPUburst;
            printf("Process %d is completed at %d ms\n", PCB->ProcId, bst);
            free(PCB);
        }
        else
        {
            PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
            CLOCK = CLOCK + quantum;
            PCB->CPUburst = PCB->CPUburst - quantum;
            PCB->queueEnterClock = CLOCK;
            temp = newPCBCmp(PCB->ProcId, PCB->ProcPR, PCB->CPUburst, PCB->queueEnterClock, PCB->waitingTime);
            EnList(LinkedList, temp);
        }
    }

    if(head == NULL)
    {
        tail = NULL;
    }
    performance_metrics();
}