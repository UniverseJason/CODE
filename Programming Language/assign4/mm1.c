// gcc -Wall mm1.c -o mm1
// ./mm1 4 8 -5 0 20
// prints: mm1: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

static int mm1_min(int argc, char *argv[]) {
  int min = 1000;
  int i;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num < min) min = num;
  }
  return min;
}


static int mm1_max(int argc, char *argv[]) {
  int max = -1000;
  int i;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num > max) max = num;
  }
  return max;
}

int main(int argc, char *argv[]) {
  int x = mm1_min(argc, argv);
  int y = mm1_max(argc, argv);
  printf("mm1: min=%d max=%d\n", x, y);
  return 0;
}
