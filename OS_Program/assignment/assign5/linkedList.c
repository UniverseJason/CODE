#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

PCB_st *head = NULL;
PCB_st *tail = NULL;

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
    PCB_st *pcb_list = (PCB_st*)malloc( sizeof(PCB_st) );
    if(!pcb_list)
    {
        fprintf(stderr,"ERROR: newPCBlist cannot allocate memory\n");
        return NULL;
    }

    head = NULL;
    tail = NULL;

    return pcb_list;
}

/*
    this function will add a new node to the double linked list
*/
void addNewPCB(PCB_st* pcb_list, PCB_st* node)
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
        node->prev = tail;
        tail = node;
    }
}

/*
    this function will free the double linked list
*/
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

/*
    this function will print all the node in the Linked list
*/
void printLL(PCB_st *list)
{
    PCB_st *temp = head;
    if(head == NULL)
    {
        fprintf(stderr, "WARNING: list is empty, can not print\n");
        return;
    }
    while (temp != NULL)
    {
        fprintf(stdout, "ID: %d PR: %d\n", temp->ProcId, temp->ProcPR);
        temp = temp->next;
    }
}