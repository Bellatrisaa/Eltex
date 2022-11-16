#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/udp.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int fd;

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;

    int server_addr_size = sizeof(server_addr);
    char buf[1024] = {};

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    if (fd == -1)
        handle_error("socket");

   int one = 1;
   const int *val = &one;
	
   if (setsockopt (fd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
		handle_error("Error setting IP_HDRINCL");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    server_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");

    char datagram[512];
    memset (datagram, 0, 512);
    char *msg;

    msg = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(msg, "Hi, you!");

    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct iphdr));

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = 0;
    iph->id = 0;
    iph->ttl = 255;
    iph->frag_off = 0;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = 0;
    iph->daddr = server_addr.sin_addr.s_addr;
    
    udph->source = htons(7777);
    udph->dest = htons(10000);
    udph->len = htons(sizeof (struct udphdr) + strlen(msg));
    udph->check = 0;

    sendto(fd, datagram, sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg),
		    0, (struct sockaddr *) &server_addr, server_addr_size);
    while(1){
	int n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_addr_size);
	udph = (struct udphdr *) (buf + sizeof(struct iphdr));

        if (n != -1 && ntohs(udph->dest) == 7777)  break;
    }
    iph = (struct iphdr *) buf;
    printf("destination ip: %s\n", inet_ntoa(*((struct in_addr *)&iph->saddr)));
    printf("destination port: %d\n", ntohs(udph->dest));
    printf("msg: %s\n", buf + 28);
    close(fd);
}
