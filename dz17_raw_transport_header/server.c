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

int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = 0;
    char buf[1024] = {};

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(10000);

    if (bind(server_fd, (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    client_addr_size = sizeof(struct sockaddr_in);

    while(1){
    	recvfrom(server_fd, buf, sizeof(buf), 0, 
        	        (struct sockaddr *) &client_addr, &client_addr_size);
    	printf("[MSG]: %s\n", buf);
	printf("%d \n", ntohs(client_addr.sin_port));
	strcat(buf," (this is server message)");
	while(1){
    	sendto(server_fd, buf, sizeof(buf), 0,
         	        (struct sockaddr *) &client_addr, client_addr_size);
	sleep(1);
	}
    }
}
