#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int fd;

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int fd;
    struct sockaddr_in addr;

    socklen_t addr_size = sizeof(addr);
    char buf[256];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&addr, 0, sizeof(struct sockaddr_in));
                        
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    	
    if (inet_aton("255.255.255.255", &addr.sin_addr) == -1)
	handle_error("inet_pton");

    if(bind(fd, (struct sockaddr *)&addr, addr_size) == -1)
	    handle_error("bind");
 
    recvfrom(fd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *) &addr, &addr_size);

    printf("[MSG]: %s\n", buf);
}
