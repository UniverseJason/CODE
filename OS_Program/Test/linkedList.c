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
    and return the new head
*/
PCB_st *appendPCBlist(PCB_st *list, PCB_st *node)
{
    PCB_st *temp = list;

    // if the list is empty
    if(list->next == NULL)
    {
        list->next = node;
        node->prev = list;
        return list;
    }

    // find the last node
    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    // add the new node to the end of the list
    temp->next = node;
    node->prev = temp;

    return list;
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
    this function will free a node
*/
void freeNode(PCB_st *node)
{
    free(node->CPUBurst);
    free(node->IOBurst);
    free(node);
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
    printf("\n");
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
    this function save the first node of the double linked list
*/
PCB_st *getFirst(PCB_st *list)
{
    if(isEmpty(list)) return NULL;
    return list->next;
}


/* 
    this function will remove the head node of the double linked list
*/
void removeFirst(PCB_st **list)
{
    if(*list != NULL)
    {
        PCB_st *temp = *list;
        *list = (*list)->next;
        (*list)->prev = NULL;
        free(temp);
    }
    else
    {
        free(*list);
        *list = NULL;
    }
}

/*
    This function will remove a node from the double linked list
*/
void removeNode(PCB_st **list, PCB_st *node)
{
    // head node is null
    if(*list == NULL || node == NULL) return;

    // if node is head
    if(node == *list)
    {
        *list = (*list)->next;
    }

    if(node->next != NULL)
    {
        node->next->prev = node->prev;
    }

    if(node->prev != NULL)
    {
        node->prev->next = node->next;
    }

    free(node);
    return;
}