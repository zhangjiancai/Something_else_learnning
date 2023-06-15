#include <gtk/gtk.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <login_register.h>

GtkWidget *message_display;
GtkWidget *message_input;

typedef struct {
    int client_socket;
    // 其他需要传递的数据
} UserData;

void send_message(GtkWidget *button, gpointer data)
{
    UserData *data = (UserData *)user_data;
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(message_input));
    gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display)), message, -1);
    gtk_entry_set_text(GTK_ENTRY(message_input), "");

    // 发送消息到服务器
    send(data->client_socket, message, strlen(message), 0);

}

void chat_show(int client_socket){
    UserData *data = malloc(sizeof(UserData));
    if (data == NULL) {
        // 处理内存分配失败的情况
       perror("data");
    }

    // 将 client_socket 赋值给 UserData 结构体对象的成员变量
    data->client_socket = client_socket;
    // 初始化GTK
    gtk_init(NULL,NULL);
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
    gtk_box_pack_start(GTK_BOX(main_box), message_input, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), data);
    gtk_box_pack_start(GTK_BOX(main_box), send_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}