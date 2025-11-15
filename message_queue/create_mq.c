#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>

#define MQ_DEFALT_NAME "/mymsgq"
#define MAX_NO_MSG 15
#define MAX_MSG_SIZE 2000

void printUsage(char* progName){
    printf("Usage: %s [-h] -f filename \n", progName);
    printf("    -h           To print help\n");
    printf("    -f filename  To Specify the name of the message queue discripter\n");
}

void main(int argc, char* argv[]){

    char opt;
    char* mqfd = NULL;

    mqd_t fd;

    struct mq_attr mqattr;

    while ((opt = getopt(argc, argv, "-hf:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);

        case 'f':
            mqfd = (char*)malloc((strlen(optarg) + 1) *sizeof(char));
            strcpy(mqfd, optarg);
            mqfd[strlen(optarg)] = '\0';
            break;
        case '?':
            printf("Unknown option: -%c\n", optopt);
            exit(EXIT_FAILURE);
        
        default:
            printf("ERROR: Unknown argument passed!! \n");
            exit(EXIT_FAILURE);
        }
    }

    if(mqfd == NULL){
        mqfd = (char*)malloc((strlen(MQ_DEFALT_NAME) + 1) * sizeof(char)); 
        strcpy(mqfd, MQ_DEFALT_NAME);
        mqfd[strlen(MQ_DEFALT_NAME)] = '\0';
    }

    memset(&mqattr, 0, sizeof(mqattr));
    mqattr.mq_flags = 0;
    mqattr.mq_maxmsg = MAX_NO_MSG;
    mqattr.mq_msgsize = MAX_MSG_SIZE;

    printf("creating the posix message queue\n");

    umask(0);

    fd = mq_open(mqfd, O_RDWR|O_CREAT|O_EXCL, S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR, &mqattr);

    if(fd == -1){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("message queue is created and exitting...\n");

    exit(EXIT_SUCCESS);
    
}