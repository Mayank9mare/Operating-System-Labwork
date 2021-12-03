#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(){
    int pipes[2];
    char read_ms[1000];
    char write_msg[1000]={"HELLO"};
    int r=pipe(pipes);
    if(r==-1){
        printf("error");
        return 0;
    }
    write(pipes[1],write_msg,sizeof(write_msg));
    read(pipes[0],read_ms,sizeof(read_ms));
    printf("Recieved Message: %s",read_ms);
    return 1;

 


}