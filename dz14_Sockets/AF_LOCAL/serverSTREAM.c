#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MY_SOCK_PATH "/tmp/socket_server"
#define LISTEN_BACKLOG 5

#define handle_error(msg) \
    do { perror(msg); unlink(MY_SOCK_PATH); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_size;
    char buf[256];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
                        
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, MY_SOCK_PATH,
            sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr_un)) == -1)
        handle_error("bind");

    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen");
    printf("listen\n");

    client_addr_size = sizeof(struct sockaddr_un);

    client_fd = accept(server_fd, (struct sockaddr *) &client_addr,
                 &client_addr_size);
    if (client_fd == -1)
        handle_error("accept");
   
//while{
        recv(client_fd, buf, sizeof(buf), 0);
        printf("[MSG]: %s\n", buf);
        send(client_fd, buf, sizeof(buf), 0);
//} 

    unlink(MY_SOCK_PATH);
}
