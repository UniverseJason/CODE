#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // if(argc < 3)
    // {
    //     fprintf(stderr, "Usage: ./part1 infile outfile.\n");
    //     return 1;
    // }

    int i=0;

    unsigned long LA, PA;
    unsigned int PT[32]={2,4,1,7,3,5,6};
    
    // set other PT list index  to 0
    for(i=7; i<=32; i++)
    {
        PT[i]=0;
    }

    unsigned int p=5, f=3, d=7;
    unsigned int pnum, fnum, dnum;

    // open the file 
    FILE *inFile = fopen("part1testsequence","rb");
    if(inFile == NULL)
    {
        fprintf(stderr, "ERROR: can not read the file\n");
        exit(1);
    }

    // read file 
    while( fread(&LA, sizeof(LA), 1, inFile) != 0)
    {
        printf("%lx\n", LA);
    }
    printf("\n");

    return 0;
}