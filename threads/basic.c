#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int global = 0;

pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

void* thread_fun(){
    for(int i =0; i < 5; i++){
        pthread_mutex_lock(&mutx);
        global++;
        printf("Thread %lu counting %d ...\n", (unsigned long)pthread_self(), global);
        pthread_mutex_unlock(&mutx);
        sleep(1);
    }
}

int main(){
    int s;
    pthread_t p1, p2;

    s = pthread_create(&p1, NULL, thread_fun, NULL);
    if( s != 0){
        perror("pthread_create...");
    }

    s = pthread_create(&p2, NULL, thread_fun, NULL);
    if( s != 0){
        perror("pthread_create...");
    }

    s = pthread_join(p1, NULL);
    if( s != 0){
        perror("pthread_join...");
    }

    s = pthread_join(p2, NULL);
    if( s != 0){
        perror("pthread_join...");
    }

    return 0;

}