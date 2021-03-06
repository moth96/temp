#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "wrap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999

int main(void)
{
    int connfd;
    struct sockaddr_in server_addr;
    char buf[BUFSIZ];
    int n;

    connfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);

    Connect(connfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        Write(connfd, buf, strlen(buf));
        n = Read(connfd, buf, sizeof(buf));
        Write(STDOUT_FILENO, buf ,n);
    }
    close(connfd);
    return 0;
}
