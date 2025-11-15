#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_MESSAGE_QUEUE_NAME "/tmsgq"

struct mq_attr tmq_attr = {
    .mq_flags = 0,
    .mq_msgsize = 50 * sizeof(char),
    .mq_maxmsg = 5,
    .mq_curmsgs = 0
};
mqd_t tmq;

void* consumer(void* arg){
    struct mq_attr attr;
    mq_getattr(tmq, &attr);
    char* msg = malloc(attr.mq_msgsize);

    while(mq_receive(tmq, msg, attr.mq_msgsize, 0) > 0)
        printf("Consumer %s \n", msg);

}

void* producer(void*){
    for(int i = 0; i < 10; i++){
        char* msg = malloc((strlen("hello ") + 2) * sizeof(char));
        memset(msg, 0, (strlen("hello ") + 2));
        sprintf(msg, "hello %d", i);
        printf("Producer: %s\n", msg);
        mq_send(tmq, msg, strlen(msg),0);
    }

}


int main(){
    pthread_t consumer_tid, producer_tid;

    mq_unlink(THREAD_MESSAGE_QUEUE_NAME);
    tmq = mq_open(THREAD_MESSAGE_QUEUE_NAME, O_CREAT | O_RDWR, 0666, &tmq_attr);

    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    pthread_detach(producer_tid);
    pthread_detach(consumer_tid);

    pthread_exit(NULL);
}