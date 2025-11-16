#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SHARED_MEMORY_NAME "/producer_consumer_shm"

int main(){
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 1000);

    char* ptr = (char*)mmap(NULL, 1000, PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, 0);

    int offset = 0;
    for(int i = 0; i < 10; i++){
        sprintf(ptr+offset, "hello %d\n", i);
        offset+=strlen("hello %d\n");
    }

    munmap(ptr, 1000);
}