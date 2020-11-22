#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "wrap.h"
#include <strings.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>

#define SERVER_PORT 9999
#define MAXLINE 1024
#define OPEN_MAX 1024

int listenfd;
struct sockaddr_in server_addr;

void listenfd_ini()
{
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(listenfd, 128);

    return;
}

int main(void)
{
    int i, j, connfd, sockfd, epfd, ret;

    int nready;
    ssize_t n;

    char buf[MAXLINE], str[INET_ADDRSTRLEN];

    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    struct epoll_event tep, ep[OPEN_MAX];

    listenfd_ini();

    epfd = epoll_create(OPEN_MAX);
    if (epfd == -1) perr_exit("epoll create error");
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &tep);
    if (ret == -1) perr_exit("epoll control error");

    while(1)
    {
        nready = epoll_wait(epfd, ep, OPEN_MAX, -1);
        if (nready == -1) perr_exit("epoll wait error");

        for (i = 0; i < nready; i++)
        {
            if (!(ep[i].events & EPOLLIN)) continue;
            if (ep[i].data.fd == listenfd)
            {
                client_addr_len = sizeof(client_addr);
                connfd = Accept(listenfd, (struct sockaddr *) &client_addr, &client_addr_len);
                printf("client IP : %s , client port : %d connect... ID: [%d]\n", inet_ntop(AF_INET, 
                        &client_addr.sin_addr.s_addr, str, sizeof(str)), 
                        ntohs(client_addr.sin_port), connfd);

                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &tep);
                if (ret == -1) perr_exit("epoll control error");
            }
            else
            {
                sockfd = ep[i].data.fd;
                n = Read(sockfd, buf, MAXLINE);
                if (n == 0)
                {
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (ret ==  -1) perr_exit("epoll control delete error");
                    Close(sockfd);
                    printf("client[%d] closed connection\n", sockfd);
                }
                else if (n < 0)
                {
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    Close(sockfd);
                    perr_exit("read error");
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
            }
        }
    }
    Close(listenfd);
    return 0;
}
