/*
* File: wordcount.c
* Author: Jiaxing Li (ycy354)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char ch;
    int counter = 0;
    int checker = 0; // this will check if there is a space in front of a world
    
    FILE *inFile = fopen(argv[1], "r");
    
    if(inFile == NULL)
    {
        fprintf(stderr, "wordcount PID = %d cannnot open file \"%s\"\n", getpid(), argv[1]);
        return 1;
    }
    else
    {
        while( (ch = fgetc(inFile)) != EOF )
        {
            if(ch == ' ' || ch == '\t' || ch =='\n' || ch == '\r')
            {
                checker = 0;
            }
            else if(checker == 0)
            {
                checker = 1; // we meet a world, set checker to 1 to avoid miscount
                counter++;
            }
        }
        printf("wordcount PID = %d counted words in file \"%s\" are %d\n", getpid(), argv[1], counter);
    }
    
    fclose(inFile);
    return 0;
}