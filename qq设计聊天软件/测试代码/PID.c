#include <stdio.h>
#include <unistd.h>

int main() {
    char exec_path[256];
    ssize_t len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
    if (len != -1) {
        exec_path[len] = '\0';
        printf("Current process path: %s\n", exec_path);
    } else {
        perror("readlink");
        return 1;
    }

    return 0;
}
