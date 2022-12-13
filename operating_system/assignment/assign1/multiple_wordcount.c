/*
* File: multiple_wordcount.c
* Author: Jiaxing Li (ycy354)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // check argc number
    if(argc < 2)
    {
        fprintf(stderr, "ERROR: Incorrect number of argument.\n");
        exit(1);
    }

    // fork for child
    // sample: execl("./wordcount", "wordcount", argv[1],(char*)NULL);
    int i = 0, exeCheck;
    pid_t childPID = 0, waitPID = 0;
    for(i = 1; i < argc; i++)
    {
        if( (childPID = fork()) == -1 )
        {
            perror("ERROR: Failed to fork()");
            exit(EXIT_FAILURE);
        }
        else if( childPID == 0 ) // fork success, creat a new procress
        {
            exeCheck = execl("./wordcount", "wordcount", argv[i], (char*)NULL);
            if(exeCheck == -1)
            {
                perror("ERROR: execution failed");
                exit(2);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // wait for all child
    while( (waitPID == wait(NULL)) > 0)
    {
        if(waitPID == -1)
        {
            printf("ERROR: wait failed\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}