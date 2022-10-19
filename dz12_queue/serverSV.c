#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATHNAME "./path.txt"
#define PROJ_ID 'A'
#define IPC_PARAM 0660

struct message {
    long type;
    char text[15];    
};

int main(void)
{
    key_t queue_key;        //server queue key
    int myqid;                    //message queue id
    struct message message;

    if ((queue_key = ftok (PATHNAME, PROJ_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }
    
    if ((myqid = msgget (queue_key, IPC_CREAT | IPC_PARAM)) == -1) {
        perror ("msgget");
        exit (1);
    }

    printf ("Server: Queue created.\n");
    
 
   if (msgrcv (myqid, &message, sizeof (message.text), 1, 0) == -1) {
        perror ("msgrcv");
        exit (1);
    }

    printf ("Message received.\n");
    printf("Received message: %s\n", message.text);

    if (msgctl (myqid, IPC_RMID, NULL) == -1) {
        perror ("server: msgctl");
        exit (1);
    }
     
  return 0;
}

