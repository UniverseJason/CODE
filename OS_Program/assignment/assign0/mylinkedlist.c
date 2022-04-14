/*
 * File: mylinkedlist.c
 * YOUR NAME Jiaxing Li
 * YOU NEED TO IMPLEMENT THE FUNCTIONS  here
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include "mylinkedlist.h"

/*
 * Function: NewStudentCell
 * Usage: student_cell_T *element;
 *        element = NewStudentCell(int id, double gpa, char *name);
 * --------------------------
 * This function allocates space for a student cell and initialize its filedes
 */
student_cell_T *NewStudentCell(int id, double gpa, char *name)
{
  student_cell_T *element;
  
  element = (student_cell_T *) malloc( sizeof(student_cell_T) );
  if( !element){
      fprintf(stderr,"NewStudentCell cannot allocate memory\n");
      return NULL;
  }
  element->id = id;
  element->gpa = gpa;
  element->name = name;

  return element;
}


/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void)
{
  linked_list_T *list;
  
  list = (linked_list_T *) malloc( sizeof(linked_list_T) );
  if( !list){
    fprintf(stderr,"NewLinkedList cannot allocate memory\n");
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;

  return list;
}


/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list)
{
  student_cell_T *temp;
  
  while (list->head != NULL)
  {
    temp = list->head;
    list->head = list->head->next;
    free(temp);
  }
  free(list);
}

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, student_cell_T *element)
{
  if(list->head == NULL)
  {
    list->head = element;
    list->tail = element; 
  }
  else
  {
    while(list->tail->next != NULL)
    {
      list->tail = list->tail->next;
    }
    list->tail->next = element;
  }
}

/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
student_cell_T *Delist(linked_list_T *list)
{
  student_cell_T *temp;
  
  if(list->head == NULL)
  {
    fprintf(stderr, "ERROR: linked list is empty!");
    return NULL;
  }
  else
  {
    temp = list->head;
    list->head = list->head->next;
    free(temp);
  }

  return temp;
}

/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list)
{
  if(list->head == NULL) return 1;
  else return 0;
}

int LinkedListIsFull(linked_list_T *list)
{
  return 0; // because we have linked list
}

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T  *list)
{
  int counter = 0;
  student_cell_T *curr = list->head;
  while(curr != NULL)
  {
    counter++;
    curr = curr->next;
  }
  return counter;
}

/*
 * Function: GetLinkedListElement
 * Usage: element = GetLinkedListElement(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetLinkedListElement(list, 0) returns the initial
 * element from the list without removing it.  If the caller tries
 * to select an element that is out of range, GetLinkedListElement prints
 * Error and returns NULL.  Note: This function is not a fundamental list operation
 * and is instead provided mainly to facilitate debugging.
 */
student_cell_T *GetLinkedListElement(linked_list_T *list, int index)
{
  int max = LinkedListLength(list);
  int counter = 0;
  
  // check the index is in the range
  if(index < 0 || index > max)
  {
    fprintf(stderr, "ERROR: index out of bound or invalid.");
    return NULL;
  }

  // return head if index is 0
  if(index == 0)
  {
    return list->head;
  }
  
  // for longer linked list
  student_cell_T *curr = list->head;
  while(curr != NULL)
  {
    if(counter == index) // founded the target
    {
      return curr;
    }
    counter++;
    curr = curr->next;
  }

  // if have any error will print these
  fprintf(stderr, "ERROR: not founded");
  return NULL;
}



/* OTHER FUNCTIONS YOU WOULD NEED....
 * EXPORT THEM HERE, BUT IMPLMENT THEM in mylinkedlist.c 
 */

void printList(linked_list_T *list)
{
  student_cell_T *curr = list->head;
  while(curr != NULL)
  {
    printf("Student %s, ID %d, GPA %lf\n", curr->name, curr->id, curr->gpa);
    curr = curr->next;
  }
}

void calculator(linked_list_T *list)
{
  student_cell_T *curr = list->head;

  double maxGPA = curr->gpa;
  double minGPA = curr->gpa;
  
  double avgGPA = 0.0;
  double stuNum = 0;
  double totalGPA = 0.0;
  
  while(curr != NULL)
  {
    // find max
    if(curr->gpa > maxGPA)
    {
      maxGPA = curr->gpa;
    }

    // find min
    if(curr->gpa < minGPA)
    {
      minGPA = curr->gpa;
    }

    // for average
    stuNum++;
    totalGPA += curr->gpa;
    
    // go to the next
    curr = curr->next;
  }

  avgGPA = totalGPA / stuNum;

  printf("Min = %lf, Max = %lf, Avg = %lf\n", minGPA, maxGPA, avgGPA);
}

void removeLargest(linked_list_T *list)
{
  student_cell_T *curr = list->head;
  student_cell_T *prev = list->head;

  double maxGPA = curr->gpa;

  // find Max gpa
  while(curr != NULL)
  {
    if(curr->gpa > maxGPA)
    {
      maxGPA = curr->gpa;
    }

    curr = curr->next;
  }

  // remove this GPA
  // need to continue until meet the max gpa, move the previous and current pointer as well
  curr = list->head; // set current node back to the head
  while(curr != NULL && curr->gpa != maxGPA)
  {
    prev = curr;
    curr = curr->next;
  }

  // if can not found, should not reach
  if(curr == NULL)
  {
    printf("ERROR: cannnot found\n");
    return;
  }

  // link previous to the next of the current, then free current
  prev->next = curr->next;
  printf("Remove the max GPA = %lf, with name %s and ID %d\n", curr->gpa, curr->name, curr->id);
  free(curr);
}