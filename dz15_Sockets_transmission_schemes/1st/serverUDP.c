#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void ConnectionWork(struct sockaddr_in addr, int addr_size, int ClientNum) {
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1)
        	handle_error("socket");

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = 0;

	if (bind(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1)
       		 handle_error("bind");

	socklen_t addrlen = sizeof(server_addr);

	getsockname(fd, (struct sockaddr*)&server_addr, &addrlen);

        char msg[256];
        strcpy(msg, "Hi, I am your server!");
        char char_number[10];
        sprintf(char_number, "%d", ClientNum);

	sendto(fd, msg, sizeof(msg), MSG_CONFIRM,
              (struct sockaddr *) &addr, addr_size);

        while (msg[0] != '\0') {
		strcpy(msg, "");
                recvfrom(fd, msg, sizeof(msg), MSG_WAITALL,
                	(struct sockaddr *) &addr, &addr_size);
                if(msg[0] == '\0') break;
                strcat(msg, " | This is your message. Your client number is ");
                strcat(msg, char_number);
		sendto(fd, msg, sizeof(msg), MSG_CONFIRM,
                        (struct sockaddr *) &addr, addr_size);

	}
	close(fd);
}


int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = 0;
    char buf[256] = {};
    int ClientsCount = 1;
    client_addr_size = sizeof(struct sockaddr_in);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(10000);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1)
   	 handle_error("bind");

    while(1){
    	if(recvfrom(server_fd, buf, sizeof(buf), MSG_WAITALL, 
                (struct sockaddr *) &client_addr, &client_addr_size) <= 0)
		handle_error("recvfrom");

   	 printf("Client connected! [MSG]: %s\n", buf);
	 if(!fork()){
		ConnectionWork(client_addr, client_addr_size, ClientsCount);
		close(server_fd);
		exit(0);
	 }
	 else{
		 ClientsCount++;
	 }
    }
}
