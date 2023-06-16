
#include "login_register.h"

GtkWidget *message_display;
GtkWidget *message_input;
typedef struct {
    int client_socket;
    // 其他需要传递的数据
    GtkWidget *message_input;
    
} UserData2;

void send_message(GtkWidget *button, gpointer user_data)
{
    UserData2 *data = (UserData2 *)user_data;
    GtkWidget *message_input = data->message_input;
    //const gchar *messages;
    //messages = gtk_entry_get_text(GTK_ENTRY(message_input));
    const gchar *messages=gtk_entry_get_text(GTK_ENTRY(message_input));
    printf("message= %s ",messages);
    
   /* const gchar *sender = NULL;
    const gchar *receiver = NULL;
   // const gchar *sender = NULL;
char *content1 = malloc(strlen(sender) + strlen(receiver) + strlen(messages) + 5);
	strcpy(content1, sender);
        strcat(content1, ",");
        strcat(content1, receiver);
        strcat(content1, ",");
        strcat(content1, messages);
    // 发送消息到服务器
    send(data->client_socket, content1, strlen(content1), 0);
    free(content1);
    */
    int send_length = send(data->client_socket, messages, strlen(messages), 0);
    printf("\nsend_length=%d\n",send_length);
    gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display)), messages, -1);
    gtk_entry_set_text(GTK_ENTRY(message_input), "\n");
   }

void *receive_message(void *arg)  // 修改函数签名
{
 UserData2 *data = (UserData2 *)arg;
   int client_socket = data->client_socket;  // 获取client_socket参数
//int client_socket=4;
    char buffer[4096];
   // char *buffer = (char*)malloc(4096 * sizeof(char));
    int bytes_received;

    while (1) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        printf("bytes_received= %d ",bytes_received);
        if (bytes_received<=0)
        {
        
            // 处理接收错误或连接断开的情况
            perror("recv");
            break;
        }

	printf("%s\n",buffer);
        buffer[bytes_received] = '\0';

        // 在消息显示框中追加接收到的消息
        gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display)), buffer, -1);
        GtkTextIter end;
      GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display));
       gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, buffer, -1);
      // GtkTextIter start,end;/*定义迭代器起点终点*/
        // gchar *S_text,*R_text;/*定义文字存储变量*/
 //gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);

     //       gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&end,"服务器:\n",10);/*插入文本到缓冲区*/
     //           gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&end,buffer,strlen(buffer));/*插入文本到缓冲区*/
      //          gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&end,"\n",1);/*插入文本到缓冲区*/
                
    }

    pthread_exit(NULL);  // 退出线程
}
void chat_show(int client_socket)
{
pthread_t receive_thread;  // 声明receive_thread变量
    UserData2 *data = malloc(sizeof(UserData2));
    if (data == NULL) {
        // 处理内存分配失败的情况
        perror("data");
        return;
    }

    // 将 client_socket 赋值给 UserData 结构体对象的成员变量


    // 初始化GTK
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    message_display = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(message_display), FALSE);
    gtk_box_pack_start(GTK_BOX(main_box), message_display, TRUE, TRUE, 0);
    
    
    message_input = gtk_entry_new();
    //printf("gtk_entry:%s",gtk_entry_new());
    gtk_box_pack_start(GTK_BOX(main_box), message_input, FALSE, FALSE, 0);
    data->client_socket = client_socket;
    data->message_input = message_input;
    //printf("data->client_socket:%d\n",data->client_socket);
    //printf("data->message_input:%s\n", data->message_input);
    GtkWidget *send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), data);
    gtk_box_pack_start(GTK_BOX(main_box), send_button, FALSE, FALSE, 0);
    //printf("client: %d",client_socket);
    if (pthread_create(&receive_thread, NULL, receive_message, data) != 0) 
    {
        // 处理线程创建失败的情况
        perror("pthread_create");
        //return -1;
    }
    gtk_widget_show_all(window);
       // 创建一个单独的线程来接收服务器消息
       //intptr_t client_socket1 = (intptr_t)client_socket;
      
     gtk_main();
}
