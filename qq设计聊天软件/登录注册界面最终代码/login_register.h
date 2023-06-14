#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

#include <gtk/gtk.h>

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
#endif  // LOGIN_REGISTER_H
