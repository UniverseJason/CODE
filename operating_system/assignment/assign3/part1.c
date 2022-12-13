#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Usage: ./part1 infile outfile.\n");
        return 1;
    }

    int i=0;

    unsigned long LA, PA;

    // set other PT list index  to 0
    unsigned int PT[32]={2,4,1,7,3,5,6};
    for(i=7; i<32; i++)
    {
        PT[i]=0;
    }

    unsigned int p=5, f=3, d=7;
    unsigned int pnum, fnum, dnum;

    // open the file 
    FILE *inFile = fopen(argv[1],"rb");
    if(inFile == NULL)
    {
        fprintf(stderr, "ERROR: can not read the file\n");
        exit(EXIT_FAILURE);
    }

    // open the writefile
    FILE *outFile = fopen(argv[2], "wb");
    if(outFile == NULL)
    {
        fprintf(stderr, "ERROR: can not open write the file\n");
        exit(EXIT_FAILURE);
    }

    // read file and get the corresponding LA and PA
    i = 0;
    while( fread(&LA, sizeof(LA), 1, inFile) != 0 )
    {
        dnum = LA & 0x7F;
        pnum = LA >> d;
        fnum = PT[pnum];
        PA = (fnum << d) + dnum;
        fwrite(&PA, sizeof(PA), 1, outFile);
        i++;
    }

    printf("Total number of page is %d\n", i);
    
    fclose(outFile);
    return EXIT_SUCCESS;
}