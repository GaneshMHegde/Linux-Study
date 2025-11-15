#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "common.h"


void client(int no){
    //=======================================
    // data members
    pid_t pid = getpid();
    struct request req;
    struct responce res;

    int serverFd, clientFd;

    char clientFifoName[CLIENT_FIFO_NAME_SIZE];

    //==========================================
    // main logic of client
    sprintf(clientFifoName, CLIENT_FIFO_FORMAT, pid);
    printf("*********created fifoname is %s\n", clientFifoName);
    umask(0);
    if(mkfifo(clientFifoName, S_IWOTH|S_IWGRP|S_IRUSR|S_IWUSR)  == -1){
        perror("while creating the client fifo.\n");
        exit(errno);
    }

    req.clientPid = pid;
    req.seqLen = no;

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1){
        perror("while opening the server flle discripter\n");
        exit(errno);
    }

    if(write(serverFd, &req, sizeof(struct request)) != sizeof(struct request) && errno != 0){
        perror("error while sending the request to server\n");
        exit(-1);
    }

    clientFd = open(clientFifoName, O_RDONLY);
    if(clientFd == -1){
        perror("while opening the client fifo for readinfg the responce\n");
        exit(-1);
    }

    if(read(clientFd, &res, sizeof(struct responce)) != sizeof(struct responce) && errno != 0){
        perror("while while reading the responce from the server");
        exit(-1);
    }

    printf("==============================\n");
    printf("responce %d \n", res.seqNoOffset);
    printf("================================\n");
    close(serverFd);
    close(clientFd);
    unlink(clientFifoName);
    return;
}


void main(int argc, char* argv[]){
    printf("creating the clients....\n");

    for(int i = 1; i < argc; i++){
        switch(fork()){
            case -1:
                perror("while creating the client process. ");
                return;
            case 0:
                client(atoi(argv[i]));
                exit(0);
            default:
                printf("client %d is created...\n", i);
                continue;
        }
    }
}