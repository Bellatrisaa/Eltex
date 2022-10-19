#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <fcntl.h>

#define PATHNAME "/pathPOSIX"
#define MAX_MSG_SIZE 64 
#define IPC_PARAM 0660

int main(void)
{
    struct mq_attr attr;
    char message[MAX_MSG_SIZE];
    int myqid;                    //message queue id

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; // MAX_MESSAGES
    attr.mq_msgsize =  MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((myqid = mq_open (PATHNAME, O_RDONLY | O_CREAT, IPC_PARAM, &attr)) == -1) {
        perror ("Server: mq_open");
        exit (1);
    }

    printf ("Server: Queue created.\n");
 
    if (mq_receive (myqid, message, MAX_MSG_SIZE, NULL) == -1) {
        perror ("Server: mq_receive");
        exit (1);
    }

    printf ("Message received.\n");
    printf("Received message: %s\n", message);

    if (mq_close (myqid) == -1) {
        perror ("Server: mq_close");
        exit (1);
    }

    printf ("Server: Queue is closed\n");

    if (mq_unlink (PATHNAME) == -1) {
        perror ("Server: mq_unlink");
        exit (1);
    }
    printf ("Server: Queue is removed\n");
     
  return 0;
}

