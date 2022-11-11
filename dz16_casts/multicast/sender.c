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
    do { perror(msg); close(fd); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    struct ip_mreqn req;
    socklen_t addr_size = sizeof(addr);
    char buf[256] = {};

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&addr, 0, sizeof(struct sockaddr_in));
    memset(&req, 0, sizeof(struct ip_mreqn));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);

    if (inet_aton("224.0.0.0", &addr.sin_addr) == -1)
        handle_error("inet_aton addr");

    req.imr_address.s_addr = htonl(INADDR_ANY);
    req.imr_ifindex = 0;

    if (inet_aton("224.0.0.0", &req.imr_multiaddr) == -1)
        handle_error("inet_aton addr");

    if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &req, sizeof(req)) == -1)
	handle_error("setsockopt");

    strcpy(buf, "This is multicast msg");

    sendto(fd, buf, sizeof(buf), MSG_CONFIRM,
                (struct sockaddr *) &addr, addr_size);
}
