#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int product = 0;

pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* consumer(){
    pthread_mutex_lock(&mutx);
    while(product == 0) pthread_cond_wait(&cond, &mutx);

    while (product != 0){
        product--;
        sleep(1);
        printf("consumer [%lu] is consuming product %d....\n",(unsigned long)pthread_self(), product);
    }

    pthread_mutex_unlock(&mutx);

}

void* producer(){

    for(int i = 0; i < 10; i++){
        pthread_mutex_lock(&mutx);
        product = product + 2;
        pthread_mutex_unlock(&mutx);
        printf("Producer [%lu] thread producing product %d \n",(unsigned long)pthread_self(), product);
        pthread_cond_broadcast(&cond);
    }

}

int main(){
    int s;
    pthread_t t1, t2, t3;

    s = pthread_create(&t1, NULL, producer, NULL);
    if(s != 0){
        perror("pthread_create");
    } 

    s = pthread_detach(t1);
    if(s != 0){
        perror("pthread_detach");
    }

    s = pthread_create(&t2, NULL, consumer, NULL);
    if(s != 0){
        perror("pthread_create");
    }
    s = pthread_detach(t2);
    if(s != 0){
        perror("pthread_detach");
    }

    s = pthread_create(&t3, NULL, consumer, NULL);
    if(s != 0){
        perror("pthread_create");
    }
    s = pthread_detach(t3);
    if(s != 0){
        perror("pthread_detach");
    }

    printf("main thread is exiting....\n");

    pthread_exit(0);
}