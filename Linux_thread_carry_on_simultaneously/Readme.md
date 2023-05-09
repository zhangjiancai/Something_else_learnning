[多线程编程基础知识总结！](https://mp.weixin.qq.com/s/BP7plMheWW-0lkVif0xS6w)
![image](.\Chatgpt解释.png)
pthread_create()函数用于创建一个新的线程，该线程将执行thread_fun()函数的代码。同时，pthread_join()函数用于等待该线程完成，并获取线程的返回值。因此，您的代码创建了一个新线程，运行thread_fun()函数的代码，然后等待该线程完成并获取返回值。
**也就是说，运行我想要执行的程序的代码和阻碍主进程（main）运行的代码并行运行**

### pthread_join与pthraad_detach的区别：
:joy:`pthread_detach()`即主线程与子线程**分离**，两者相互不干涉，子线程结束**同时**子线程的资源自动回收。
:joy:`pthread_join()`即是子线程**合入**主线程，主线程会一直阻塞，**直到**子线程执行结束，然后回收子线程资源，并继续执行。

## command
`gcc thread_something.c -o thread_something -lpthread`  
注意要有`lpthread`
### 属性(attr)
[`attr`是属性的意思，也就是说：`pthread_attr_init` `pthread_attr_setdetachstate`中带有`attr`的都是带有自定义属性的函数](https://blog.csdn.net/zhizhengguan/article/details/114985589)