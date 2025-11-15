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
    printf("Usage: %s -c semname [-h]\n", binaryName);
    printf("    -h         To show help\n");
    printf("    -c semname To specify the semaphore name\n");
}

int main(int argc, char* argv[]){
    char opt;
    char* semfdname = NULL;

    sem_t* semfd;

    int ret, currval;

    while((opt = getopt(argc, argv, "-hc:")) != -1){
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);

        case 'c':
            semfdname  = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(semfdname, optarg);
            semfdname[strlen(optarg)] = '\0';
            break;

        default:
            printf("Invalid argument passed.!\n");
        }
    }

    if(semfdname == NULL){
        printf("Required arg semname is not passed.\n");
        exit(EXIT_FAILURE);
    }

    semfd = sem_open(semfdname, O_CREAT | O_EXCL, 0666, 0);

    if(semfd == (sem_t*)-1){
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("%s semaphore is created. \n", semfdname);

    ret = sem_getvalue(semfd, &currval);
    if(ret != 0){
        perror("sem_getval\n");
        exit(EXIT_FAILURE);
    }

    printf("%s holds %d val\n", semfdname, currval);

    return EXIT_SUCCESS;

}