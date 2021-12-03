#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

char * int_to_str(int n) {
    int ref = n;
    char value[50];
    int ctr = 0;
    if (n<0) {
        n = -n;
        value[ctr++] = '-';
    }
    int arr[50];
    int arrctr=0;
    while (n > 0) {
        int mod = n % 10;
        n /= 10;
        arr[arrctr++] = mod;
    }

    int len = arrctr;
    for (int i = len-1;i>=0;i--) {
        value[ctr++] = arr[i]+'0';
    }

    value[ctr] = '\0';

    // printf("printing ans : %s\n",value);
    char * myans;
    myans = value;
    // printf("%s\n",myans);
    return myans;
}

int calculate_result(char * value) {
    int flag=0;
    int i =0;
    int add=0,sub=0,div=0,mul=0;
    int arr[2];
    int arrptr=0;
    while (value[i] != '\0') {
        int val = value[i] - '0';
        // printf("val : %d\n",val);
        if (value[i] == ' ') {
            // printf("space found \n");
            i++;continue;
        } if (val < 0) {
            if (val == -6) {
                mul=1;
            } else if (val == -5) {
                add=1;
            } else if (val == -3) {
                sub =1;
            } else if (val == -1) {
                div = 1;
            } else {
                // printf("doing flag1\n");
                flag =1;
            }
            i++;
        } else if (val >=0 && val <=9) {
            char num[50];
            int numptr=0;
            num[numptr++] = value[i++];
            while ((value[i]-'0') >=0 && (value[i]-'0')<=9) {
                num[numptr++] = value[i++];
            }
            num[numptr] ='\0';
            int actualnum = atoi(num);
            arr[arrptr++] = actualnum;
        } else {
            i++;
            // printf("doing flag2\n");
            flag = 1;
        }
    }
    if (flag) {
        return INT_MIN;
    }
    
    int ans =0;
    if (add == 1) {
        ans = (arr[0] + arr[1]);
    } else if (sub==1) {
        ans = (arr[0] - arr[1]);
    } else if (div == 1) {
        ans = (arr[0]/arr[1]);
    } else if (mul == 1) {
        ans = (arr[0] * arr[1]);
    }

    // char *finalans;
    // finalans = atoa(ans);
    return ans;

}

char client_message[512];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg) {
    int newSocket = *((int *)arg);
    int a;
    while (1) {
        bzero(buffer,1024);
        bzero(client_message,512);
        a = recv(newSocket , client_message , 512 , 0);
        if (!strcmp(client_message,"exit")) {
            break;
        }

        printf("from client: %s\n",client_message);
    
        pthread_mutex_lock(&lock);
        strcpy(buffer,client_message);
        int vall = calculate_result(buffer);
        if (vall != INT_MIN) {
            strcpy(buffer,int_to_str(vall));
        } else {
            strcpy(buffer,"invalid expression only ADDITION , SUBTRACTION , DIVISION, MULTIPLICATION operation is allowed");
        }
        pthread_mutex_unlock(&lock);
        send(newSocket,buffer,1024,0);
    }
    
    // printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(NULL);
}

int main(int argc , char * argv[]) {
    if (argc < 2) {
        printf("provide some port\n");
        return -1;
    }

    int serverSocket, newSocket, port;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("cannot connect socket\n");
        return -1;
    }

    port = atoi(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    int binder;
    binder = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (binder < 0) {
        printf("error binding the server\n");
        return -1;
    }

    if(listen(serverSocket,50) < 0) {
        printf("Listen failed \n");
        return -1;
    }
    
    
    pthread_t tid[60];
    int i = 0;
    while(1) {

        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) < 0 )
            printf("Failed to create thread\n");

        if( i >= 50) {
            i = 0;
            while(i < 50) {
                pthread_join(tid[i++],NULL);
            }
            i = 0;
        }
    }

    return 0;

}