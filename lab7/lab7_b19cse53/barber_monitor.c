#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#define N 7

typedef struct barber_monitor{
    sem_t empchairs;
    sem_t filledchairs;
    pthread_t barber;
    pthread_mutex_t mut,mut1;
    pthread_t th[N];
} bm;

bm * barber;
void initialize_monitor(){
    sem_init(&(barber->empchairs),0,5);
    sem_init(&(barber->filledchairs),0,0);
    pthread_mutex_init(&(barber->mut),NULL);
    pthread_mutex_init(&(barber->mut1),NULL);
}

void * destroy_monitor(){
    sem_destroy(&(barber->empchairs));
    sem_destroy(&(barber->filledchairs));
    pthread_mutex_destroy(&(barber->mut));
    pthread_mutex_destroy(&(barber->mut1));
}

void * barberjob(void * args){
    pthread_mutex_lock(&(barber->mut));
    printf("barber engaged with %d\n",*(int *)args);
    sleep(5);
    printf("barber done with %d, %d leaves\n",*(int *)args,*(int *)args);
    // sleep(2);
    sem_post(&(barber->empchairs));
    sem_wait(&(barber->filledchairs));
    pthread_mutex_unlock(&(barber->mut));
}


void * routine(void * args){
    while (1){
        int myans;
        sem_getvalue(&(barber->empchairs),&myans);
        if (!myans) {       
            printf("customer %d is returning(no chairs left)\n",*(int *)args);  
                // return (void *)NULL;
            sleep(5);
            printf("customer %d has come back after %d mins\n",*(int *)args,5);
            continue;
                // pthread_create(&delivery_thread,NULL,&deliverpizza,(void *)a);
                // pthread_join(delivery_thread,NULL);
        }
        // printf("hi\n");
        sem_wait(&(barber->empchairs));
        sem_post(&(barber->filledchairs));
        printf("cutsomer %d sits on the chair\n",*(int *)args);
        pthread_mutex_lock(&(barber->mut1));
        int *myb = (int*)malloc(sizeof(int));
        *myb = *(int*)args;
        pthread_create(&(barber->barber),NULL,&barberjob,(void *)myb);
        pthread_join((barber->barber),NULL);
        pthread_mutex_unlock(&(barber->mut1));
        // sleep(6);
        // sem_post(&empchairs);
        // sem_wait(&filledchairs);
        // printf("customer %d leaves\n",*(int *)args);
        int vall;
        sem_getvalue(&(barber->filledchairs),&vall);
        if (!vall){
            printf("barber goes to sleep no customers are left\n");
        }
        sleep(55);
    }
}

void createthreads(){
    for (int i =0;i<N;i++) {
        int * a = (int *)malloc(sizeof(int));
        *a = i;
        if (pthread_create(&(barber->th[i]),NULL,&routine,(void*)a) != 0) {
            _exit(0);
        }
    }
    for (int i =0;i<N;i++) {
        pthread_join((barber->th[i]),NULL);
    }
}


int main(){
    barber = (bm *)malloc(sizeof(bm)); 
    initialize_monitor();
    createthreads();    
    destroy_monitor();
    return 0;
}