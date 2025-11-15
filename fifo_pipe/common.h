#include <sys/types.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define CLIENT_FIFO_FORMAT "/tmp/client_%d"
#define CLIENT_FIFO_NAME_SIZE strlen(CLIENT_FIFO_FORMAT) + 20

struct request
{
    /* data */
    int seqLen;
    pid_t clientPid;
};

struct responce
{
    /* data */
    int seqNoOffset;
};

