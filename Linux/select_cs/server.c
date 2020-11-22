#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>
#include "wrap.h"
#include <sys/wait.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999

int main(void)
{
    int i, j, n, maxi;

    int nready, client[FD_SETSIZE];
    int maxfd, listenfd, connfd, sockfd;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    fd_set rset, allset;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(listenfd, 128);

    maxfd = listenfd;
    maxi = -1;

    for(i = 0;i < FD_SETSIZE; i++) client[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while(1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (nready < 0) perr_exit("select error");

        if (FD_ISSET(listenfd, &rset))
        {
            client_addr_len = sizeof(client_addr);
            connfd = Accept(listenfd, (struct sockaddr *) &client_addr, &client_addr_len);
            printf("client IP : %s , client port : %d connect... ID: [%d]\n\n", inet_ntop(AF_INET, 
                    &client_addr.sin_addr.s_addr, str, sizeof(str)), 
                    ntohs(client_addr.sin_port), connfd);

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            if (i == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
                exit(1);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd) maxfd = connfd;
            if (i > maxi) maxi = i;
            if (--nready == 0) continue;
        }

        for (i = 0; i <= maxi; i++)
        {
            if ((sockfd = client[i]) < 0) continue;
            if (FD_ISSET(sockfd, &rset))
            {
                if ((n = Read(sockfd, buf, sizeof(buf))) ==0)
                {
                    printf("[%d] leave...\n",sockfd);
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else if (n > 0)
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
