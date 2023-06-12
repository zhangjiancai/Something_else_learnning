#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#define MAXLEN 1024
#define MAXTHR 10
#define MAX_USERNAME_LENGTH 50

const int port = 8888;
const char *ip = "127.0.0.1";

void *send_username(void *buffer);

typedef struct mysocketinfo {
    int socketcon;
    char *ipaddr;
    uint16_t port;
    char username[MAX_USERNAME_LENGTH];
} _mysocketinfo;

typedef struct {
    char sender[50];
    char receiver[50];
    char message[MAXLEN];
} Message;


pthread_t arrthrreceiveclient[MAXTHR];
struct mysocketinfo arrconsocket[MAXTHR];
static int arr = 0;

int exit_thr(void *_socketcon) {
    int socketcon = *((int *)_socketcon);
    int i = 0, k = 0;
    printf("线程退出\n");
    for (i = 0; i <= arr; i++) {
        if (arrconsocket[i].socketcon == socketcon) {
            for (k = i; k <= arr; k++) {
                arrconsocket[k] = arrconsocket[k + 1];
                arrthrreceiveclient[k] = arrthrreceiveclient[k + 1];
            }
        }
    }
}

/*写入日志文件*/
int write_to_file(char *name) {
    FILE *fp;
    char buffer[50];
    time_t timep;
    char time_buffer[100];

    fp = fopen("server.log", "a+");
    if (fp == NULL) {
        exit(-1);
    }

    sprintf(buffer, "%s\n", name);
    if (fwrite(buffer, sizeof(char), strlen(buffer), fp) <= 0) {
        return 0;
    }

    time(&timep);
    sprintf(time_buffer, "%s\n", ctime(&timep));
    if (fwrite(time_buffer, sizeof(char), strlen(time_buffer), fp) <= 0) {
        return 0;
    }

    fclose(fp);
    return 1;
}

//接收消息函数
void *fun_thrreceivehandler(void *socketcon) {
    char buffer[MAXLEN];
    int buffer_length;
    int socketcon1;

    socketcon1 = *((int *)socketcon);
    while (1) {
        memset(buffer, '\0', sizeof(buffer));

    //    printf("接收套接字：%d\n", socketcon1);
        buffer_length = read(socketcon1, buffer, MAXLEN - 1);
        if (buffer_length < 0) {
            if (write_to_file("read from client error") == 0) {
                exit(-1);
            }
            exit(-1);
        } else if (strncmp(buffer, "NULL", 4) == 0) {
            fprintf(stdout, "套接字:%d close\n", socketcon1);
            exit_thr(&socketcon1);
            arr--;
            pthread_exit("线程退出!");
            break;
        }
        if (strncmp(buffer, "quit", 4) == 0) {
            fprintf(stdout, "套接字:%d close\n", socketcon1);
            exit_thr(&socketcon1);
            arr--;
            pthread_exit("线程退出!");
            break;
        }
        buffer[buffer_length] = '\0';
    //    printf("客户端%d:%s\n", socketcon1, buffer);

        /*------------------------这是一个已经完整的数据包，可以写入数据库-------------------------------*/

        send_username(buffer);
    }
}
void *send_username(void *buffer){
// 解析接收到的消息
Message msg;
memcpy(&msg, buffer, sizeof(Message));

// 查找接收者并发送消息
int i;
for (i = 1; i <= arr; i++) {
    if (strcmp(arrconsocket[i].username, msg.receiver) == 0) {
        // 找到接收者，发送消息
        int sendmsg_len = write(arrconsocket[i].socketcon, &msg, sizeof(Message));
        if (sendmsg_len > 0) {
            printf("向客户端%s:%d发送成功\n", arrconsocket[i].ipaddr, arrconsocket[i].port);
        } else {
            printf("向客户端%s:%d发送失败\n", arrconsocket[i].ipaddr, arrconsocket[i].port);
        }
        break;
    }
}
}
//处理 accept
void *fun_thraccepthander(void *socketlisten) {
    char buf[MAXLEN];
    ssize_t size;
    int sockaddr_in_size = sizeof(struct sockaddr_in);
    int socklisten1 = *((int *)socketlisten);
    int socketcon;
    pthread_t thrreceive = 0;
    struct sockaddr_in client_addr;
    char accept_message[50];

    while (1) {
        socketcon = accept(socklisten1, (struct sockaddr *)(&client_addr), (socklen_t *)(&sockaddr_in_size));
        if (socketcon < 0) {
            if (write_to_file("accept error") == 0) {
                exit(-1);
            }
        } else {
            if (write_to_file("accept success") == 0) {
                exit(-1);
            }

            sprintf(accept_message, "ip: %s, port: %d", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
            if (write_to_file(accept_message) == 0) {
                exit(-1);
            }

            printf("在线人数:%d\n", ++arr);
            printf("连接客户端的套接字:%d\n", socketcon);

            // 向客户端发送请求，要求发送用户名信息
            const char *request_username = "Please send your username";
            if (write(socketcon, request_username, strlen(request_username)) < 0) {
                // 处理发送请求失败
                if (write_to_file("send username request error") == 0) {
                    exit(-1);
                }
            } else {
                // 接收用户名
                size = read(socketcon, arrconsocket[arr].username, sizeof(arrconsocket[arr].username));
                if (size <= 0) {
                    // 处理接收用户名错误
                    if (write_to_file("receive username error") == 0) {
                        exit(-1);
                    }
                } else {
                    // 其他处理逻辑
                    arrconsocket[arr].socketcon = socketcon;
                    arrconsocket[arr].ipaddr = inet_ntoa(client_addr.sin_addr);
                    arrconsocket[arr].port = client_addr.sin_port;

                    // 创建线程处理消息
                    pthread_create(&thrreceive, NULL, fun_thrreceivehandler, &socketcon);
                    arrthrreceiveclient[arr] = thrreceive;
                }
            }
        }
    }
}

int main() {
    int socklisten;
    int opt = 1;
    struct sockaddr_in server_addr;

    socklisten = socket(AF_INET, SOCK_STREAM, 0);
    if (socklisten < 0) {
        perror("socket");
        exit(-1);
    }

    setsockopt(socklisten, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socklisten, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0) {
        perror("bind");
        exit(-1);
    }

    if (listen(socklisten, 5) < 0) {
        perror("listen");
        exit(-1);
    }

    pthread_t thraccept;
    pthread_create(&thraccept, NULL, fun_thraccepthander, &socklisten);

    pthread_join(thraccept, NULL);

    close(socklisten);

    return 0;
}
