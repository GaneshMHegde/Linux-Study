#include <stdio.h>
#include <pthread.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int value;
    char* msg;
}return_value_t;

void* thread_func(void* arg){
    
    return_value_t* return_value = (return_value_t*)malloc(sizeof(return_value_t));

    return_value->value = 100;
    return_value->msg = (char*)malloc((strlen("hello fro thread function") + 1)*sizeof(char));
    strcpy(return_value->msg, "hello fro thread function");

    pthread_exit((void*)return_value);

}

int main(){
    pthread_t tid;

    pthread_create(&tid, NULL, thread_func, NULL);

    return_value_t* ret_val;

    pthread_join(tid, (void**)&ret_val);

    printf("%d %s", ret_val->value, ret_val->msg);
}