#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SHARED_MEMORY_NAME "/producer_consumer_shm"

int main(){
    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);

    char* ptr = mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    for(int i = 0 ; i < 1000; i++){
        printf("%c", ptr[i]);
    }
}