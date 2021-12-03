#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


pthread_t delivery_thread;
sem_t pizza;
int ordered = 0;
void * deliverpizza(void * args) {
    printf("%d ordered pizza and %d goes to sleep\n",*(int *)args,*(int *)args);

    sleep(rand()%6+13);
    printf("the pizza is delivered \n");
    for (int i =0;i<5;i++) {
        sem_post(&pizza);
    }
}

void * routine(void * args) {
    // printf("%d talking from deliver pizza\n",*(int *)args);
    // printf("%d\n",*(int *)args);
    while (1){
        int myans;
        sem_getvalue(&pizza,&myans);
        if (!myans) {
            if (!ordered){
                ordered =1;
                int * a = malloc(sizeof(int));
                *a = *(int *)args;
                pthread_create(&delivery_thread,NULL,&deliverpizza,(void *)a);
                pthread_join(delivery_thread,NULL);
                sleep(4);
                ordered = 0;    
            }else{
                printf("%d goes to sleep\n",*(int *)args);
            }

        }
        sem_wait(&pizza);
        printf("%d got pizza slice\n",*((int *)args));
        sleep((rand()%5 +1));
        printf("%d finishes pizza slice\n",*((int *)args));
        // sem_post(&pizza);
    }
}

int main(int argc, char * argv[]) {
    
    sem_init(&pizza,0,5);
    int * sem_val;
    // pthread_create(&delivery_thread,NULL,&deliverpizza,NULL);
    // pthread_join(delivery_thread,NULL);
    pthread_t th[4];
    for (int i =0;i<4;i++) {
        int * a = (int *)malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i],NULL,&routine,(void*)a) != 0) {
            _exit(0);
        }
    }
    for (int i =0;i<4;i++) {
        pthread_join(th[i],NULL);
    }
    sem_destroy(&pizza);
    return 0;
}