#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <fcntl.h>

#define PATHNAME "/pathPOSIX"
#define MSG_SIZE 20
#define IPC_PARAM 0660

int main(void)
{
    struct mq_attr attr;
    char message[MSG_SIZE];
    int server_qid;                    //message queue id

    strcpy(message, "Hi, POSIX QUEUE!");

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; // MAX_MESSAGES
    attr.mq_msgsize = 256; // MAX_MSG_SIZE
    attr.mq_curmsgs = 0;
 
    if ((server_qid = mq_open (PATHNAME, O_WRONLY)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }

     printf ("Client: Queue created.\n");

    if (mq_send (server_qid, message, strlen (message) + 1, 0) == -1) {
        perror ("Client: Not able to send message to server");
        exit(1);
    }

    printf ("Message sended.\n");
    printf("Message: %s\n", message);

    if (mq_close (server_qid) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }

    printf ("Client: Queue is closed\n");

    if (mq_unlink (PATHNAME) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: Queue is removed\n");
     
  return 0;
}

