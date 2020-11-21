#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include "wrap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define MAXLINE 8192

struct s_info
{
    struct sockaddr_in client_addr;
    int conn_fd;
};

void *do_work(void *arg)
{
    int n, i;
    struct s_info *ts = (struct s_info*)arg;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    while(1)
    {
        n = Read(ts->conn_fd, buf, MAXLINE);
        if (n == 0)
        {
            printf("The client %d closed...\n",ts->conn_fd);
            break;
        }
        printf("received from %s at PORT %d\n",
                inet_ntop(AF_INET, &(*ts).client_addr.sin_addr.s_addr, str, sizeof(str)),
                ntohs((*ts).client_addr.sin_port));
        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);

        Write(STDOUT_FILENO, buf, n);
        Write(ts->conn_fd, buf, n);
    }
    Close(ts->conn_fd);

    return (void *)0;
}
    
int main(void)
{
    pthread_t tid;
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    struct s_info ts[256];
    int i = 0;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(listen_fd, 128);

    printf("Accepting client connect...\n");

    while(1)
    {
        client_addr_len = sizeof(client_addr);
        conn_fd = Accept(listen_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        ts[i].client_addr = client_addr;
        ts[i].conn_fd = conn_fd;

        pthread_create(&tid, NULL, do_work, (void*)&ts[i]);
        pthread_detach(tid);
        i++;
    }

    return 0;
}
