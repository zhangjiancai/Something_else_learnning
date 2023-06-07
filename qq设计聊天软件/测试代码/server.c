#include "server.h"

int checkthriskill(pthread_t thr)
{
       //1存在,0不存在
        int res=1;
        //判断线程是否存在
        int kill_rc=pthread_kill(thr,0);

    if(kill_rc == ESRCH)
    {
        printf("ID为%d的线程不存在或者已经退出。\n",(unsigned int)thr);
        res=0;
    }
    else if(kill_rc == EINVAL)
    {
        printf("发送信号非法。\n");
        res=0;
    }
    else
    {
      printf("ID为%d的线程目前仍然存活。\n",(unsigned int)thr);
      res=1;
    }
    return res;
}


//删除杀死的线程
int delete_client(void *fp,int num)
{
    int i=0;

    pthread_t *ps=(pthread_t *)fp;

    if(num<1)
    {
        return 0;   
    }

    for(i=num;i<=arr;i++)
    {
        ps[i]=ps[i+1];  
    arrconsocket[i]=arrconsocket[i+1];
    }

    return 1;
}




int main()
{
    int service_socket=socket(AF_INET,SOCK_STREAM,0);//创建服务端的套接字
    if(service_socket<0)//如果创建套接字失败了
    {
        if(write_to_file("server socket error")==0)
    {
        exit(-1);
    }
        exit(-1);
    }

    struct sockaddr_in addr;//套接字地址结构
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);

    if(bind(service_socket,(struct sockaddr*)&addr,sizeof(addr))<0)//连接套接字结构和套接字
    {
        if(write_to_file("bind error")==0)
    {
        exit(-1);
    }
        exit(-1);
    }

    int listen_socket=listen(service_socket,10);//监听套接字
    if(listen_socket<0)//失败的处理
    {
        if(write_to_file("listen error")==0)
    {
        exit(-1);
    }
        exit(-1);
    }

    pthread_t thraccept;//创建的线程数组
    //pthread_create创建线程（实际上就是确定调用该线程函数的入口点），在线程创建以后，就开始运行相关的线程函数
    if(pthread_create(&thraccept,NULL,fun_thraccepthander,&service_socket)!=0)
    {
    if(write_to_file("pthread_create error")==0)
    {
        exit(-1);
    }
        exit(-1);
    }
    sleep(1);

    while(1)
    {
        int i=1;
        for(i=1;i<=arr;i++)
        {
            if(checkthriskill(arrthrreceiveclient[i])==0)
            {
                printf("have a thread is killed\n");
                if(delete_client((void*)(&arrthrreceiveclient),i)==0)
                {
            if(write_to_file("delete_client error")==0)
            {
                exit(-1);
            }
                        exit(-1);
                }
                --arr;
            }
        }
        printf("在线人数:%d\n",arr);
        if(arr<=0)
        {
            printf("没有客户端连接\n");
        }
        else
        {
            int i=0;
            char buf[MAXLEN];
            ssize_t size=0;

            memset(buf,'\0',MAXLEN);
            size=read(STDIN_FILENO,buf,sizeof(buf));
            if(size>0)
            {
                buf[size]='\0';
            }
            else
            {
        if(write_to_file("read error")==0)
        {
            exit(-1);
        }
                break;
            }

            for(i=1;i<=arr;i++)
            {
                int sendmsg_len=write(arrconsocket[i].socketcon,buf,size);
                if(sendmsg_len>0)
                {
                    printf("向客户端%s:%d发送成功\n",arrconsocket[i].ipaddr,arrconsocket[i].port);
                }
                else
                {
                    printf("向客户端%s:%d发送失败\n",arrconsocket[i].ipaddr,arrconsocket[i].port);
                }
            }
        }
        sleep(1);
    }
    if(arr>=1)
    {
        char *message;
        int res=1;
        printf("等待线程退出\n");


            if((res=pthread_join(thraccept,(void*)&message))==0)
            {
                printf("%s\n",message);
            }
            else
            {
        if(write_to_file("pthread_join error")==0)
        {
            exit(-1);
        }
        exit(-1);
            }

    }
    else
    {
        printf("没有线程\n");
    }
    close(service_socket);
    pthread_kill(thraccept,SIGKILL);
    return(0);
}
