#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <fcntl.h>

#define INTER_PROCESS_MQUEUE_NAME "/interp_mq"

int main(){
    mqd_t ipcmq = mq_open(INTER_PROCESS_MQUEUE_NAME, O_RDONLY);
    if(ipcmq < 0){
        perror("failed to open mq");
        exit(1);
    }
    struct mq_attr attr;
    mq_getattr(ipcmq, &attr);

    char* msg = malloc(attr.mq_msgsize);

    while(mq_receive(ipcmq, msg, attr.mq_msgsize, 0) > 0)
        printf("Message : %s\n", msg);
}