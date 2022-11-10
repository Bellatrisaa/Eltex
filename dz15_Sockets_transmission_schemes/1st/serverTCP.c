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

#define LISTEN_BACKLOG 5

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int NewConnection(int fd, struct sockaddr_in* addr, socklen_t addrlen){
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

void ConnectionWork(int client_fd, struct sockaddr_in addr, int ClientNum) {
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

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size;
    int ClientsCount = 1;

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
        client_fd = NewConnection(server_fd, &server_addr, client_addr_size);
        if(!fork()){
            ConnectionWork(client_fd, server_addr, ClientsCount);
            close(client_fd);
            close(server_fd);
            exit(0);
        }
        else{
	    ClientsCount++;
	    close(client_fd);
        }
    }
}    
