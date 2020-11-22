#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "wrap.h"
#include <strings.h>
#include <poll.h>
#include <netinet/in.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define MAXLINE 80
#define OPEN_MAX 1024

int main(void)
{
    int i, j, maxi, listenfd, connfd, sockfd;

    int nready;
    ssize_t n;

    char buf[MAXLINE], str[INET_ADDRSTRLEN];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    struct pollfd client[OPEN_MAX];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(listenfd, 128);
    
    client[0].fd = listenfd;
    client[0].events = POLLIN;

    for(i = 1;i < OPEN_MAX; i++) client[i].fd = -1;
    maxi = 0;

    while(1)
    {
        nready = poll(client, maxi+1, -1);
        if (client[0].revents & POLLIN)
        {
            client_addr_len = sizeof(client_addr);
            connfd = Accept(listenfd, (struct sockaddr *) &client_addr, &client_addr_len);
            printf("client IP : %s , client port : %d connect... ID: [%d]\n", inet_ntop(AF_INET, 
                    &client_addr.sin_addr.s_addr, str, sizeof(str)), 
                    ntohs(client_addr.sin_port), connfd);

            for (i = 1; i < OPEN_MAX; i++)
                if (client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            if (i == OPEN_MAX)
                perr_exit("too many clients");

            client[i].events = POLLIN;
            if (i > maxi) maxi = i;
            if (--nready <= 0) continue;
        }

        for (i = 1; i <= maxi; i++)
        {
            if ((sockfd = client[i].fd) < 0) continue;
            if (client[i].revents & POLLIN)
            {
                if ((n = Read(sockfd, buf, sizeof(buf))) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        printf("client [%d] aborted connection\n",i);
                        Close(sockfd);
                        client[i].fd = -1;
                    }
                    else perr_exit("read error");
                }
                else if (n == 0)
                {
                    printf("[%d] leave...\n",sockfd);
                    Close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                    for (j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, n);
                    printf("[%d]: ", sockfd);
                    fflush(stdout);
                    Write(STDOUT_FILENO, buf, n);
                }
                if (--nready == 0) break;
            }
        }
    }
    Close(listenfd);
    return 0;
}
