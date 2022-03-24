/*
 * File: mylinkedlist.h

 * YOUR NAME Jiaxing Li
 * DO NOT CHANGE THE EXISTING STRUCT OR FUNCTION NAMES HERE
 *
 * WHEN ADDING NEW FUNCTIONS, e.g., listing all students, 
 *       finding min, max, avg gpa, and removing the student with highest gpa etc.
 * YOU CAN NAME THEM AS YOU WISH, BUT TRY TO USE THE SAME NAMING CONVENTION  
 *
 * THIS IS JUST AN INTERFACE, SO DO NOT WRITE ANY IMPLEMENTATION/CODE HERE
 * ALL IMPLEMENTATION/CODE FOR THE BELOW FUNCTIONS MUST BE in mylinkedlist.c 
 *  
 * -----------------------------------------------------
 * This is just an interface  for a basic library of linked list of students.
 * In future assignments, you will use linked list structure for other purposes 
 * and accordingly reuse/modify functions you develop here... 
 */

#ifndef _mylinkedlist_h
#define _mylinkedlist_h

/*
 * Type: student_cell_T
 * -----------
 * This type defines the cells used for the linked list that
 * stores the student information we use in assing0  
 * You can change this structure later on for other assignments!
 */
typedef struct student_cell_T {
    int     id;      
    double  gpa;       
    char   *name;  //  name is just a pointer here, you need to allocate space for name
    struct student_cell_T *next;
} student_cell_T;


/*
 * Type: linked_list_T
 * --------------
 * This type defines the concrete representation of a linked list of student cells.
 * The head field points to the first element in the linked list.
 * The tail field points to the last element in the linked list.
 * The empty linked list is indicated by a NULL head pointer.
 */
typedef struct linked_list_T {
    student_cell_T *head;
    student_cell_T *tail;
} linked_list_T;


/*
 * Function: NewStudentCell
 * Usage: student_cell_T *element;
 *        element = NewStudentCell(id, gpa, name);
 * --------------------------
 * This function allocates space for a student cell and intilize its fileds
 */
student_cell_T *NewStudentCell(int id, double gpa, char *name);


/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void);


/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list);

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, student_cell_T *element);

/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
student_cell_T *Delist(linked_list_T *list);

/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list);
int LinkedListIsFull(linked_list_T *list);

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T  *list);

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
student_cell_T *GetLinkedListElement(linked_list_T *list, int index);



/* OTHER FUNCTIONS YOU WOULD NEED....
 * EXPORT THEM HERE, BUT IMPLMENT THEM in mylinkedlist.c 
 */

void printList(linked_list_T *list);
void calculator(linked_list_T *list);
void removeLargest(linked_list_T *list);

#endif
