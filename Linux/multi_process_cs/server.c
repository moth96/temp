#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>
#include "wrap.h"
#include <sys/wait.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999

void wait_child(int signo)
{
    while (waitpid(0, NULL, WNOHANG) > 0);
    return;
}

int main(void)
{
    pid_t pid;
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buf[BUFSIZ], client_IP[BUFSIZ];
    int n, i;

    server_fd = Socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(server_fd, 128);

    while(1)
    {
        client_addr_len = sizeof(client_addr);
        client_fd = Accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        printf("client IP : %s , client port : %d \n", inet_ntop(AF_INET, 
                    &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), 
                ntohs(client_addr.sin_port));

        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
            exit(1);
        }
        else if (pid == 0)
        {
            Close(server_fd);
            break;
        }
        else
        {
            Close(client_fd);
            signal(SIGCHLD, wait_child);
        }
    }
    if (pid == 0)
    {
        while(1)
        {
            n = Read(client_fd, buf, sizeof(buf));
            if (n == 0)
            {
                Close(client_fd);
                return 0;
            }
            else if (n == -1)
            {
                perror("read error");
                exit(1);
            }
            else
            {
                for (i = 0; i < n; i++)
                    buf[i] = toupper(buf[i]);
                Write(client_fd, buf, n);
            }
        }
    }
    return 0;
}
