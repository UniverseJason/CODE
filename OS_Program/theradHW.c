#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 1 - Pass a long value to a thread (special case – pass the value of long as pointer value)
void *myth1(void *arg){
    long myi = *((long*)arg);
    printf("%ld",myi);
}

// 2 - Pass a long value to a thread (general case- pass the address of long variable)
void *myth2(void *arg){
    long myi = *((long*)arg);
    printf("%ld",myi);
}

// 3 - Pass a double value to a thread (general case- pass address of double variable)
void *myth3(void *arg){
    double myi = *((double*)arg);
    printf("%f",myi);
}

// 4 - Return a long value to main (special case – return the value of long as pointer)
void *myth4(void *arg)
{
    long myi = 3733;
    pthread_exit((void *)myi);
}

// 5 - Return a long value to main (general case – dynamically allocate space, ...)
void *myth5(void *arg)
{
    long myi = 3733;
    long *target = (long *)malloc(sizeof(long));
    *target = myi;
    pthread_exit(target);
}
// 6 - Return a double value to main (general case – dynamically allocate space, ...)
void *myth6(void *arg)
{
    double myi = 3733.001;
    double *val = (double *)malloc(sizeof(double));
    *val = myi;
    pthread_exit(val);
}

int main()
{
    // 1 - main
    pthread_t tid;
    long i = 3733;
    long *p = &i;
    pthread_create(&tid, NULL, myth1, p);
    pthread_join(tid, NULL);

    // 2 - main
    pthread_t tid;
    long i = 3733;
    pthread_create(&tid, NULL, myth2, &i);
    pthread_join(tid, NULL);

    // 3 - main
    pthread_t tid;
    double d = 3733.001;
    pthread_create(&tid, NULL, myth3, &d);
    pthread_join(tid, NULL);

    // 4 - main
    pthread_t tid;
    long i;
    long *ptr;
    pthread_create(&tid, NULL, myth4, NULL);
    pthread_join(tid, (void**)&ptr);
    i = (long)ptr;

    // 5 - main
    pthread_t tid;
    long i;
    void *ptr;
    pthread_create(&tid, NULL, myth5, NULL);
    pthread_join(tid, &ptr);
    i = *(long *)ptr;

    // 6 - main
    pthread_t tid;
    double d;
    void *value;
    pthread_create(&tid, NULL, myth6, NULL);
    pthread_join(tid, &value);
    d = *(double *)value;


    return 0;
}