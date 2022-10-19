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
    key_t server_queue_key;        //server queue key
    int server_qid;                    //message queue id
    struct message message;
    message.type = 1;
    strcpy(message.text, "Hi, Queue!");

    if ((server_queue_key = ftok (PATHNAME, PROJ_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (server_queue_key, IPC_CREAT | IPC_PARAM)) == -1) {
        perror ("msgget: server_qid");
        exit (1);
    }

    printf ("Server: Queue created.\n");

    if (msgsnd (server_qid, &message, sizeof (message.text), 0) == -1) {
        perror ("msgsnd");
        exit (1);
    } 

    printf ("Server: message sended.\n");
    printf("Message: %s\n", message.text);
  
  return 0;
}

