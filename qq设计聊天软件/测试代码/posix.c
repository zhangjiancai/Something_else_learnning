#include <stdio.h>
#include <spawn.h>
#include <unistd.h>

int main() {
    pid_t pid;
    char *const argv[] = { "ls", "-l", NULL };
    char *const envp[] = { NULL };
    posix_spawn_file_actions_t file_actions;
    posix_spawnattr_t attr;

    char exec_path[256]; // 声明一个足够大的字符数组来存储可执行文件路径
    ssize_t len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
    if (len != -1) {
        exec_path[len] = '\0'; // 将最后一个字符设置为终止符
    } else {
        perror("readlink");
        return 1;
    }

    posix_spawn_file_actions_init(&file_actions);
    posix_spawnattr_init(&attr);

    if (posix_spawn(&pid, exec_path, &file_actions, &attr, argv, envp) == 0) {
        printf("New process created with PID: %d\n", pid);
    } else {
        perror("posix_spawn");
    }

    posix_spawn_file_actions_destroy(&file_actions);
    posix_spawnattr_destroy(&attr);

    return 0;
}
#include <stdio.h>
#include <spawn.h>
#include <unistd.h>

int main() {
    pid_t pid; // 用于存储新进程的PID（进程ID）
    char *const argv[] = { "ls", "-l", NULL }; // 传递给新进程的命令行参数
    char *const envp[] = { NULL }; // 传递给新进程的环境变量
    posix_spawn_file_actions_t file_actions; // 用于设置文件操作的结构体
    posix_spawnattr_t attr; // 用于设置进程属性的结构体
    
    posix_spawn_file_actions_init(&file_actions); // 初始化文件操作结构体
    posix_spawnattr_init(&attr); // 初始化进程属性结构体
    
    // 通过posix_spawn函数创建新进程
    // 参数依次为：新进程的PID变量、可执行文件路径、文件操作结构体、进程属性结构体、命令行参数、环境变量
    if (posix_spawn(&pid, "/bin/ls", &file_actions, &attr, argv, envp) == 0) {
        printf("New process created with PID: %d\n", pid); // 打印新进程的PID
    } else {
        perror("posix_spawn"); // 打印错误信息
    }
    
    posix_spawn_file_actions_destroy(&file_actions); // 销毁文件操作结构体
    posix_spawnattr_destroy(&attr); // 销毁进程属性结构体
    
    return 0;
}
