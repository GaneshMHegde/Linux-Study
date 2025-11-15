#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_FD_NAME "/home/ganesh/Documents/LinusSystemStudy/socket/socket_unix_server"
#define SOCKET_FD_NAME_LEN strlen(SOCKET_FD_NAME)
#define SOCKET_MSG_BUF_LRN 50

void printUsage(char* binaryName){
    printf("Usage %s -m msg[-h]\n", binaryName);
    printf("    -m msg  To pass massage to server\n");
    printf("    -h      To get help\n");
}

int main(int argc, char* argv[]){
    char opt;
    int ret, written;

    char* msg = NULL;

    int svrfd;
    struct sockaddr_un svrsockaddr;

    while((opt = getopt(argc, argv, "-hm:")) != -1){
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'm':
            msg = (char*)malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(msg, optarg);
            msg[strlen(optarg)] = '\0';
            break;

        default:
            printf("Invalid arg input !!\n");
            exit(EXIT_FAILURE);
        }
    }

    if(msg == NULL){
        printf("msg arg is missing!\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    svrfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(svrfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&svrsockaddr, 0, sizeof(struct sockaddr_un));
    svrsockaddr.sun_family = AF_UNIX;
    strcpy(svrsockaddr.sun_path, SOCKET_FD_NAME);

    ret = connect(svrfd, (struct sockaddr*)&svrsockaddr, sizeof(struct sockaddr_un));

    if(ret != 0){
        perror("connect");
        exit(EXIT_SUCCESS);
    }

    if(write(svrfd, msg, strlen(msg)) != strlen(msg)){
        perror("message send failed.");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;

}