#include "login_register.h"

GtkWidget *window;
GtkWidget *listbox;
#define MAXLEN 1024
typedef struct {
    int client_socket;
    // 其他需要传递的数据
} UserData;

static void on_user_selected(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data)
{
   // UserData *data = (UserData *)user_data;
    const *username = gtk_label_get_text(GTK_LABEL(gtk_bin_get_child(GTK_BIN(row))));
    // TODO: 根据所选用户执行相应的聊天逻辑，更新文本区域中的聊天内容
    g_print("开始与用户 %s 进行聊天\n", username);
    //chat_show(data->client_socket);
}


void receive_usernames(int client_socket) {
        // 向服务端发送请求获取用户列表
    const char *request_users = "quit";
    ssize_t send_len = send(client_socket, request_users, strlen(request_users), 0);
    if (send_len < 0)
    {
        perror("send");
        // TODO: 处理发送请求失败的情况
        return;
    }

    
    char buffer[MAXLEN];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("recv");
        exit(1);
    }

    // 将接收到的数据转换为 JSON 字符串
    buffer[bytes_received] = '\0';
    const char* jsonString = buffer;

    // 解析 JSON 字符串
    json_object* root = json_tokener_parse(jsonString);
    if (root == NULL) {
        fprintf(stderr, "Failed to parse JSON\n");
        exit(1);
    }

    // 获取用户名列表
    int num_users = json_object_array_length(root);
    printf("Received %d usernames:\n", num_users);
    for (int i = 0; i < num_users; i++) {
        json_object* usernameObj = json_object_array_get_idx(root, i);
        const char* username = json_object_get_string(usernameObj);
        printf("- %s\n", username);
        GtkWidget *list_item = gtk_label_new(username);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), list_item, -1);
    }

    // 释放 JSON 对象
    json_object_put(root);
}
void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
    //int client_socket;
    // 关闭套接字
    // 将用户数据指针转换为 UserData 类型
    UserData *data = (UserData *)user_data;

    if (close(data->client_socket) < 0) {
        perror("close");
        // TODO: 处理关闭套接字失败的情况
    }
    gtk_main_quit();
}
void list_show(int client_socket){
        UserData *data = malloc(sizeof(UserData));
    if (data == NULL) {
        // 处理内存分配失败的情况
       perror("data");
    }

    // 将 client_socket 赋值给 UserData 结构体对象的成员变量
    data->client_socket = client_socket;
    // 初始化GTK
    gtk_init(NULL,NULL);

    // 创建主窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), data);

    // 创建垂直布局容器
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // 创建用户列表容器
    listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_SINGLE);
    g_signal_connect(listbox, "row-selected", G_CALLBACK(on_user_selected), data);
    gtk_box_pack_start(GTK_BOX(vbox), listbox, TRUE, TRUE, 0);

// 获取用户列表并更新界面

     receive_usernames(client_socket);


   

    // 显示窗口并启动主循环
    gtk_widget_show_all(window);
    gtk_main();
}