[多线程编程基础知识总结！](https://mp.weixin.qq.com/s/BP7plMheWW-0lkVif0xS6w)
![image](.\Chatgpt解释.png)
pthread_create()函数用于创建一个新的线程，该线程将执行thread_fun()函数的代码。同时，pthread_join()函数用于等待该线程完成，并获取线程的返回值。因此，您的代码创建了一个新线程，运行thread_fun()函数的代码，然后等待该线程完成并获取返回值。
**也就是说，运行我想要执行的程序的代码和阻碍主进程（main）运行的代码并行运行**