#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]){
    int newc;
    char **newv;
    int i,j;

    newc = argc;
    newv = (char**)malloc((newc+1) * sizeof(char*));
    if(newv == NULL) exit(1);

    for(i=0; i<argc; i++){
        newv[i] = malloc(strlen(argv[i]) + 1);
        if(newv[i] == NULL) exit(1);
        strcpy(newv[i], argv[i]);
    }
    newv[i] = NULL;
    
    printf("argc = %d\n", argc);
    printf("newc = %d\n", newc);
    for(i=0; i<newc; i++){
        printf("newv[%d] = %s\n", i, newv[i]);
    }

    for(i=0; i<argc; i++){
        free(newv[i]);
    }
    free(newv);
}