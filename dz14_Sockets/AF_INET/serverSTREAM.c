#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define LISTEN_BACKLOG 5

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size;
    char buf[256];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
                        
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(10000);

    if (bind(server_fd, (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen");
    printf("listen\n");

    client_addr_size = sizeof(struct sockaddr_in);

    client_fd = accept(server_fd, (struct sockaddr *) &client_addr,  &client_addr_size);
    if (client_fd == -1)
        handle_error("accept");
   
        recv(client_fd, buf, sizeof(buf), 0);
        printf("[MSG]: %s\n", buf);
        send(client_fd, buf, sizeof(buf), 0);
}
