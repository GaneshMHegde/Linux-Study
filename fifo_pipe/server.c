#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "common.h"


void main(){
    printf("inside main of the server.....\n");

    int serverFd, clientFd, dummyFd;

    char clientFifo[CLIENT_FIFO_NAME_SIZE]; 

    struct request req;
    struct responce res;

    umask(0);

    printf("creating the server fifo.\n");

    if(mkfifo(SERVER_FIFO, S_IWOTH|S_IWGRP|S_IRUSR|S_IWUSR) == -1){
        perror("while creating server fifo.");
        return;
    }

    printf("server fifo fd is opening...\n");
    serverFd = open(SERVER_FIFO, O_RDONLY);

    if(serverFd == -1){
        perror("while opening the server fd.");
        return;
    }

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if(dummyFd == -1){
        perror("while opening the dummy fd for server to be in active.\n");
        return;
    }

    while (1)
    {
        printf("listening to new request...\n");
        if(read(serverFd, &req, sizeof(struct request)) != sizeof(struct request) && errno != 0){
            printf("read request is not valid discarding it \n");
            continue;
        }

        sprintf(clientFifo, CLIENT_FIFO_FORMAT, req.clientPid);
        printf("*********created fifoname is %s\n", clientFifo);

        clientFd = open(clientFifo, O_WRONLY);
        if(clientFd == -1){
            printf("error while opening the client fd for sending responce\n");
            continue;
        }

        res.seqNoOffset = req.seqLen;

        printf("writing the responce to the client...\n");

        if(write(clientFd, &res, sizeof(struct responce)) != sizeof(struct responce) && errno != 0){
            printf("error while writing the responce\n");
            continue;
        }

        close(clientFd);

    }

    close(serverFd);

    if(unlink(SERVER_FIFO) == -1){
        perror("error while closing the server fifo");
    }

    printf("server is terminating end of the main......\n");


}