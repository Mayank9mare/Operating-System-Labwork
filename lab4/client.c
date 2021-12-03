#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


// #define PORT 8080

int main(int argc , char * argv[]) {
    if (argc < 3) {
        printf("enter the required values carefully\n");
        return -1;
    }
    
    int sockfd,port,n;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    port = atoi(argv[2]);

    char buffer[1024];
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("error connecting socket\n");
        return 1;
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("server may be not started or you have mentioned wrong ip\n");
        return -1;
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(port);

    

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("connection error \n");
        return 3;
    }

    while (1) {
        bzero(buffer,1024);
        printf("enter msg to send : ");
        fgets(buffer,1024,stdin);
        buffer[strlen(buffer)-1] = '\0';

        n = write(sockfd,buffer,strlen(buffer));

        if (n < 0) {
            printf("error on writing \n");
        }

        bzero(buffer,1024);
        printf("listening ... \n");
        n = read(sockfd,buffer,1024);
        if (n < 0) {
            printf("error on reading \n");
        }

        printf("from server : %s\n",buffer);
    }

    close(sockfd);

    // valread = read(sock,buffer,1024);
    // printf("%s\n",buffer);
    return 0;
}