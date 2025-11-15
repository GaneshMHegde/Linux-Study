#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define INTER_PROCESS_MQUEUE_NAME "/interp_mq"

int main(){
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = 200,
        .mq_curmsgs = 0
    };
    mq_unlink(INTER_PROCESS_MQUEUE_NAME);
    mqd_t ipcmq = mq_open(INTER_PROCESS_MQUEUE_NAME, O_CREAT|O_RDWR, 0666, &attr);

    for(int i = 0; i < 5; i++){
        char* msg = malloc((strlen("hello ") + 2)*sizeof(char));
        memset(msg, 0, (strlen("hello ") + 2));
        sprintf(msg, "hello %d", i);
        mq_send(ipcmq, msg, (strlen("hello ") + 2), 0);
    }

}