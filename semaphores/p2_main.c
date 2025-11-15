#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SEMAPHORE_ODD_NAME "/shared_semaphore_odd"
#define SEMAPHORE_EVEN_NAME "/shared_semaphore_even"

int main(){
    sem_t* lock_even = sem_open(SEMAPHORE_EVEN_NAME, O_CREAT, 0666, 1);
    sem_t* lock_odd = sem_open(SEMAPHORE_ODD_NAME, O_CREAT, 0666, 0);

    for(int i = 1; i <= 10; i+=2){
        sem_wait(lock_odd);
        printf("P %llu -> %d\n", (long long)getpid(), i);
        sem_post(lock_even);
    }

    sem_close(lock_even);
    sem_close(lock_odd);

}