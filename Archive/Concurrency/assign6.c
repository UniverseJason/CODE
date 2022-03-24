#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void getArgList(char* list[], int i, int length);

int main(int argc, char* argv[])
{
    int i=1, j=1, idx; // idx for argc
    int piExitStatus = 0;
    
    int len = 0;
    for(idx=1; idx<argc; idx++){
        
        // found a ',' Need to adjust start and end position of the current argument
        if (strcmp(argv[idx], ",") == 0)
        {
            j = idx;
            len = j - i + 1;
            getArgList(argv, i, len);
            i = ++idx;
        }
    }
    
    j = argc;
    len = j - i + 1;
    getArgList(argv, i, len);
    
    // wait for all child
    long waitID;
    while(waitID == wait(&piExitStatus) > 0)
    {
        if(waitID == -1)
        {
            printf("Wait ERROR!\n");
            return -1;
        }
    }
    
    return 0;
}

/*
*   argList - argument list
*   startIDX - start index for the fist argument in a argument list.
*   len - length of the prefix char* for execvp
*/
void getArgList(char* argList[], int startIDX, int len) {

    char* args[len];
    int i;
    for (i = 0; i < len - 1; i++) 
    {
        args[i] = argList[i + startIDX];
    }
    args[len - 1] = NULL; // last element is NULL 

    // use execvp
    long forkPID;
    forkPID = fork();
    if(forkPID == -1) // failed
    {
        printf("fork failed\n");
        return;
    }
    else if(forkPID == 0) // for child procress
    {
        printf("PID: %d, PPID: %d, CMD: %s\n",getpid(), getppid(), argList[startIDX]);
        execvp(argList[startIDX], args);        
    }
    else // for parent procress
    {
        return;
    }
}