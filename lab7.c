#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

sem_t s1,s2;
pthread_mutex_t mutex,mutex2;
int slices =5;

void* student(void* arg){
    int i=*(int*)arg
    while(1){
        sem_wait(&s1);
        pthread_mutex_lock(&mutex);
        s--;
        printf("Student %d has eaten now",&i);
        if(s==0){
            //pthread_mutex_unlock(&mutex);
            sem_post(&s2);
        }
        else{

        pthread_mutex_unlock(&mutex);
        }




    }
}

void* pizza(){
    while(1){
        sem_wait(&s2);
        
        pthread_mutex_lock(&mutex);
        sleep(2);
        printf("PIZZA delivered\n");
        slices=5;
        
        for(int i=0;i<5;i++){
            sem_post(&s1);
        }
        pthread_mutex_unlock(&mutex);


    }

}



int main(){
    pthread_t th[5];
    pthread_t t;
    sem_init(&s1,0,5);
    sem_init(&s2,0,1);
    pthread_mutex_init(&mutex, NULL);
        int i;
    for (i = 0; i < 5; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &student, a) != 0) {
            perror("Failed to create thread");
        }
      
    }
      if(pthread_create(&t, NULL, &pizza, NULL)!=0){
            perror("Failed to create thread");
            

        }
     for (i = 0; i < 5; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

     if(pthread_join(t,NULL)!=0){
         perror("Failed to join thread");

     }
     sem_destroy(&s1);
     pthread_mutex_destroy(&mutex);


}
