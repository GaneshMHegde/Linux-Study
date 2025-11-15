#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condiction_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t condiction_full = PTHREAD_COND_INITIALIZER;

void* consumer(void* arg){
    while(1){
        pthread_mutex_lock(&lock);
    
        while(count == 0) pthread_cond_wait(&condiction_full, &lock);
        
        count -= 1;
        
        printf("Consumer count [%d]\n", count);
        
        if(count == 0) pthread_cond_signal(&condiction_empty);
        
        pthread_mutex_unlock(&lock);
    }
    
}

void* producer(void* arg){
    
    for(int i = 0; i < 5; i++){
        pthread_mutex_lock(&lock);
    
        while(count != 0) pthread_cond_wait(&condiction_empty, &lock);
        
        count += 5;
        
        printf("Producer: count [%d]\n", count);
        
        pthread_cond_signal(&condiction_full);
        
        pthread_mutex_unlock(&lock);
    }
}


int main(){
    pthread_t consumer_tid, producer_tid;
    
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);
    
    pthread_join(producer_tid, NULL);
    pthread_detach(consumer_tid);
    
    return 0;
}
