/**
 * @brief 等待某个线程结束
 *
 * Detailed function description: 这是一个线程阻塞函数，调用该函数则等到线程结束才继续运行
 *
 * @param[in] thread: 某个线程的ID
 * @param[in] retval: 用于获取线程 start_routine 的返回值
 *
 * @return 线程创建成功，则返回0，失败则返回错误码，并且 thread 内容是未定义的
 * 
 * int pthread_join(pthread_t thread, void **retval);
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t s_thread_id;
static unsigned char s_thread_running= 0;

void *thread_fun(void *arg)
{
    static int res = 0;
    int a = 1,b = 2;

    res = a + b ;
    sleep(1);
    
    printf("thread run ,a+b = %d,addr = %p\n",res,&res);

    pthread_exit(&res);

}

int main(void)
{
    int ret = 0;
    int *retval = NULL;
    printf("Before Thread \n");
    ret = pthread_create(&s_thread_id,NULL,thread_fun,NULL);

    if (ret != 0 )
    {
        printf("pthread_create error\n");
        exit(EXIT_FAILURE);
    }
    //在这个代码中，Pthread_join是否是必要的。为啥

    //在这个代码中，pthread_join是必要的。因为如果不使用pthread_join，主线程会直接退出，而子线程则会继续执行，这可能会导致一些问题，比如资源泄漏或者线程死锁。使用pthread_join可以让主线程等待子线程结束，然后再继续执行下面的代码，同时还可以获取子线程的返回值。在本代码中，如果不使用pthread_join，程序不会输出"After Thread"这行信息和子线程的返回值。


    ret = pthread_join(s_thread_id,(void **)&retval);

    if(ret != 0)
    {
        printf("pthread_join error\n");
        exit(EXIT_FAILURE);
    }
    if (retval != NULL)
    {
        printf("After Thread ,retval = %d,addr = %p\n",(int)*retval,retval);
    }

    exit(EXIT_SUCCESS);
}