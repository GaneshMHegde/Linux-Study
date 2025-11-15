#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>


void printUsage(char* binaryName){
    printf("Usage: %s -f memfd [-h]\n", binaryName);
    printf("    -f memfd  To specify the fd for shared memory\n");
    printf("    -h        To get help\n");
}

int main(int argc, char* argv[]){
    char opt;
    char* memfdname = NULL;

    char* addr = NULL;

    int ret;

    int shmfd;

    struct stat statbuff;

    while((opt = getopt(argc, argv, "-hf:")) != -1) {
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);

        case 'f':
            memfdname = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(memfdname, optarg);
            memfdname[strlen(optarg)];
            break;
        
        default:
            printf("Invalid arg passed!\n");
            exit(EXIT_FAILURE);
        }
    }

    if(memfdname == NULL){
        printf("Missing impt arg !!\n");
        exit(EXIT_FAILURE);
    }

    shmfd = shm_open(memfdname, O_RDWR, 0666);

    if(shmfd == -1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    ret = fstat(shmfd, &statbuff);

    if(ret != 0){
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    addr = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    printf("mesage read: %s \n", addr);
    return EXIT_SUCCESS;
}