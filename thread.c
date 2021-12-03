#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
 void *sam(){
     printf("thread");
 }
 int main(){
     pthread_t pid;
     pthread_create(&pid,NULL,&sam,NULL);
     pthread_join(pid,NULL);
     return 0;
 }