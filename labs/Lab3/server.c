#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define Max 1024



int delta(char * s,char * msg) {
    int svalue=0,msgvalue=0;
    int slen = strlen(s);
    int msglen = strlen(msg);
    for (int i =0;i<slen;i++) {
        svalue += (s[i]) % slen;
    }

    for (int i =0;i<msglen;i++) {
        msgvalue += (msg[i]) % msglen;
    }

    svalue %= slen;
    msgvalue %= msglen;
    int d = svalue - msgvalue;
    return (d < 0)?-d:d;
}


// void getstringinput(char * question) {
//     fflush(stdin);
//     char c = getchar();
//     int i =0;
//     while (c!='\n') {
//         question[i++] = c;
//         c = getchar();
//     }

//     question[i] = '\0';
// }

struct my_msg_st {
    long my_msg_type;
    char some_text[BUFSIZ];
};

int main(int argc, char * argv[]) {

    int running = 1;
    struct my_msg_st some_data,send_data;
    long int msg_to_receive=0;
    char buffer[BUFSIZ];

    send_data.my_msg_type =1;

    int msgid = msgget((key_t)1234,0666| IPC_CREAT); //recieve
    int msgid2 = msgget((key_t)2345,0666 | IPC_CREAT);//send

    if (msgid == -1 || msgid2 == -1) {
        printf("message failed \n");
        _exit(1);
    }

    while (running) {
        printf("\n waiting \n");
        if (msgrcv(msgid,(void *)&some_data,BUFSIZ,msg_to_receive,0) == -1) {//eads a message from the queue associated with the message queue identifier specified by msgid and places it in the user-defined buffer pointed
            printf("message failure\n");  
        } 
        printf("Your wrote : %s \n",some_data.some_text);
        if (strncmp(some_data.some_text,"end",3) == 0) {
            running = 0;
        } else {
            printf("Enter some text : ");
            fgets(buffer,BUFSIZ,stdin);
            send_data.my_msg_type=1;
            strcpy(send_data.some_text,buffer);
            if (msgsnd(msgid2 , (void *)&send_data,Max,0) == -1) {
                printf("message failure \n");
                _exit(1);
            }
            if (strncmp(buffer,"end",3) == 0) {
                running = 0;
            }

        }

        if (msgctl(msgid,IPC_RMID,0) == -1) {
            printf("failure \n");
            _exit(1);
        }

        _exit(1);

    }

    return 0;
}


    // char question[Max];
    // char ques[Max];
    // getstringinput(question);
    // getstringinput(ques);

    // printf("%s\n %s\n",question,ques);

    // printf("%d",delta(question,ques));

    // int k; scanf("%d",&k);
    // int * clients = (int *)malloc(k*sizeof(int));

    // struct msg_buffer message;
    // int msgid = msgget(100, 0666 | IPC_CREAT);
    // printf("%d \n",msgid) ;
    // message.msg_type = 1; 
    // printf("Write Data : ");
    // scanf("%[^\n]", message.msg_text);
    // msgsnd(msgid, &message, sizeof(message), 0);
  
    // // printf("Data send is : %s \n", message.msg_text);