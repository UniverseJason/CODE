#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double BALANCE = 0.0;
pthread_mutex_t lock;

typedef struct param
{
    int x;
    double y;
}paramT;

void *deposit(void *args){
    int i;
    struct param *ptr = (struct param *) args;
    for(i=0; i<ptr->x; i++) {
        pthread_mutex_lock(&lock);
        BALANCE += ptr->y; // b
        pthread_mutex_unlock(&lock);
    }
}

void *withdraw(void *args){
    int i;
    struct param *ptr = (struct param *) args; 
    for(i=0; i<ptr->x; i++) {
        pthread_mutex_lock(&lock);
        BALANCE -= ptr->y; // d
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[])
{
    struct param dep, with;
    pthread_t t1, t2;
    pthread_mutex_init(&lock,NULL);
    dep.x=atoi(argv[1]); // a
    dep.y=atof(argv[2]); // b
    with.x=atoi(argv[3]); // a
    with.y=atof(argv[4]); // b

    pthread_create(&t1,NULL, deposit, &dep);
    pthread_create(&t2,NULL, withdraw, &with);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    double total = (dep.x  * dep.y) - (with.x * with.y);
    printf("B = %lf vs T = %lf\n", BALANCE, total);
    
    return 0;

}