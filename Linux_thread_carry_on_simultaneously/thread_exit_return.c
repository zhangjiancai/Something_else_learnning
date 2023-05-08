#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t s_thread_id;
static unsigned char s_thread_running = 0;
const static char *thread_exit_str = "thread_exit ok!";

void *thread_fun(void *arg)
{
    sleep(1);
    printf("thread_fun run...\n");
    pthread_exit(NULL);
}

int main(void)
{
    int ret = 0;
    void *thread_res = NULL;

    printf("Before Thread\n");
    ret = pthread_create(&s_thread_id, NULL, thread_fun, NULL);
    if (ret != 0)
    {
        printf("thread_create error!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("main thread exit\n");
    
    return 0;
}