#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>

void printUsage(char *binaryName){
    printf("Usage: %s -f semfd [-h]\n", binaryName);
    printf("    -h         To show help\n");
    printf("    -f semfd   To specify the semaphore to read on. \n");
}

int main(int argc, char* argv[]){
    char opt;
    int ret, currval;

    char* semfdname = NULL;

    sem_t* semfd = NULL;

    while((opt = getopt(argc, argv, "-hf:")) != -1){
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);

        case 'f':
            semfdname = (char*)malloc((strlen(optarg)+1)* sizeof(char));
            strcpy(semfdname, optarg);
            semfdname[strlen(optarg)] = '\0';
            break;
        default:
            printf("Invalid arg passed !! \n");
            exit(EXIT_FAILURE);
        }
    }

    if(semfdname == NULL){
        printf("Invalid parameter passed.\n");
        exit(EXIT_FAILURE);
    }

    semfd = sem_open(semfdname, 0);

    if(semfd == (sem_t*)-1){
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    ret = sem_wait(semfd);

    if(ret != 0){
        perror("sem_post");
        exit(EXIT_FAILURE);
    }

    printf("semaphore is read from %s \n", semfdname);

    return EXIT_SUCCESS; 

}