#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef struct page_table
{
    unsigned int pt_fnum;
    int vi;
}page_table;

int find_empty_frame(int *numlist, int length)
{
    int i = 0;
    for(i=1; i<length; i++)
    {
        // we find a empty frame, return the index of the frame
        if(numlist[i] == 1)
        {
            numlist[i] = 0;
            return i;
        }
    }

    // no empty frame
    return -1;
}

int LRU_find(int *numlist, int length)
{
    int i = 0;
    int min = numlist[1];

    for(i=1; i<length; i++)
    {
        if(min > numlist[i])
        {
            min = numlist[i];
        }
    }

    for(i=1; i<length; i++)
    {
        if(min == numlist[i])
        {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Usage: ./part1 infile outfile.\n");
        return 1;
    }

    unsigned long LA, PA;
    unsigned int p=5, f=3, d=7;
    unsigned int pnum, fnum, dnum;

    int CLK = 0, i = 0;
    int freeframe[8] = {0,1,1,1,1,1,1,1};
    int LRUcount[8] = {0};
    int revmap[8] = {-1};

    page_table PT[32];
    for(i=0; i<32; i++)
    {
        PT[i].vi = 0;
    }

    // open the file 
    FILE *inFile = fopen("part2sequence","rb");
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
    int x = 0;
    int y = 0;
    int page_fault = 0;

    while( fread(&LA, sizeof(LA), 1, inFile) != 0 )
    {
        CLK++;
        dnum = LA & 0x7F;
        pnum = LA >> d;
        if(PT[pnum].vi == 1) // valid bit 
        {
            fnum = PT[pnum].pt_fnum;
            PA = (fnum << d) + dnum;
            LRUcount[fnum] = CLK;
        }
        else
        {
            page_fault++;
            x = find_empty_frame(freeframe, 8);
            if(x > 0)
            {
                PT[pnum].pt_fnum = x;
                PT[pnum].vi = 1;
                fnum = PT[pnum].pt_fnum;
                PA = (fnum << d) + dnum;
                revmap[x] = pnum;
                LRUcount[fnum] = CLK;
            }
            else // no empty frame, run LRU
            {
                y = LRU_find(LRUcount, 8);
                PT[revmap[y]].vi = 0;
                PT[pnum].pt_fnum = y;
                PT[pnum].vi = 1;
                fnum = PT[pnum].pt_fnum;
                PA = (fnum << d) + dnum;
                LRUcount[fnum] = CLK;
                revmap[fnum] = pnum;
            }
        }
        fwrite(&PA, sizeof(PA), 1, outFile);
    }

    printf("Part 2 page faults: %d\n", page_fault);
    fclose(outFile);

    return EXIT_SUCCESS;
}