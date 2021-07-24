#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#define BUFFSIZE 2048
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 23333
int sock_fd;
void stopServerRunning(int p) {
    close(sock_fd);
    printf("Close Server\n");
    exit(0);
}
int main() {
    // 向服务器（特定的端口）发起请求
    struct sockaddr_in serv_addr;
    char buff[BUFFSIZE];

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    while (true) {
        signal(SIGINT, stopServerRunning);
        sleep(2);
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == sock_fd) {
            printf("Create socket error(%d): %s\n", errno, strerror(errno));
            return -1;
        }

        if ( -1 == connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
            printf("Connect error(%d): %s\n", errno, strerror(errno));
            return -1;
        }
//        printf("Please input: ");
//        scanf("%s", buff);
        memcpy(buff, "ping", sizeof("ping"));
        send(sock_fd, buff, strlen(buff), 0);
        printf("Send: %s\n", buff);

        bzero(buff, sizeof(buff));
        recv(sock_fd, buff, BUFFSIZE - 1, 0);
        printf("Recv: %s\n", buff);
        close(sock_fd);
    }

    return 0;

}