// gcc -Wall mm4.c -o mm4
// ./mm4 4 8 -5 0 20
// prints: mm4: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;   // min
  int y;   // max
} mm_t;

static void mm4(int argc, char *argv[], mm_t *mm) {
  int i;
  (*mm).x = 1000;
  (*mm).y = -1000;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num < mm->x) (*mm).x = num;
  }
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num > mm->y) (*mm).y = num;
  }
}

int main(int argc, char *argv[]) {
  mm_t mm;
  mm4(argc, argv, &mm);
  printf("mm4: min=%d max=%d\n", mm.x, mm.y);
  return 0;
}
