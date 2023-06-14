#include <stdio.h>
#include "login_register.h"
// 登录回调函数
void login_callback(const char *content) {
    printf("Login successful!\n");
    printf("Content: %s\n", content);
}

// 注册回调函数
void register_callback(const char *content) {
    printf("Registration successful!\n");
    printf("Content: %s\n", content);
}

int main() {
    // 调用登录界面库函数，并传递登录回调函数
    login_register_init(login_callback);
    //register_init(register_callback);
    // 显示登录界面
    login_register_show();
   // register_show();

    return 0;
}
