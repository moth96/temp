#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

int create_sock(u_short*);
void perror_exit(const char*);

int main(void)
{
    int server_sock = -1;
    int save_log = -1;
    u_short port = 0;
    int client_sock = -1;
    struct sockaddr_in client_name;
    int client_name_len = sizeof(client_name);
    char buf[1024];

    server_sock = create_sock(&port);
    printf("httpd running on port %d\n", port);

    client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
    if(client_sock == -1) perror_exit("accept");
    
    save_log = open("http.log",O_WRONLY | O_CREAT);
    int n = read(client_sock, buf, sizeof(buf));
    write(save_log, buf, n);

    close(save_log);
    close(server_sock);
    close(client_sock);
    return 0;
}

int create_sock(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name;  
    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        perror_exit("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY); 
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        perror_exit("bind");
    if (*port == 0)  /* if dynamically allocating a port */
    {
        int namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
            perror_exit("getsockname");
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0)
        perror_exit("listen");
    return(httpd);
}

void perror_exit(const char *s)
{
    perror(s);
    exit(1);
}
