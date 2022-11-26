#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ether.h>	
#include <net/if.h>
#include <linux/if_packet.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define MY_IP "192.168.0.107"
#define MY_MAC "5c:87:9c:f5:b2:7f"

#define SERVER_IP "192.168.0.107"
#define SERVER_MAC "5c:87:9c:f5:b2:7f"

int fd;

int csum(struct iphdr *buf)
{
	int sum = 0;
	short *ptr = (short *)buf;

	for (int i = 0; i < 10; i++) {
		sum = sum + ptr[i];
	}
	int tmp = sum >> 16;
	sum = (sum & 0xFFFF) + tmp;
	sum = ~sum;

	return sum;
}

int main(int argc, char *argv[])
{
    struct sockaddr_ll addr_ll;
    socklen_t addrlen = sizeof addr_ll;
    char buf[1024] = {};

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));

    if (fd == -1)
        handle_error("socket");

    memset(&addr_ll, 0, sizeof(struct sockaddr_ll));
                        
    addr_ll.sll_family = htons(AF_PACKET);
    addr_ll.sll_ifindex = if_nametoindex("wlp2s0");
    addr_ll.sll_halen = 6;
    addr_ll.sll_protocol = htons(ETH_P_IP);

    char datagram[1024];
    memset (datagram, 0, 1024);
    char *msg;

    msg = datagram + sizeof(struct ethhdr) +  sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(msg, "Hi, you!");

    struct ethhdr *ethh = (struct ethhdr *) datagram;
    struct iphdr *iph = (struct iphdr *)(datagram + sizeof(struct ethhdr));
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct ethhdr) + sizeof(struct iphdr));

    memset (ethh, 0, sizeof(struct ethhdr));
    memset (iph, 0, sizeof(struct iphdr));
    memset (udph, 0, sizeof(struct udphdr));

    memcpy(ethh->h_source, ether_aton(MY_MAC), 6);
    memcpy(ethh->h_dest, ether_aton(SERVER_MAC), 6);
    ethh->h_proto = htons(ETH_P_IP);

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg)); 
    iph->id = htons(5555);
    iph->ttl = 255;
    iph->frag_off = 0;
    iph->protocol = IPPROTO_UDP;
    iph->saddr = inet_addr(MY_IP);		
    iph->daddr = inet_addr(SERVER_IP);
    
    udph->source = htons(7777);
    udph->dest = htons(10000);
    udph->len = htons(sizeof (struct udphdr) + strlen(msg));
    udph->check = 0;

    memcpy(addr_ll.sll_addr, ethh->h_dest, 6);

    iph->check = csum(iph);
    /*
	printf("Address:\n");
	printf("addr_ll.sll_family: %X\n", addr_ll.sll_family);
	printf("addr_ll.sll_ifindex: %X\n", addr_ll.sll_ifindex);
	printf("addr_ll.sll_halen: %X\n", addr_ll.sll_halen);
	printf("addr_ll.sll_protocol: %X\n", addr_ll.sll_protocol);
	printf("addr_ll.sll_addr: ");
	for (int i = 0; i < ETH_ALEN; i++)
		printf("%X", addr_ll.sll_addr[i]);
	printf("\n");
    */

    int packet_size = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg);

    while(1){
	int n = sendto(fd, datagram, packet_size, 0, (struct sockaddr *) &addr_ll,  addrlen);
	if(n < 0) handle_error("sendto");
	sleep(2);
    }
}
