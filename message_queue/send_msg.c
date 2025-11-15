#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <string.h>
#include <sys/stat.h>

void printUsage(char* binaryName){
    printf("Usage: %s -f mqfd -m message [-h]\n", binaryName);
    printf("    -f mqfd message queue file discripter name\n");
    printf("    -m msg  message to be sent \n");
    printf("    -h      prints help \n");
}

int main(int argc, char* argv[]){
    char opt;
    char* mqfdname = NULL;
    char* msg = NULL;

    int ret = 0;

    mqd_t mqfd;
    struct mq_attr *mqattr;

    while((opt = getopt(argc, argv, "-hf:m:")) != -1){
        switch (opt)
        {
        case 'f':
            mqfdname = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(mqfdname, optarg);
            mqfdname[strlen(optarg)] = '\0';
            break;

        case 'm':
            msg = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(msg, optarg);
            msg[strlen(optarg)] = '\0';
            break;

        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);
        
        default:
            printf("Error: Invalid argument...\n");
            exit(EXIT_FAILURE);
        }
    }

    if(mqfdname == NULL || msg == NULL){
        printf("Error: please provide propper mqfd and msg\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    mqfd = mq_open(mqfdname, O_WRONLY);
    if(mqfd == (mqd_t)-1){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mqattr = (struct mq_attr*)malloc(sizeof(struct mq_attr));
    memset(mqattr, 0, sizeof(struct mq_attr));

    ret = mq_getattr(mqfd, mqattr);

    if(ret == -1){
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    ret = mq_send(mqfd, msg, mqattr->mq_msgsize, 0);

    if(ret == -1){
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Message %s sent success fully to %s fd.\n", msg, mqfdname);

    exit(EXIT_SUCCESS);

}