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
#define MY_MAC "5c:87:9c:f5:b2:7f" //48:2a:e3:58:be:50

#define SERVER_IP "192.168.0.102"
#define SERVER_MAC "50:E5:49:E3:8A:48"

int fd;

int csum(struct iphdr *buf)
{
	int sum = 0;
	short *ptr = (short *)buf;

	for (int i = 0; i < 10; i++) {
		sum = sum + *ptr;
		ptr++;
	}
	int tmp = sum >> 16;
	sum = (sum & 0xFFFF) + tmp;
	sum = ~sum;

	return sum;
}

int main(int argc, char *argv[])
{
    struct sockaddr_ll addr_ll;
    char buf[1024] = {};

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd == -1)
        handle_error("socket");

    memset(&addr_ll, 0, sizeof(struct sockaddr_ll));
                        
    addr_ll.sll_family = htons(AF_PACKET);
    addr_ll.sll_ifindex = htonl(if_nametoindex("wlp2s0"));
    addr_ll.sll_halen = htons(6);
    addr_ll.sll_protocol = htons(ETHERTYPE_IP);

    char datagram[1024];
    memset (datagram, 0, 1024);
    char *msg;

    msg = datagram + sizeof(struct ether_header) +  sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(msg, "Hi, you!");

    struct ether_header *ethh = (struct ether_header *) datagram;
    struct ether_addr *eth_addr;
    struct iphdr *iph = (struct iphdr *)(datagram + sizeof(struct ether_header));
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct ether_header) + sizeof(struct iphdr));

    eth_addr = ether_aton(SERVER_MAC);

    for (int i = 0; i < ETH_ALEN; i++){
	addr_ll.sll_addr[i] = eth_addr->ether_addr_octet[i];
    }

    for (int i = 0; i < ETH_ALEN; i++)
	ethh->ether_dhost[i] = eth_addr->ether_addr_octet[i];

    eth_addr = ether_aton(MY_MAC);

    for (int i = 0; i < ETH_ALEN; i++)
	ethh->ether_shost[i] = eth_addr->ether_addr_octet[i];

    ethh->ether_type = htons(ETHERTYPE_IP);

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr)); 
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

    iph->check = csum(iph);

    while(1){
	int n = sendto(fd, datagram, strlen(datagram), 0, (struct sockaddr *) &addr_ll, sizeof(struct sockaddr_ll));
	if(n < 0) handle_error("sendto");
	sleep(2);
    }
}
