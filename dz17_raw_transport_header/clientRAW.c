#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/udp.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int fd;

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;

    int server_addr_size = sizeof(server_addr);
    char buf[512];

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    if (fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    server_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");

    char datagram[256];
    char *msg;

    msg = datagram + sizeof(struct udphdr);
    strcpy(msg, "Hi, you!");
    struct udphdr *udph = (struct udphdr *)datagram;
    
    udph->source = htons(7777);
    udph->dest = htons(10000);
    udph->len = htons(sizeof (struct udphdr) + strlen(msg));
    udph->check = 0;

    sendto(fd, datagram, sizeof(struct udphdr) + strlen(msg), 0, (struct sockaddr *) &server_addr, server_addr_size);

    while(1){
	int n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_addr_size);
	udph = (struct udphdr *) buf;

        if (n != -1 && ntohs(udph->dest) == 1052)  break; // ==7777
	//Always returns port 1052 if the packet came from the correct server. So I haven't figured out how to fix it.
    }
        printf("dest: %d\n", ntohs(udph->dest));
        printf("msg: %s\n", buf + 28);
	close(fd);
}
