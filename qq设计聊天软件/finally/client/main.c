#include "login_register.h"


#define MAXLEN 4096
#define MAX_USERNAME_LENGTH 50

char *message = NULL;
const int port = 8000;
const char *ip = "127.0.0.1";
int client_socket;

// 连接到服务端
int connect_to_server() {
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));//---------------?

    // 创建客户端套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
   // printf("client_main_socket:%d",client_socket);
    if (client_socket < 0) 
    {
        perror("socket");
        return -1;
    }

    // 设置服务端地址
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(server_address.sin_addr)) <= 0) {
        perror("inet_pton");
        return -1;
    }

    // 连接到服务端
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect");
        return -1;
    }

    return client_socket;
}

// 登录回调函数
void login_callback(const char *content) {
    printf("Login successful!\n");
    printf("Content: %s\n", content);
    message = (char *)content;  // 强制类型转换，将 const char * 赋值给 char *

    if (message != NULL) {
        printf("message: %s\n", message);
        size_t message_len = strlen(message);
        ssize_t bytes_sent = send(client_socket, message, message_len, 0);
        if (bytes_sent == -1) {
            perror("send");
            exit(1);
        }
        char buffer_login[MAXLEN];
        ssize_t bytes_received = recv(client_socket, buffer_login, sizeof(buffer_login) - 1, 0);
        if (strncmp(buffer_login, "login_success", 13) == 0) {
           // list_show(client_socket);
           chat_show(client_socket);
        } else {
            printf("%s", buffer_login);
        }
    }
    memset(message, '\0', strlen(message));  // 清零内存区域
}

// 注册回调函数
void register_callback(const char *content) {
    printf("Registration successful!\n");
    printf("Content: %s\n", content);
    message = (char *)content;  // 强制类型转换，将 const char * 赋值给 char *

    if (message != NULL) {
        printf("message: %s\n", message);
        size_t message_len = strlen(message);
        ssize_t bytes_sent = send(client_socket, message, message_len, 0);
        if (bytes_sent == -1) {
            perror("send");
            exit(1);
        }
    }
    memset(message, '\0', strlen(message));  // 清零内存区域
}

int main() {
    client_socket = connect_to_server();
    printf("socket=%d  ",client_socket);
    if (client_socket < 0) {
        // 处理连接服务端失败的情况
        return -1;
    }

    // 调用登录界面库函数，并传递登录回调函数
    login_register_init(login_callback);
    // 显示登录界面
    login_register_show();
    memset(message, '\0', strlen(message));  // 清零内存区域

    return 0;
}
