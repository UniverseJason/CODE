#include <stdio.h> 
typedef struct 
{
    char a;
    char b;
    double c;
    char d;
} alpha_t;

typedef struct 
{
    char a;
    char b;
    short c;
    int d;
    char e;
} beta_t;

typedef struct
{
    char message[1000];
} gamma_t;

static void fn1 (gamma_t param)
{
    param.message[0] = '1';
} 

static void fn2 (gamma_t *param)
{
    param->message[0] = '2';
}

int main (int argc, char *argv[])
{
    gamma_t x;
    gamma_t y;
    printf ("Size(char) = %ld\n", sizeof (char));
    printf ("Size(short) = %ld\n", sizeof (short));
    printf ("Size(int) = %ld\n", sizeof (int));
    printf ("Size(long) = %ld\n", sizeof (long));
    printf ("Size(long long) = %ld\n", sizeof (long long));
    printf ("Size(void *) = %1d\n", sizeof (void *));
    printf ("Size(alpha_t) = %1d\n", sizeof (alpha_t));
    printf ("Size(betat) = %ld\n", sizeof (beta_t));
    
    x.message[0] = 'x';
    fn1(x);
    printf ("x.message[0] = %c\n", x.message[0]);
    
    y.message[0] = 'y';
    fn2(&y);
    printf ("y.message[0] = %c\n", y.message[0]);
    
    return 0;
}