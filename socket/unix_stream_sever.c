#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_FD_NAME "/home/ganesh/Documents/LinusSystemStudy/socket/socket_unix_server"
#define SOCKET_FD_NAME_LEN strlen(SOCKET_FD_NAME)
#define SOCKET_BACKLOG 5
#define SOCKET_MSG_BUF_LRN 50

void printUsage(char* binaryName){
    printf("Usage: %s [-h]\n", binaryName);
    printf("    -h   To get help\n");
}

int main(int argc, char* argv[]){
    char opt;
    int ret = 0;

    char buffer[SOCKET_MSG_BUF_LRN];

    int socfd, clientfd;

    struct sockaddr_un server_adder;

    while((opt = getopt(argc, argv, "-h")) != -1){
        switch (opt)
        {
        case 'h':
            printUsage(argv[0]);
            exit(EXIT_SUCCESS);
        
        default:
            printf("Invalid input arg !!\n");
            exit(EXIT_FAILURE);
        }
    }

    ret = remove(SOCKET_FD_NAME);

    if(ret != 0 && errno != ENOENT){
        int elen = (strlen("remove while [%s]\n") + SOCKET_FD_NAME_LEN + 1);
        char* emsg = (char*)malloc(elen * sizeof(char));
        memset(emsg, 0, elen);
        sprintf(emsg, "remove while [%s]\n",  (char*)SOCKET_FD_NAME);
        perror(emsg);
        exit(EXIT_FAILURE);
    }

    socfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(socfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_adder, 0, sizeof(struct sockaddr_un));
    server_adder.sun_family = AF_UNIX;
    strcpy(server_adder.sun_path, SOCKET_FD_NAME);

    ret = bind(socfd, (struct sockaddr*)&server_adder, sizeof(struct sockaddr_un));

    if( ret != 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(socfd, SOCKET_BACKLOG);

    if(ret != 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("\n");
        clientfd = accept(socfd, NULL, NULL);

        if(clientfd == -1){
            perror("accept");
            continue;
        }

        while ((ret = read(clientfd, buffer, SOCKET_MSG_BUF_LRN)) > 0)
        {
            if(ret == -1){
                perror("read");
                if(close(clientfd) != 0){
                perror("close");
                exit(EXIT_FAILURE);
                }
                continue;
            }

            if(write(STDOUT_FILENO, buffer, ret) != ret){
                perror("write");
                continue;
            }
        }

        if(close(clientfd) != 0){
            perror("close");
            exit(EXIT_FAILURE);
        }
        
    }

    return EXIT_SUCCESS;
}

