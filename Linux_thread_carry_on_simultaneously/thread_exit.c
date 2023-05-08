/**
 * @brief 退出线程
 *
 * Detailed function description
 *
 * @param[in] retval: 它指向的数据将作为线程退出时的返回值
 *
 * @return void
 * 
 * void pthread_exit(void *retval);
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t s_thread_id[2]={1,2};

//The keyword static specifies that the variable has static storage duration and its value is preserved between function calls. 
//The keyword unsigned specifies that the variable can only hold non-negative values. 
//The keyword char specifies that the variable can hold 8-bit values.
static unsigned char s_thread_running = 0;
//关键字 const 是 C 语言中的一个关键字，用于定义常量。在 C 语言中，const 关键字可以用于修饰变量、函数参数和函数返回值等。在本文所提到的多线程编程中，const 关键字可以用于修饰函数参数，表示该参数是只读的，不能被修改。
const static char *thread_exit_str = "thread_exit OK";

void *thread_fun_second(void *arg)
{
    int cnt = 0;
    
    s_thread_running = 1;
    printf("\n running thread fun \n\n");
    //while s_thread_runnning still under five 
    while (s_thread_running)
    {
        /* code */
        cnt++;
        if(cnt > 5)
        {
            pthread_exit((void*)thread_exit_str);
        }
        printf("thread run...\n");
        //sleep(1);
    }
    printf("\n");
    cnt = 0;
    pthread_exit(NULL);
}

void *thread_fun_2(void *arg)
{
    int ret3 = 0;
    int thread_args[1] = {3};
    static pthread_t s_thread_id_3;
    printf("\nthrid time to create thread beginning to test return\n");
    ret3 = pthread_create(&s_thread_id_3,NULL,thread_fun_second,&thread_args[3]);
    if(ret3 !=0)
    {
        printf("thread_create error\n");
        exit(EXIT_FAILURE);
    }
    printf("main thread exit\n");
    return(0);

}
void *thread_fun_1(void *arg)
{
    static pthread_t s_thread_id_2;
    
    int thread_args[1] = {2};
    int ret2 = 0;
    
    printf("\nsecond time to create thread beginning to test pthread_exit\n");
    ret2 = pthread_create(&s_thread_id_2,NULL,thread_fun_second,&thread_args[2]);
    if(ret2 !=0)
    {
        printf("thread_create error\n");
        exit(EXIT_FAILURE);
    }
    printf("main thread exit\n");
    pthread_exit(NULL);
}

int main(void)
{
    int ret = 0;
    int i;
    int thread_args[2] = {1,4};
    void *thread_res = NULL;

    printf("Before Thread \n");
    //for(i = 0 , i < 3 ; i++)
    //{

        ret = pthread_create(&s_thread_id[1],NULL,thread_fun_1,&thread_args[1]);
    
        if(ret != 0)
        {
            printf("thread_creat error\n");
            exit(EXIT_FAILURE);
        }
    //}
    //符号&表示获取该变量的内存地址，也就是指向该变量的指针。在这里，(void**)&thread_res表示获取thread_res指针的指针，即把thread_res指针的地址传递给pthread_join()函数，让函数能够将线程的返回值存储到该指针所指向的内存地址中。
    //for(i = 0 ;i < 3 ;i++)
   // {
        ret = pthread_join(s_thread_id[1],(void**)&thread_res);

        if(ret != 0 )
        {
            printf("thread_join error!\n");
            exit(EXIT_FAILURE);
        }
   // }

    ret = pthread_create(&s_thread_id[2],NULL,thread_fun_2,&thread_args[4]);
    
        if(ret != 0)
        {
            printf("thread_creat error\n");
            exit(EXIT_FAILURE);
        }

    ret = pthread_join(s_thread_id[2],(void**)&thread_res);

        if(ret != 0 )
        {
            printf("thread_join error!\n");
            exit(EXIT_FAILURE);
        }
    printf("After Thread,thread_res = %s\n",(char*)thread_res);
    exit(EXIT_SUCCESS);


}