#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define N 5
// const int N= 5;

sem_t chop[N];

void* take(void* l){
        int p=*(int*)l;
        sem_wait(&chop[p]);
}
void* frees(void* l){
        int p=*(int*)l;
        sem_post(&chop[p]);
}
void* philos(void* n){
        int p=*(int*)n;//particular philospher
        printf("Philospher %d start thinking\n",p);
        printf("Philospher %d try to take both chopstick\n",p);
        // sem_wait(&chop[p]);
        // printf("Philospher %d takes left chopstick\n",p);
        // printf("Philospher %d try to take right chopstick\n",p);
        int r=(p+1)%N;
        // sem_wait(&chop[r]);
        pthread_t p1,p2;
        pthread_create(&p1,NULL,take,(void*)&p);
        pthread_create(&p2,NULL,take,(void*)&r);
        pthread_join(p1,NULL);
        pthread_join(p2,NULL);
        printf("Philospher %d takes both chopstick\n",p);
        printf("Philospher %d starts eating\n",p);
        sleep(2);
        printf("Philospher %d ends eating\n",p);
        pthread_t p3,p4;
        pthread_create(&p1,NULL,frees,(void*)&p);
        pthread_create(&p2,NULL,frees,(void*)&r);
        pthread_join(p1,NULL);
        pthread_join(p2,NULL);

        //sem_post(&chop[r]);
        printf("Philospher %d frees both chopstick\n",p);
        //sem_post(&chop[p]);
        //printf("Philospher %d frees left chopstick\n",p);


}

int main(){
        int n;
        int a[N];
        pthread_t T[N];
        for(int i=0;i<N;i++){
                sem_init(&chop[i],0,1);
        }

        for(int i=0;i<N;i++){
                a[i]=i;
                pthread_create(&T[i],NULL,philos,(void*)&a[i]);
        }
        for(int i=0;i<N;i++){
                pthread_join(T[i],NULL);

        }
        return 0;


}