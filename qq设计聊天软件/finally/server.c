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
#include "mydb.h"
#include <json-c/json.h>

#define MAXLEN 1024
#define MAXTHR 10
#define MAX_USERNAME_LENGTH 1024

const int port = 8000;
const char *ip = "127.0.0.1";

//void *send_username(void *buffer);

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

const char** usernames;
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

void send_usernames(int client_socket, const char** usernames, int num_users) {
    json_object* root = json_object_new_array();
    for (int i = 0; i < num_users; i++) {
        json_object_array_add(root, json_object_new_string(usernames[i]));
    }

    const char* jsonString = json_object_to_json_string(root);

    ssize_t bytes_sent = send(client_socket, jsonString, strlen(jsonString), 0);
    if (write_to_file((char*)jsonString) == 0) {
        exit(-1);
    }
    if (bytes_sent == -1) {
        perror("send");
        exit(1);
    }

    json_object_put(root);
}

void *fun_thrreceivehandler(void *socketcon) {
    int buffer_length;
    char buffer[MAXLEN];
    int socketcon1;

    socketcon1 = *((int *)socketcon);

    while (1) {
        printf("server_socket_con:%d\n",socketcon1);
        memset(buffer, '\0', sizeof(buffer));
        buffer_length = read(socketcon1, buffer, MAXLEN - 1);
        printf("buffer_length=%d\n",buffer_length);
        printf("buffer:%s\n",buffer);
        if (buffer_length < 0) {
            if (write_to_file("read from client error") == 0) {
                exit(-1);
            }
        } else if (strncmp(buffer, "NULL", 4) == 0) {
            fprintf(stdout, "套接字:%d close\n", socketcon1);
            exit_thr(&socketcon1);
            arr--;
            pthread_exit("线程退出!");
            break;
        }
        if (strncmp(buffer, "quit", 4) == 0) {
            usernames = malloc(arr * sizeof(const char*));

            for (int i = 0; i < arr; i++) {
                if (write_to_file(arrconsocket[i].username) == 0) {
                    exit(-1);
                }
                const char* temp = malloc(strlen(arrconsocket[i].username) + 1);
                temp = arrconsocket[i].username;
                usernames[i] = temp;
                if (write_to_file((char*)temp) == 0) {
                    exit(-1);
                }
            }

            send_usernames(socketcon1, (const char**)usernames, arr);

           free(usernames);
          // memset(usernames, '\0', sizeof(usernames));
        }
        //buffer[buffer_length] = '\0';
            char* client_name = (char*)malloc(8 * sizeof(char));
if (client_name == NULL) {
    fprintf(stderr, "内存分配失败\n");
    exit(1);  // 根据需要进行错误处理
}
        //send_username(buffer);
        int i=0;
            for(i=1;i<=arr;i++)
        {
        if(arrconsocket[i].socketcon!=socketcon1&&arrconsocket[i].username!=NULL)
        {

            sprintf(client_name,"%s",buffer);
if (write_to_file(client_name) == 0) {
                    exit(-1);
                }
			size_t client_name_len = strlen(client_name);
            int sendmsg_len=write(arrconsocket[i].socketcon,client_name,client_name_len);
                if(sendmsg_len>0)
                {
                    printf("向客户端%s:%d发送成功 %s\n sendmsg_len %d",arrconsocket[i].ipaddr,arrconsocket[i].port,client_name,sendmsg_len);
                }
                else
                {
                    printf("向客户端%s:%d发送失败\n",arrconsocket[i].ipaddr,arrconsocket[i].port);
                }
                
        }
        
        }
        
        free(client_name);  // 释放内存
        sleep(1);
    }//while
}

//void *send_username(void *buffer) {
    // Function implementation
    
//}

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
        memset(buf, '\0', sizeof(buf));
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
            size = read(socketcon, buf, sizeof(buf));
            if (write_to_file(buf) == 0) {
                exit(-1);
            }
            char* username = strtok(buf, ",");
            char* password = strtok(NULL, ",");
            char* sign = strtok(NULL, ",");
            if (write_to_file(username) == 0) {
                exit(-1);
            }
            if (write_to_file(password) == 0) {
                exit(-1);
            }
            if (write_to_file(sign) == 0) {
                exit(-1);
            }

            if (size <= 0) {
                if (write_to_file("receive username error") == 0) {
                    exit(-1);
                }
            } else {
                if (strcmp(sign, "1") == 0) {
                    if (write_to_file("receive user login!") == 0) {
                        exit(-1);
                    }
                    mydb_init();
                    if (mydb_test(mysql, username, password) == 1) {
                        const char* success_signal = "login_success";
                        if (write(socketcon, success_signal, strlen(success_signal)) < 0) {
                            if (write_to_file("send login_success error") == 0) {
                                exit(-1);
                            }
                        }
                        if (write_to_file("login success!") == 0) {
                            exit(-1);
                        }
                        mydb_destory(mysql);
                        arrconsocket[arr].socketcon = socketcon;
                        arrconsocket[arr].ipaddr = inet_ntoa(client_addr.sin_addr);
                        arrconsocket[arr].port = client_addr.sin_port;
                        strncpy(arrconsocket[arr].username, username, sizeof(arrconsocket[arr].username) - 1);
                        arrconsocket[arr].username[sizeof(arrconsocket[arr].username) - 1] = '\0';

                        pthread_create(&thrreceive, NULL, fun_thrreceivehandler, &socketcon);
                        arrthrreceiveclient[arr] = thrreceive;
                    }
                } else if (strcmp(sign, "2") == 0) {
                    if (write_to_file(" receive user regist!") == 0) {
                        exit(-1);
                    }

                    mydb_init();
                    if (mydb_insert(mysql, username, password) == 1) {
                        if (write_to_file("register success!") == 0) {
                            exit(-1);
                        }
                        mydb_destory(mysql);
                    } else {
                        if (write_to_file("register failed!") == 0) {
                            exit(-1);
                        }
                    }
                } else {
                    if (write_to_file("login / register failed!") == 0) {
                        exit(-1);
                    }
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
