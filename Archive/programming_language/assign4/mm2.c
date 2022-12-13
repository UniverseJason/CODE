// gcc -Wall mm2.c -o mm2
// ./mm2 4 8 -5 0 20
// prints: mm2: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

static int x;   // min
static int y;   // max

static void mm2(int argc, char *argv[]) {
  x = 1000;
  y = -1000;
  
  int i;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num < x) x = num;
    if (num > y) y = num;
  }
}

int main(int argc, char *argv[]) {
  mm2(argc, argv);
  printf("mm2: min=%d max=%d\n", x, y);
  return 0;
}
