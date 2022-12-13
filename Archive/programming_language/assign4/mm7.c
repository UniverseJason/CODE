// gcc -Wall mm7.c -o mm7
// ./mm7 4 8 -5 0 20
// prints: mm7: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

#define mm7(argc, argv, x, y) \
  x = 1000; \
  y = -1000; \
  int i; \
  for (i = 1; i < argc; i++) { \
    int num = atoi(argv[i]); \
    if (num < x) x = num; \
    if (num > y) y = num; \
  }


int main(int argc, char *argv[]) {
  int x, y;
  mm7(argc, argv, x, y);
  printf("mm7: min=%d max=%d\n", x, y);
  return 0;
}
