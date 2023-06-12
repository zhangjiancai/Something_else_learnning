#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MAXLEN 1024
#define MAX_USERNAME_LENGTH 50

const int port = 8888;
const char *ip = "127.0.0.1";

GtkWidget *window;
GtkWidget *listbox;
int client_socket;

// 回调函数：处理用户点击列表项的事件
static void on_user_selected(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data)
{
    gchar *username = gtk_label_get_text(GTK_LABEL(gtk_bin_get_child(GTK_BIN(row))));
    // TODO: 根据所选用户执行相应的聊天逻辑，更新文本区域中的聊天内容
    g_print("开始与用户 %s 进行聊天\n", username);
}

// 连接到服务端
int connect_to_server()
{
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    // 创建客户端套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("socket");
        return -1;
    }

    // 设置服务端地址
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(server_address.sin_addr)) <= 0)
    {
        perror("inet_pton");
        return -1;
    }

    // 连接到服务端
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect");
        return -1;
    }

    return client_socket;
}

// 获取用户列表并更新界面
void update_user_list()
{
    // 向服务端发送请求获取用户列表
    const char *request_users = "Please send the user list";
    ssize_t send_len = send(client_socket, request_users, strlen(request_users), 0);
    if (send_len < 0)
    {
        perror("send");
        // TODO: 处理发送请求失败的情况
        return;
    }

    // 接收用户列表
    char user_list[MAXLEN];
    ssize_t recv_len = recv(client_socket, user_list, sizeof(user_list) - 1, 0);
    if (recv_len < 0)
    {
        perror("recv");
        // TODO: 处理接收用户列表失败的情况
        return;
    }

    // 解析用户列表
    user_list[recv_len] = '\0';
    char *token = strtok(user_list, "\n");
    while (token != NULL)
    {
        // 添加用户列表项到界面
        GtkWidget *list_item = gtk_label_new(token);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), list_item, -1);
        token = strtok(NULL, "\n");
    }
}

int main(int argc, char *argv[])
{
    // 初始化GTK
    gtk_init(&argc, &argv);

    // 创建主窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 创建垂直布局容器
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // 创建用户列表容器
    listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_SINGLE);
    g_signal_connect(listbox, "row-selected", G_CALLBACK(on_user_selected), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), listbox, TRUE, TRUE, 0);

    // 连接到服务端
    client_socket = connect_to_server();
    if (client_socket < 0)
    {
        // TODO: 处理连接服务端失败的情况
        return -1;
    }

    // 获取用户列表并更新界面
    update_user_list();

    // 显示窗口并启动主循环
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
