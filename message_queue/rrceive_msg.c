#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

void printUsage(char* binaryNanme){
    printf("Usage: %s -f mqfd [-p prio] [-h] \n", binaryNanme);
    printf("    -f mqfd   To specify the message queue fd name.\n");
    printf("    -p prio   To specify the priority\n");
    printf("    -h        To get help\n");
}

int main(int argc, char* argv[]){
    char opt;
    char* mqfdname = NULL;
    int prio = 0;
    int ret = 0;
    ssize_t msgred;
    char* buffer;

    mqd_t mqfd;
    struct mq_attr mqattr;

    while((opt = getopt(argc, argv, "-hf:p:")) != -1){
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'f':
            mqfdname = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(mqfdname, optarg);
            mqfdname[strlen(optarg)] = '\0';
            break;
        case 'p':
            prio = atoi(optarg);
            break;

        default:
            printf("Error: Invalid args...!\n");
            printUsage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if(mqfdname == NULL){
        printf("Error: Invalid args...!\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    mqfd = mq_open(mqfdname, O_RDONLY);

    if(mqfd == (mqd_t)-1){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    ret = mq_getattr(mqfd, &mqattr);

    if(ret == -1){
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    buffer = (char*)malloc(mqattr.mq_msgsize);
    memset(buffer,0, mqattr.mq_msgsize);

    msgred = mq_receive(mqfd, buffer, mqattr.mq_msgsize, &prio);

    if(msgred == -1){
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }

    printf("%lu bytes read from mq %s\n", (long)msgred, mqfdname);
    printf("    read message: %s", buffer);

    exit(EXIT_SUCCESS);

}

