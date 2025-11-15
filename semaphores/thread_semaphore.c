#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t odd_lock;
sem_t even_lock;

void* even_func(void* arg){
    for(int i = 0; i <= 10; i+=2){
        sem_wait(&even_lock);
        printf("Thread %llu : %d\n", pthread_self(), i);
        sem_post(&odd_lock);
    }
    
}

void* odd_func(void* arg){
    for(int i = 1; i <= 10; i+=2){
        sem_wait(&odd_lock);
        printf("Thread %llu : %d\n", pthread_self(), i);
        sem_post(&even_lock);
    }
}


int main() {
    sem_init(&even_lock, 0, 1);
    sem_init(&odd_lock, 0, 0);
    
    pthread_t t1;
    pthread_t t2;
    
    pthread_create(&t1, NULL, even_func, NULL);
    pthread_create(&t2, NULL, odd_func, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    sem_destroy(&odd_lock);
    sem_destroy(&even_lock);

    return 0;
}