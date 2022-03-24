#include <stdio.h>
#include <stdlib.h>

struct list_item{
    struct Proc_Ctrl_Block *PCB;
    int cpu_burst;
    int io_burst;
    struct list_item *next;
};

struct list_item *head=NULL, *tail=NULL;

struct list_item *Get_next_item_FCFS(){
    struct list_item *tmp;
    if(head==NULL)
    {
        return NULL;
    }

    tmp = head;
    head = head->next;
    
    if (head == NULL)
    {
        tail = NULL;
    }
    return tmp;
}

struct list_item *Get_next_item_SJF(){
    struct list_item *tmp, *prev, *min_i, *prev_i;
    if(head == NULL)
    {
        return NULL;
    }

    // go through and find the shortest time
    tmp = head;
    min_i = head;
    while(tmp != NULL)
    {
        if(min_i == NULL || tmp->cpu_burst < min_i->cpu_burst)  // founded a shorter Cpu time
        {
            min_i = tmp;
            prev_i = prev;
        }
    }

    // go to the next
    prev = tmp;
    tmp = tmp->next;

    // if the min is the head, remove the head and return the head
    if(min_i == head)
    {
        head = head->next;
        min_i->next = NULL;
        return min_i;
    }
    else // min is in the middle of the linked list
    {
        prev_i->next = min_i->next;
        if(tmp->next == NULL) // if reach the end of the list
        {
            tail = tmp;
        }
    }
    return min_i;
}