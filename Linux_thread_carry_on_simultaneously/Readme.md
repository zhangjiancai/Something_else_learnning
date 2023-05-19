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

[lpthread和pthread的区别](https://zhuanlan.zhihu.com/p/89678862)[^1]

### 属性(attr)
[`attr`是属性的意思，也就是说：`pthread_attr_init` `pthread_attr_setdetachstate`中带有`attr`的都是带有自定义属性的函数](https://blog.csdn.net/zhizhengguan/article/details/114985589)

[^1]: 1、```-lpthread```是较为老式的解决方法，```pthread```新加了对于宏```D_REENTRANT```的定义，```-pthread```会被展开为```-D_REENTRANT -lpthread```，它不仅可以链接```pthread```库，还可以打开系统头文件中的各种多线程支持分支，比如，我们常常使用的错误码标志```errno```，如果没有定义```_REENTRANT```，则实现为一个全局变量；若是定义了```_REENTRANT```，则会实现为每线程独有，从而避免线程竞争错误。
2、```-pthread```可移植性较强：在Linux中，```pthread```是作为一个单独的库存在的```libpthread.so```，但是在其他Unix变种中却不一定，比如在FreeBSD中是没有单独的```pthread```库的，因此在FreeBSD中不能使用```-lpthread```来链接```pthread```，而使用```-pthread```则不会存在这个问题，因为FreeBSD的编译器能正确将```-pthread```展开为该系统下的依赖参数。同样道理，其他不同的变种也会有这样那样的区别，如果使用```-lpthread```, 则可能在移植到其他Unix变种中时会出现问题，为了保持较高的可移植性，我们最好还是使用```-pthread```（尽管这种做法未被接纳成为C标准，但已基本是事实标准）。