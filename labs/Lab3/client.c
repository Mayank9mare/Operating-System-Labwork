#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define Max 1024
struct msg_buffer {
    long msg_type;
    char msg_text[Max];
};

// likhte jaa 11:30 tak uske bad se mai likh rha hoo
int  main(int argc , char * argv[]){
    int running =1;
    int msgid,msgid2;
    struct msg_buffer some_data,send_data;
    long int msg_to_recieve=0;
    char buffer[BUFSIZ];
    send_data.msg_type=1;
    msgid=msgget((key_t)2345,0666|IPC_CREAT);
    msgid2=msgget((key_t)1234,0666|IPC_CREAT);
    if(msgid==-1 || msgid2==-1){
        fprintf(stderr,"error\n");
        _exit(1);
    }
    while(running){
        printf("Enter text: ");
        fgets(buffer,BUFSIZ,stdin);
        send_data.msg_type=1;
        strcpy(send_data.msg_text,buffer);
        if(msgsnd(msgid2,(void *)&send_data,Max,0)==-1){
            fprintf(stderr,"send failed\n");
            _exit(1);

        }
        if(strncmp(buffer,"end",3)==0){
            running=0; 
        }
        else{
            printf("\nWaiting \n");
            if(msgrcv(msgid,(void*)&some_data,BUFSIZ,msg_to_recieve,0)==-1){
                fprintf(stderr,"msgrcv failed\n");
                _exit(1);
            }
            printf("Message: %s",some_data.msg_text);
            if(strncmp(some_data.msg_text,"end",3)==0){
                running =0;

            }
        }
    }
    if(msgctl(msgid,IPC_RMID,0)==-1){
        fprintf(stderr,"msgctl Failed\n");
        _exit(1);
    }
    return 0;

    
}