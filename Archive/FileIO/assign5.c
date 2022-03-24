#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define COURSE_FILE "courses.dat"
#define COURSE_TEST "coursesTest.dat"

typedef struct
{
	char course_Name[80];
	char course_Sched[4];
	unsigned course_Hours;
	unsigned course_Size;
	unsigned padding;
} COURSE;

void printMenu()
{
	printf("\nEnter one of the following actions or press CTRL-D to exit.\n");
	printf("C - create a new course record\n");
	printf("U - update an existing course record\n");
	printf("R - read an existing course record\n");
	printf("D - delete an existing course record\n");
	printf("F - find course record by string\n");
}

int main(int argc, char * argv[])
{
	// open file 
	FILE * inFile = fopen(COURSE_TEST,"rb+");
	if(inFile == NULL)
	{
		inFile = fopen(COURSE_TEST,"wb+");
	}
	
	// for regular operation
	COURSE crs;
	COURSE tempCrs;
	COURSE eptCrs;
	char userInput;
	char buffer[4000];
	int courseNum = -1;
	
	// for search
	

	COURSE tempSearchCourse;
	char key[500];
	int i;
	int searchNum = -1;
	bool ifNum = false;
	int flag = 0;
	printMenu();
	while(fgets(buffer, 5, stdin) != NULL)
	{
		buffer[strlen(buffer)-1] = 0;
		sscanf(buffer, "%c", &userInput);

		switch (userInput)
		{
			// read one file
			case 'r':
				// get buffer from stdin and seek the course
				printf("Enter a CS course number: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer)-1] = 0;
				sscanf(buffer, "%d", &courseNum);
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				fread(&crs, sizeof(COURSE), 1L, inFile);
				if(crs.course_Name[0] == '\0') // check if the course is empty
				{
					printf("ERROR: course not found\n");
					break;
				}
				// print information
				printf("Course number: %d\n", courseNum);
				printf("Course name: %s\n", crs.course_Name);
				printf("Scheduled days: %s\n", crs.course_Sched);
				printf("Credit hours: %d\n", crs.course_Hours);
				printf("Enrolled Students: %d\n", crs.course_Size);
				break;
			
			// update one record
			case 'u':
				printf("Enter a CS course number: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer)-1] = 0;
				sscanf(buffer, "%d", &courseNum);
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				fread(&crs, sizeof(crs), 1L, inFile); // for original corse
				if(crs.course_Name[0] == '\0') // check if the course is empty
				{
					printf("ERROR: course not found\n");
					break;
				}
				
				// get course name
				printf("Enter Course Name: ");
				fgets(buffer, 500, stdin);
				buffer[strlen(buffer)-1] = 0;
				strncpy(tempCrs.course_Name, buffer, 80);
				if(tempCrs.course_Name[0] == '\0')
				{
					strncpy(tempCrs.course_Name, crs.course_Name, 80);
				}
				
				// get course sche
				printf("Enter Course Schedule: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				strncpy(tempCrs.course_Sched, buffer, 4);
				if(tempCrs.course_Sched[0] == '\0')
				{
					strncpy(tempCrs.course_Sched, crs.course_Sched, 4);
				}

				// get course credit hours
				tempCrs.course_Hours = -1;
				printf("Enter Course Credit Hours: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				sscanf(buffer, "%d", &tempCrs.course_Hours);
				if (tempCrs.course_Hours == -1)
				{
					tempCrs.course_Hours = crs.course_Hours;
				}
				
				// get course enrollment
				tempCrs.course_Size = -1;
				printf("Enter Course Enrollment: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				sscanf(buffer, "%d", &tempCrs.course_Size);
				if(tempCrs.course_Size == -1)
				{
					tempCrs.course_Size = crs.course_Size;
				}

				// update the file
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				fwrite(&tempCrs, sizeof(COURSE), 1L, inFile);
				break;
			
			// write one record to file
			case 'c':
				// get course number
				printf("Enter Course Number: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer)-1] = 0;
				sscanf(buffer, "%d", &courseNum);
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				fread(&tempCrs, sizeof(tempCrs), 1L, inFile);
				if(tempCrs.course_Name[0] != '\0')
				{
					printf("ERROR: course already exists");
					break;
				}

				// get course name
				printf("Enter Course Name: ");
				fgets(buffer, 500, stdin);
				buffer[strlen(buffer)-1] = 0;
				strncpy(crs.course_Name, buffer, 80);

				// get course sche
				printf("Enter Course Schedule: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				strncpy(crs.course_Sched, buffer, 4);

				// get course credit hours
				printf("Enter Course Credit Hours: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				sscanf(buffer, "%d", &crs.course_Hours);

				// get course enrollment
				printf("Enter Course Enrollment: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer) - 1] = 0;
				sscanf(buffer, "%d", &crs.course_Size);
				
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				fwrite(&crs, sizeof(COURSE), 1L, inFile);
				break;

			// Delete a course
			case 'd':
				printf("Enter Course Number: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer)-1] = 0;
				sscanf(buffer, "%d", &courseNum);
				fseek(inFile, courseNum * sizeof(COURSE), SEEK_SET);
				if(crs.course_Name[0] == '\0') // check if the course is empty
				{
					printf("ERROR: course not found\n");
					break;
				} else {
					fwrite(&eptCrs, sizeof(COURSE), 1L, inFile);
					printf("course %d is successfully deleted\n", courseNum);
				}
				break;
			
			// search a course
			case 'f':
				printf("Enter a search phrease: ");
				fgets(buffer, 100, stdin);
				buffer[strlen(buffer)-1] = 0; // now buffer have the search key
				sscanf(buffer, "%d", &searchNum);
				
				// set for key
				if(searchNum != -1) // if user enter a number
				{
					sprintf(key, "%d", searchNum);
					searchNum = -1;
					ifNum = true;
				} else {
					strncpy(key, buffer, 99);
					ifNum = false;
				}
				
				// loop the file
				for(i=0; i<9999; i++)
				{
					fseek(inFile, i * sizeof(COURSE), SEEK_SET);
					fread(&tempSearchCourse, sizeof(COURSE), 1L, inFile);
					
					if(tempSearchCourse.course_Hours == atoi(key) && ifNum)
					{
						printf(" \"%d\" : Course Hours %d\n", i, tempSearchCourse.course_Hours);
					}
					else if (tempSearchCourse.course_Size == atoi(key) && ifNum)
					{
						printf(" \"%d\" : Course Size %d\n", i, tempSearchCourse.course_Size);
					}
					else if ( strcasestr(tempSearchCourse.course_Name, key) != NULL && !ifNum)
					{
						printf(" \"%d\" : %s\n", i, tempSearchCourse.course_Name);
					}
					else if ( strcasestr(tempSearchCourse.course_Sched, key) != NULL && !ifNum)
					{
						printf(" \"%d\" : %s\n", i, tempSearchCourse.course_Sched);
					}
				}
				break;

			// incorrect input
			default:
				printf("ERROR: invalid option\n");
				break;
		}
		printMenu();
	} // End of the while
	
	fclose(inFile);
	return 0;
}