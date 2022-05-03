#include <stdio.h>
#include <stdlib.h>
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

typedef struct list
{
    PCB_st* ready_Q;
    PCB_st* io_Q;
} list;

list data;

int main(int argc, char* argv[])
{
    data.ready_Q = newPCBlist();

    // create a new node
    PCB_st *node1 = newPCBnode(1, 1, 2, 2, NULL, NULL);
    PCB_st *node2 = newPCBnode(2, 2, 2, 2, NULL, NULL);
    PCB_st *node3 = newPCBnode(3, 3, 2, 2, NULL, NULL);
    PCB_st *node4 = newPCBnode(4, 4, 2, 2, NULL, NULL);
    PCB_st *node5 = newPCBnode(5, 5, 2, 2, NULL, NULL);

    // add node to the list
    data.ready_Q = appendPCBlist(data.ready_Q, node1);
    data.ready_Q = appendPCBlist(data.ready_Q, node2);
    data.ready_Q = appendPCBlist(data.ready_Q, node3);
    
    // print the list
    printLL(data.ready_Q);

    PCB_st *TTnode = data.ready_Q->next->next;

    removeNode(&data.ready_Q, TTnode);

    printLL(data.ready_Q);

    // // print the list
    // printf("Ready Q: \n");
    // printLL(data.ready_Q);
    // printf("IO Q: \n");
    // printLL(data.io_Q);



    return EXIT_SUCCESS;
}