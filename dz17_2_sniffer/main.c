#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

int fd;

int main(void)
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof addr;
 
    struct iphdr *iph;
    char buf[512];

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int n = recvfrom(fd, buf, sizeof buf, 0,
                    (struct sockaddr *)&addr, &addr_size);

        if (n == -1) {
            perror("recvfrom");
            continue;
        }

        iph = (struct iphdr *)buf;

        printf("source addr: %s\n", inet_ntoa(*((struct in_addr *)&iph->saddr)));
        printf("destination addr: %s\n\n", inet_ntoa(*((struct in_addr *)&iph->daddr)));
    }

    close(fd);
    return 0;
}
