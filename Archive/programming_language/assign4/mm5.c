// gcc -Wall mm5.c -o mm5
// ./mm5 4 8 -5 0 20
// prints: mm5: min=-5 max=20

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;   // min
  int y;   // max
} mm_t;

static mm_t *mm5(int argc, char *argv[]) {
  mm_t *mm = (mm_t *) malloc(sizeof(mm_t));
  int i;
  for (i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num < mm->x) mm->x = num;
    if (num > mm->y) mm->y = num;
  }
  return mm;
}

int main(int argc, char *argv[]) {
  mm_t *mm = mm5(argc, argv);
  printf("mm5: min=%d max=%d\n", mm->x, mm->y);
  return 0;
}
