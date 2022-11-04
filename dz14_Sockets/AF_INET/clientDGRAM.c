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
    int fd;
    struct sockaddr_in server_addr;

    int server_addr_size = 0;
    char buf[256] = {};

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    	
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == -1)
	handle_error("inet_pton");

    server_addr_size = sizeof(server_addr);

    strcpy(buf, "HI YOU!");

    sendto(fd, buf, sizeof(buf), MSG_CONFIRM, (struct sockaddr *) &server_addr, server_addr_size);

    recvfrom(fd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *) &server_addr, &server_addr_size);

    printf("[MSG]: %s\n", buf);
}
