#include<iostream>
#include<stdio.h>
#include <sys/ipc.h>
#include<sys/shm.h>
#define M 1000
using namespace std;
 int main(){
     //void* shared_memory;
     int shmid;
     
     int p=fork();
     if(p==0){//child process
         shmid=shmget((key_t)3456,1000,0666|IPC_CREAT);
        char *str = (char*) shmat(shmid,(void*)0,0);
        gets(str);
        printf("written data: %s",str);
        shmdt(str);

     }
     else{ //parent process
         shmid=shmget((key_t)3456,1000,0666|IPC_CREAT);
         char *str = (char*) shmat(shmid,(void*)0,0);
         printf("Data read: %s",str);
         shmdt(str);
         shmctl(shmid,IPC_RMID,NULL);

     }
     //input data
return 0;
    //  shmdt(str);
     


 }
