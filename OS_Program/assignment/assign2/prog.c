#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "progFunction.h"
#define FIFO 1
#define SJF 2
#define PR 3
#define RR 4

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        fprintf(stderr, "ERROR: incorrect number of argument.\nUsage: /prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n\n");
        exit(1);
    }

    int i;
    int AlgoChecker = false;
    int flagChecker = false;
    int inputChecker = false;
    int quantumChecker = false;
    int flag = -1;
    char *inFileName;
    int quantum = -1;

    // check the argument
    for(i=0; i<argc; i++)
    {
        if (strcmp("-alg", argv[i]) == 0)
        {
            AlgoChecker = true;
        }

        if (strcmp("FIFO", argv[i]) == 0)
        {
            flag = FIFO;
            flagChecker = true;
        }

        if (strcmp("SJF", argv[i]) == 0)
        {
            flag = SJF;
            flagChecker = true;
        }

        if (strcmp("PR", argv[i]) == 0)
        {
            flag = PR;
            flagChecker = true;
        }

        if (strcmp("RR", argv[i]) == 0)
        {
            flag = RR;
            flagChecker = true;
        }

        if (strcmp("-quantum", argv[i]) == 0)
        {
            quantumChecker = true;
            quantum = atoi(argv[i+1]);
        }

        if (strcmp("-input", argv[i]) == 0)
        {
            inputChecker = true;
            inFileName = argv[i+1];
        }
    }

    if(flag == RR && quantumChecker == false )
    {
        fprintf(stderr, "Usage: [RR] [-quantum integer(ms)]\n\n");
        exit(0);
    }

    if(inputChecker == false || AlgoChecker == false || flagChecker == false)
    {
        fprintf(stderr, "Usage: /prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n\n");
        exit(0);
    }

    // open file
    FILE *inputFile;
    if( (inputFile = fopen(inFileName, "r")) == NULL )
    {
        fprintf(stderr, "ERROR: can not open the file.\n");
        exit(1);
    }

    // malloc buffer
    char *buffer = (char*)malloc(64 * sizeof(char));
    if(buffer == NULL)
    {
        fprintf(stderr, "ERROR: buffer malloc fail.\n");
        exit(1);
    }

    // variable for basic steps in context-switching
    int id,pr,burst;
    PCB_st *myPCB;
    PCB_st *PCBList = newList();

    // read line and set up the linked list
    while ( fgets(buffer, 64, inputFile) != NULL )
    {
        sscanf(buffer, "%d %d %d", &id, &pr, &burst);
        myPCB = newPCB(id, pr, burst);
        EnList(PCBList, myPCB);
    }

    switch (flag)
    {
    case FIFO:
        FIFO_Scheduling();
        break;
    
    case SJF:
        SJF_Scheduling();
        break;
    
    case PR:
        PR_Scheduling();
        break;
    
    case RR:
        RR_Scheduling(PCBList, quantum);
        break;

    default:
        break;
    }

    fclose(inputFile);
    freeList(PCBList);
    free(buffer);
    return 0;
}