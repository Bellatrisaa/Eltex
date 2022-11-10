#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

int fd;

#define handle_error(msg) \
    do { perror(msg); close(fd); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    char buf[256];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == -1)
	handle_error("socket");

    connect(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in));

    for(int i = 1; i < 11; i++){
    	recv(fd, buf, sizeof(buf), 0);
    	printf("server: [MSG]: %s\n", buf);
	strcpy(buf, "My message number ");
	char char_i[2];
	sprintf(char_i, "%d", i);
	strcat(buf, char_i);
   	send(fd, buf, sizeof(buf), 0);
	sleep(1);
    } 
    send(fd, "\0",sizeof("\0") , 0);
    close(fd);
    exit(0);
}
