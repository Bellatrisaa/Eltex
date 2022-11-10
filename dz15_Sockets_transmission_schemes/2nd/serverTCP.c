#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>

#define LISTEN_BACKLOG 5
#define NUMBER_OF_SUBSERVERS 5

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int data_pipes[NUMBER_OF_SUBSERVERS][2];

int server_fd, client_fd;

pthread_mutex_t mutex;

bool IsClientWait = false;

int NewConnection(int fd, struct sockaddr_in* addr, socklen_t addrlen)
{
        int newConnection;

            if (listen(fd, LISTEN_BACKLOG) == -1)
        handle_error("listen");

        newConnection = accept(fd, (struct sockaddr*)addr, &addrlen);

        if (newConnection == -1) {
             handle_error("accept");
        }
        else {
                printf("Client Connected!\n");
                return newConnection;
        }
	
}

void ConnectionWork(int client_fd, int ClientNum)
{
        char msg[256];
       	strcpy(msg, "Hi, I am your server!");
	char char_number[10];
	sprintf(char_number, "%d", ClientNum);
        while (1) {
        	send(client_fd, msg, sizeof(msg), 0);
        	recv(client_fd, msg, sizeof(msg), 0);
		if(!msg) break;
		strcat(msg, " | This is your message. Your client number is ");
		strcat(msg, char_number);
	}
}

void *SubserverWork(void* i)
{
     int* ptrClientNum = (int*)i;
     int ClientNum = *ptrClientNum;
     while(1){
	pthread_mutex_lock(&mutex);
	if(IsClientWait){ 
		IsClientWait = false;
		pthread_mutex_unlock(&mutex);
		ConnectionWork(client_fd, ClientNum);
	}
	pthread_mutex_unlock(&mutex);
     }
}


int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size;
    bool need_to_create_processes = true;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(10000);

    if (bind(server_fd, (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    client_addr_size = sizeof(struct sockaddr_in);

    while(1){
      if(need_to_create_processes)
       	for(int i = 0; i < NUMBER_OF_SUBSERVERS; i++){
		pthread_t thread = pthread_create(&thread, NULL, SubserverWork, &i);
       	}
      need_to_create_processes = false;
      client_fd = NewConnection(server_fd, &server_addr, client_addr_size);
      IsClientWait = true;
   }
}   
