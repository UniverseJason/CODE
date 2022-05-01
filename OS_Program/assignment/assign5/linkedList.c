#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/*
    This function will create a new node of the double linked list
@ param:
    int id: the process ID
    int pr: the priority of the process
    int numCPU: the number of CPU burst
    int numIO: the number of IO burst
    int *CPU: the CPU burst list
    int *IO: the IO burst list
*/
PCB_st *newPCBnode(int pid, int pr, int numCPU, int numIO, int *CPU, int *IO)
{
    PCB_st *pcb = (PCB_st*)malloc(sizeof(PCB_st));
    if(!pcb)
    {
        fprintf(stderr,"ERROR: newPCB node cannot allocate memory\n");
        return NULL;
    }

    // save the given data into corresponding fields of PCB
    pcb->ProcId = pid;
    pcb->ProcPR = pr;
    pcb->numCPUBurst = numCPU;
    pcb->numIOBurst = numIO;
    pcb->CPUBurst = CPU;
    pcb->IOBurst = IO;
    pcb->cpuIndex = 0;
    pcb->ioIndex = 0;
    
    pcb->ts_begin.tv_sec = 0;
    pcb->ts_begin.tv_nsec = 0;
    
    pcb->ts_end.tv_sec = 0;
    pcb->ts_end.tv_nsec = 0;

    pcb->next = NULL;
    pcb->prev = NULL;

    return pcb;
}

/*
    this functrion will create a new(empty) double linked list
*/
PCB_st *newPCBlist()
{
    PCB_st *list = (PCB_st*)malloc(sizeof(PCB_st));
    if(!list)
    {
        fprintf(stderr,"ERROR: newPCBlist cannot allocate memory\n");
        return NULL;
    }

    list->next = NULL;
    list->prev = NULL;

    return list;
}

/*
    this function will add a new node to the end of the double linked list
*/
void addNewPCB(PCB_st* pcb_list, PCB_st* node)
{
    PCB_st *temp = pcb_list;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
    node->prev = temp;
}

/*
    this function will free the double linked list
*/
void freeList(PCB_st *list)
{
    if(isEmpty(list))
    {
        fprintf(stderr,"ERROR: freeList cannot free an empty list\n");
        return;
    }

    PCB_st *temp = list;
    while(temp->next != NULL)
    {
        temp = temp->next;
        free(temp->prev);
    }
    free(temp);
}

/*
    this function will print all the node in the Linked list
*/
void printLL(PCB_st *list)
{
    if(isEmpty(list))
    {
        fprintf(stderr,"ERROR: printLL cannot print empty list\n");
        return;
    }

    PCB_st *temp = list;
    while(temp->next != NULL)
    {
        temp = temp->next;
        printf("ID: %d, PR: %d, CPU: %d, IO: %d\n", temp->ProcId, temp->ProcPR, temp->numCPUBurst, temp->numIOBurst);
    }
}

/*
    this function will check if the list is empty
*/
int isEmpty(PCB_st *list)
{
    if(list->next == NULL)
        return 1;
    else
        return 0;
}

/* 
    this function will remove and free the head node of the double linked list
*/
PCB_st *removeHead(PCB_st *list)
{
    if(isEmpty(list))
    {
        return NULL;
    }
    
    PCB_st *newHead = list->next;
    newHead->prev = NULL;

    PCB_st *oldHead = list;
    free(oldHead);

    return newHead;
}