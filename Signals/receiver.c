#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void usageError(const char* msg){
    printf("%s",msg);
    printf("\n just run the binary like ./reciver \n");
    exit(1);
}

void handle1(int sig){
    printf("recived signal type is %d\n", sig);
}

void handle2(int sig){
    printf("i am going to kill myself for you.\n");
    exit(0);
}

int main(int argc, char* argv[]){
    if(argc > 2 || strcmp(argv[1], "--help")) usageError("you should use it like this:");

    void (*oldhandle)(int);

    oldhandle = signal(SIGINT, handle1);
    printf("old handle for SIGINT is %p\n", oldhandle);
    oldhandle = signal(SIGQUIT, handle2);
    while(1){
        pause();
    }

}