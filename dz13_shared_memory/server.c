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

struct sh_mem *sh_mem_ptr = NULL;

sem_t *sem_clients = NULL, *sem_msg = NULL;

void SignalHandler(int sig)
{
    if (munmap(sh_mem_ptr, sizeof (struct sh_mem)) == -1)
       perror ("munmap");

    printf("Server: Memory unmapped\n");

    if (shm_unlink(PATHNAME) == -1)
        perror("memory unlink");
    
    printf("Server: Memory object is deleted\n");

    if (sem_close(sem_msg) == -1)
        perror("sem close 1");

    if (sem_close(sem_clients) == -1)
        perror("sem close 2");

    if (sem_unlink(SEM_MSGS_NAME) == -1)
        perror("sem unlink 1");

    if (sem_unlink(SEM_CLIENTS_NAME) == -1)
        perror("sem unlink 2");
	
    printf("Server: Semaphores are deleted\n");
    signal(sig, SIG_DFL);
    exit(0);
}

int main(void)
{
    signal(SIGINT, SignalHandler);
    int shmid; 
    int clients_id = 1; 

    if ((shmid = shm_open(PATHNAME, O_RDWR | O_CREAT | O_EXCL, IPC_PARAM)) == -1){
       perror("shm_open");
       SignalHandler(0);
    }

    printf ("Server: Shared memory created.\n");

      if (ftruncate(shmid, sizeof (struct sh_mem)) == -1){
       perror("ftruncate");
       SignalHandler(0);
      }

    if ((sh_mem_ptr = mmap(NULL, sizeof(struct sh_mem), PROT_READ | PROT_WRITE, MAP_SHARED,
       shmid, 0)) == MAP_FAILED){
       perror("mmap");
       SignalHandler(0);
    }

    sh_mem_ptr->name_id = sh_mem_ptr->msg_id = 0;

    printf ("Server: Shared memory initialized.\n");

    if ((sem_clients = sem_open(SEM_CLIENTS_NAME, O_CREAT | O_EXCL, 0660, MAX_CLIENTS)) == SEM_FAILED){
        perror("sem_open 1");
	SignalHandler(0);
    }

    if ((sem_msg = sem_open(SEM_MSGS_NAME, O_CREAT | O_EXCL, 0660, 1)) == SEM_FAILED){
        perror("sem_open 2");
	SignalHandler(0);
    }

    printf ("Server: Semaphores created.\n");

    int last_msg_id = 0;
    while(1){
    	if(sh_mem_ptr->msg_id == last_msg_id) continue;

	last_msg_id = sh_mem_ptr->msg_id;
	 
	if(sh_mem_ptr->name_id == 0){
		int sem_value;
 		sem_getvalue(sem_clients, &sem_value);
		sh_mem_ptr->name_id = clients_id;
		clients_id++;
	}

	printf("(%d)%s: %s\n", sh_mem_ptr->name_id, sh_mem_ptr->name, sh_mem_ptr->msg);
    }

    printf ("Server: Message received.\n");

    SignalHandler(0);

  return 0;
}

