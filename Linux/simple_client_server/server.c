#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

int main(void)
{
    int lfd, cfd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_len, clinet_IP_len;
    char buf[BUFSIZ], client_IP[BUFSIZ];
    int n, i, ret;
    
    lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket error");
		exit(1);
	}

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (ret == -1)
	{
		perror("bind error");
		exit(1);
	}

    ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen error");
		exit(1);
	}

    client_addr_len = sizeof(client_addr);
    cfd = accept(lfd, (struct sockaddr *) &client_addr, &client_addr_len);
	if (cfd == -1)
	{
		perror("accept error");
		exit(1);
	}

    printf("client IP : %s , client port : %d \n", inet_ntop(AF_INET, 
    	&client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), 
    	ntohs(client_addr.sin_port));

    while(1)
    {
        n = read(cfd, buf, sizeof(buf));
        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        write(cfd, buf, n);
    }
    close(lfd);
    close(cfd);
    return 0;
}
