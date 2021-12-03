#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
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

int main(int argc , char * argv[]) {
    if (argc < 2) {
        printf("please provide port number\n");
        return -1;
    }

    int sockfd , newsockfd, port ,n;
    char buffer[1024];

    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;
    

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0) {
        printf("socket faliled \n");
        return 1;
    }

    bzero((char*)&serv_addr,sizeof(serv_addr));
    port = atoi(argv[1]);

    // if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))) {
    //     printf("cannot set socket\n");
    //     return 2;
    // }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) {
        printf("bind error\n");
        return 3;
    }

    if (listen(sockfd,5) <0) {
        printf("listen failed\n");
        return 4;
    }
    clilen = sizeof(cli_addr);
    if ((newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,(socklen_t*)&clilen)) <0) {
        printf("error \n");
        return 5;
    }
    
    while (1) {
        bzero(buffer,1024);
        printf("listening ... \n");
        n = read(newsockfd,buffer,1024);
        if (n < 0) {
            printf("error reading msg\n");
        }

        printf("from client : %s\n",buffer);
        int vall = calculate_result(buffer);
        bzero(buffer,1024);
        // printf("enter msg to send : ");
        // fgets(buffer,1024,stdin);
        if (vall != INT_MIN) {
            strcpy(buffer,int_to_str(vall));
        } else {
            strcpy(buffer,"invalid expression only ADDITION , SUBTRACTION , DIVISION, MULTIPLICATION operation is allowed");
        }


        n = write(newsockfd,buffer,strlen(buffer));
        if (n < 0) {
            printf("error sending msg\n");
        }
    }

    close(newsockfd);
    close(sockfd);

    // send(new_socket,hello,strlen(hello),0);
    // printf("msg sent\n");
    
    // char arr[50];
    // // fflush(stdin);
    // printf("enter a string : ");
    // fgets(arr,50,stdin);
    // arr[strlen(arr)-1] = '\0';
    // int vall = calculate_result(arr);
    // char * fun;
    // strcpy(fun,int_to_str(vall));
    // printf("%s\n",fun);
    // // printf("%d\n",'\n'-'0');
    return 0;
}