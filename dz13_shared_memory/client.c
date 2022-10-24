#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

#define PATHNAME "/pathSHM"
#define MAX_MSG_SIZE 256
#define MAX_NAME_SIZE 16
#define MAX_CLIENTS 5
#define IPC_PARAM 0660
#define SEM_CLIENTS_NAME   "/sem-clients"
#define SEM_MSGS_NAME     "/sem-msgs"

struct sh_mem {
    char msg [MAX_MSG_SIZE];
    char name [MAX_NAME_SIZE];
    int name_id;
    int msg_id;
};

struct sh_mem *sh_mem_ptr;
sem_t *sem_clients, *sem_msg;

void SignalHandler(int sig)
{
    if (munmap(sh_mem_ptr, sizeof (struct sh_mem)) == -1)
       perror ("munmap");

    printf("Client: Memory unmapped\n");
    
    if (sem_close(sem_msg) == -1)
        perror("sem unlink");

    if (sem_post(sem_clients) == -1)
        perror ("sem_post: buf_count_sem");

    if (sem_close(sem_clients) == -1)
        perror("sem unlink");
   
    printf("Client: semaphores are closed\n");
    signal(sig, SIG_DFL);
    exit(0);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, SignalHandler);
    int shmid; 
    int myname_id = 0;
    char myname[MAX_NAME_SIZE];
    strcpy(myname, argv[1]);
    
    if ((shmid = shm_open(PATHNAME, O_RDWR, 0)) == -1){
       perror("shm_open");
      SignalHandler(0);
    }
 
    if ((sh_mem_ptr = mmap(NULL, sizeof(struct sh_mem), PROT_READ | PROT_WRITE, MAP_SHARED,
            shmid, 0)) == MAP_FAILED){
       perror("mmap");
       SignalHandler(0);
    }

    if ((sem_clients = sem_open(SEM_CLIENTS_NAME, 0, 0, 0)) == SEM_FAILED){
        perror("sem_open 1");
	SignalHandler(0);
    }

    if (sem_wait(sem_clients) == -1){
        perror ("sem_post: buf_count_sem");
        SignalHandler(0);
    }

    if ((sem_msg = sem_open(SEM_MSGS_NAME, 0, 0, 0)) == SEM_FAILED){
        perror("sem_open 2");
	SignalHandler(0);
    }

    char message[MAX_MSG_SIZE];

    while(1){
	printf("Write a massage: ");

    	if(fgets(message, MAX_MSG_SIZE - 2, stdin)){
		 if (sem_wait(sem_msg) == -1){
                	perror ("sem_post: buf_count_sem");
               		SignalHandler(0);
         	}

   		strcpy(sh_mem_ptr->name,myname);
		strcpy(sh_mem_ptr->msg,message);
		sh_mem_ptr->name_id = myname_id;
		sh_mem_ptr->msg_id++;
	
		printf ("Message sended.\n");
    		
		while(!myname_id) 
			myname_id = sh_mem_ptr->name_id;

		 if (sem_post(sem_msg) == -1){
                 	perror ("sem_post: buf_count_sem");
                 	SignalHandler(0);
        	 }
    	}
	else	SignalHandler(0);
    }

  return 0;
}

