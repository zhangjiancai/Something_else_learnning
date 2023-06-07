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
