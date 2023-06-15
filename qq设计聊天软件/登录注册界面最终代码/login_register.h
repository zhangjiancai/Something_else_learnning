#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

#include <gtk/gtk.h>
#include <json-c/json.h>

typedef void (*LoginCallback)(const char *content);
typedef void (*RegisterCallback)(const char *content);

//static LoginCallback login_callback;
//static RegisterCallback register_callback;

void login_register_init(LoginCallback login_callback);
void login_register_show(void);
void register_show(void);
void register_init(RegisterCallback register_callback);
void register_callback(const char *content);
void login_callback(const char *content);
void receive_usernames(int client_socket);
int connect_to_server();
void list_show(int client_socket);
void on_window_destroy(GtkWidget *widget, gpointer user_data)
//void chat_show(int client_socket);
//void send_message(GtkWidget *button, gpointer data);

#endif  // LOGIN_REGISTER_H
