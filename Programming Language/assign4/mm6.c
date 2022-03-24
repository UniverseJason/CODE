// gcc -Wall mm6.c -o mm6
// ./mm6 4 8 -5 0 20
// prints: mm6: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

static void mm6(int argc, char *argv[], int *x, int *y) {
  *x = 1000;
  *y = -1000;
  
  int i;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num < *x) *x = num;
    if (num > *y) *y = num;
  }
}

int main(int argc, char *argv[]) {
  int x, y;
  mm6(argc, argv, &x, &y);
  printf("mm6: min=%d max=%d\n", x, y);
  return 0;
}
