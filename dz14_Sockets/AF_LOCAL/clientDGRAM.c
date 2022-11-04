#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_SOCK_PATH "/tmp/socket_server"
#define MY_SOCK_PATH "/tmp/socket_client"
#define LISTEN_BACKLOG 5

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int fd;
    struct sockaddr_un server_addr, client_addr;

    int server_addr_size = 0;
    char buf[256] = {};

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
                        
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, MY_SOCK_PATH,
            sizeof(server_addr.sun_path) - 1);

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCK_PATH,
            sizeof(server_addr.sun_path) - 1);
	
    if (bind(fd, (struct sockaddr *) &client_addr,
		sizeof(struct sockaddr_un)) == -1)
	handle_error("bind");

    server_addr_size = sizeof(server_addr);

    connect(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un));

    send(fd, "hi", 2, 0);

    recv(fd, buf, sizeof(buf), 0);

    printf("[MSG]: %s\n", buf);

	unlink(MY_SOCK_PATH);
    
}
