/**
 * @brief 创建一个线程
 *
 * Detailed function description
 *
 * @param[in] thread: 一个指向线程标识符的指针，线程调用后，该值被设置为线程ID；pthread_t为unsigned long int
 * @param[in] attr: 用来设置线程属性
 * @param[in] start_routine: 线程函数体，线程创建成功后，thread 指向的内存单元从该地址开始运行
 * @param[in] arg: 传递给线程函数体的参数
 *
 * @return 线程创建成功，则返回0，失败则返回错误码，并且 thread 内容是未定义的
 * 
 * int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void*(*start_routine)(void*), void* arg);
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t s_thread_id;
static unsigned char s_thread_running = 0 ;

void *thread_fun(void *arg)
{
	s_thread_running = 9 ;
	while(s_thread_running > 0 )
	{
		printf("thread run...\n");
		sleep(1);
		s_thread_running=s_thread_running - 1;
	}
	pthread_exit(NULL);
}

int main(void)
{
	int ret = 1;

	printf("Before Thread\n");
	ret = pthread_create(&s_thread_id,NULL,thread_fun,NULL);

	if(ret != 0 )
	{
		printf("thread_create error!\n");
		exit(EXIT_FAILURE);
	}

	ret = pthread_join(s_thread_id,NULL);
	if(ret != 0 )
	{
		printf("pthread_join error \n");
		exit(EXIT_FAILURE);
	}
	printf("After Thread\n");
	exit(EXIT_SUCCESS);
}

