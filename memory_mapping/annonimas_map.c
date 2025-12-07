#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    char* addr = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    memset(addr, 0, 100);
    strcpy(addr, "hello from parent");

    if(fork() == 0){
        printf("CHILD(%llu) MESSAGE : %s\n", (unsigned long long)getpid(), addr);
        memset(addr, 0, 100);
        strcpy(addr, "hello from child");
    }
    else{
        wait(NULL);
        printf("PARENT(%llu) MESSAGE : %s\n", (unsigned long long)getpid(), addr);
    }

    exit(EXIT_SUCCESS);
}