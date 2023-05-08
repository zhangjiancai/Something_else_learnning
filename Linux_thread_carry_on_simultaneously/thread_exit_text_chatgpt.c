#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t s_thread_id;
static unsigned char s_thread_running = 0;

void *thread_fun(void *arg)
{
    static int cnt = 0;
    printf("thread_fun running\n");
    s_thread_running = 1;
    while (s_thread_running)
    {
        cnt++;
        if (cnt > 5)
        {
            pthread_exit(NULL);
        }
        printf("thread run...\n");
        //sleep(1);
    }

    pthread_exit(NULL);
}

int main(void)
{
    int ret = 0;

    printf("Before Thread\n");
    ret = pthread_create(&s_thread_id, NULL, thread_fun, NULL);
    if (ret != 0)
    {
        printf("thread_create error!\n");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(s_thread_id, NULL);
    printf("Thread exit!\n");

    // 创建新线程
    printf("Create new thread\n");
    ret = pthread_create(&s_thread_id, NULL, thread_fun, NULL);
    if (ret != 0)
    {
        printf("thread_create error!\n");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(s_thread_id, NULL);
    printf("Thread exit!\n");

    exit(EXIT_SUCCESS);
}
