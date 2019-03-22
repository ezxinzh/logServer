#if 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

#define MAXLINE 1024
#define DEFAULT_PORT (32006)
//#define DEFAULT_ADDR ("120.78.206.87")
#define max( a, b) (a > b) ? a : b
static void handle_connection( int sockfd);

int main( int argc, char *argv[])
{
    int connfd = 0;
    int cLen = 0;
    struct sockaddr_in client;
    if( argc < 2)
    {
        printf(" Uasge: clientent [server IP address]\n");
        return -1;
    }
    client.sin_family = AF_INET;
    client.sin_port = htons( DEFAULT_PORT);
    client.sin_addr.s_addr = inet_addr( argv[1]);
//    client.sin_addr.s_addr = inet_addr(DEFAULT_ADDR);
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if( connfd < 0)
    {
        perror(" socket" );
        return -1;
    }
    if( connect( connfd, (struct sockaddr*)& client, sizeof( client)) < 0)
    {
        perror(" connect" );
        return -1;
    }
    else
    {
        printf("connect server:%s successful.\n", inet_ntoa(client.sin_addr));
        write(connfd, "hello server, i am client.\n", strlen("hello server, i am client.\n"));
    }
    /*处理 连接 描述 符*/
    handle_connection(connfd);
    return 0;
}
static void handle_connection( int sockfd)
{
    char sendline[MAXLINE] = {0};
    char recvline[MAXLINE];
    int maxfdp, stdineof;
    struct pollfd pfds[2];
    /* 添加连接描述符 */
    int n;
    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;
    /* 添加标准输入描述符 */
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;
    int cnt = 0;
    char buf[256] = {0};
    char buf1[256] = "\n                       hello, welcome to my world";

    while(1)
    {
        poll(pfds, 2, -1);
        if(pfds[0].revents & POLLIN)
        {
            memset(recvline, 0, MAXLINE);
            n = read(sockfd, recvline, MAXLINE);
            if(n == 0)
            {
                fprintf(stderr, "client:server is closed.");
                close(sockfd);
            }
//            write(STDOUT_FILENO, recvline, n);
//            write(STDOUT_FILENO, "\n", n);
            if(cnt >= 32767)
                cnt = 0;
            cnt += 1;
            sprintf(buf, " %d times ", cnt);
            strcpy(sendline, buf1);
            strcat(sendline, buf);
            write(sockfd, sendline, strlen(sendline));
            usleep(100000);
        }
        /* 测试标准输入是否准备好 */
        if(pfds[1].revents & POLLIN)
        {
            n = read(STDIN_FILENO, sendline, MAXLINE);
            if(n == 0)
            {
                shutdown(sockfd, SHUT_WR);
                continue;
            }
            write(sockfd, sendline, n);
        }
    }
}
#endif
