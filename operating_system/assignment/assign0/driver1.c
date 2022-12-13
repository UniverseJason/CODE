/*
 * File: driver1.c 
 * YOUR NAME Jiaxing Li
 * YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 * 
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "mylinkedlist.h"

/* 
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 */
char *ReadLine(void);

void printMenu();

int main(int argc, char *arvg[])
{
  // YOU NEED TO IMPLEMENT THIS driver1.c USING FUNCTIONS FROM mylinkedlist.h
  // But before that, implement your ReadLine() function, and test it as shown below. 
  // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE

  // buffer and option
  char* buff;
  char* numbuff;
  char userOption;

  // linked list
  linked_list_T *stuList = NewLinkedList();

  // for information entered
  student_cell_T *tempStu;
  int id;
  double gpa;
  char* name;

  // other variable
  int length = -1;
  int index = -1;

  // main loop
  printMenu();
  while((buff = ReadLine()) != NULL)
  {
    sscanf(buff, "%c", &userOption);
    switch (userOption)
    {
      case '1':
        printf("Enter Student ID: ");
        id = atoi(ReadLine());

        printf("Enter Student GPA: ");
        numbuff = ReadLine();
        sscanf(numbuff, "%lf", &gpa);
        
        printf("Enter Student name: ");
        name = ReadLine();

        tempStu = NewStudentCell(id, gpa, name);
        Enlist(stuList, tempStu);
        break;
      
      case '2':
        tempStu = Delist(stuList);
        printf("Student %s has been deleted.\n", tempStu->name);
        break;

      case '3':
        length = LinkedListLength(stuList);
        printf("List length is %d\n", length);
        break;

      case '4':
        printf("Enter the index: ");
        index = atoi(ReadLine());
        tempStu = GetLinkedListElement(stuList, index);
        printf("Founded student on index %d, with name %s and ID %d.\n", index, tempStu->name, tempStu->id);
        break;

      case '5':
        printList(stuList);
        break;

      case '6':
        calculator(stuList);
        break;
      
      case '7':
        removeLargest(stuList);
        break;
        
      case '8':
      default:
        FreeLinkedList(stuList);
        printf("Linked list has been free and quit.\n");
        exit(0);
        break;
    }

    printMenu();
  }
  


  return 0;
}

void printMenu()
{
  printf("\n1 - Create a new student cell with given id, gpa, name info, and add it to the end of the linked list.\n");
  printf("2 - Remove the first student from linked list and print his/her id, gpa, name info\n");
  printf("3 - Print the number of students in the linked list\n");
  printf("4 - Print of a student at a specific index\n");
  printf("5 - Print the list of all studdents in the linked list. Print of every student\n");
  printf("6 - Print the min, average, max GPAs in the linked list\n");
  printf("7 - Remove the student with highest GPA and print his/her info\n");
  printf("8 - Exit\n");
  printf("Enter an Option: ");
}

/* 
 * IMPLEMENTATION of ReadLine();
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 *
 * In contrast to standard I/O functions (e.g., scanf with "%s", fgets) 
 * that can read strings into a given static size buffer, ReadLine function 
 * should read the given input line of characters terminated by a newline 
 * character ('\n') into a dynamically allocated and resized buffer based on 
 * the length of the given input line. 
 *
 * When implementing this function you can use standard I/O functions. 
 * We just want you to make sure you allocate enough space for the entered data. 
 * So don't simply allocate 100 or 1000 bytes every time. 
 * If the given input has 5 characters, you need to allocate space for 6 characters.
 *
 * Hint: initially dynamically allocate an array of char with size 10. 
 * Then, read data into that array character by charecter (e.g, you can use getchar()). 
 * If you see '\n' char before reading 10th character, you are done. And you know the 
 * exact length of the input string. So, accordingly allocate enough space and copy the 
 * data into new char array, insert '\0' instead of '\n' and free the original array. 
 * Then return the new string. However, if you DO NOT see '\n' char after 10th character, 
 * then you need larger space. Accordingly, resize your original array and double its size 
 * and continue reading data character by character as in the first step... 
 * Hope you got the idea! 
 *
 * Also please check for possible errors (e.g., no memory etc.) and appropriately handle 
 * them. For example, if malloc returns NULL, free partially allocated space and return 
 * NULL from this function. The program calling this function may take other actions, 
 * e.g., stop the program!
 */
char *ReadLine()
{
  // A SIMPLE WAY TO IMPLEMENT JUST TO TEST FOR NOW, BUT THIS IS NOT WHAT WE WANT!!!
  // char *buff = malloc(100);
  // scanf("%s", buff);
  // return buff;
  // YOU NEED TO DELETE ABOVE 3 LINES, and IMPLEMENT THIS as described above
  
  int line_length = 10;
  char *line = (char*)malloc((line_length + 1) * sizeof(char));
  if(line == NULL){
    fprintf(stderr, "ERROR: malloc fail\n");
    return NULL;
  }
  int i, myChar;

  // loop user input character one by one
  for(i=0; (myChar = getchar()) != EOF; ++i)
  {

    // if we need to stop reading the character when condition encounter
    if(i <= line_length && myChar == '\n')
    {
      line[i+1] = '\0'; // set the next character to \0 for the end of the string
      return line;
    }

    // if we have longer string than we expected
    if(i > line_length)
    {
      line_length *= 2;
      line = (char*)realloc(line, line_length); // resize the string user entered
      if(line == NULL)
      {
        fprintf(stderr, "ERROR: realloc fail\n");
        free(line);
        return NULL;
      }
    }
    
    line[i] = myChar;
  }
  return(NULL);   // if there is any error!
}