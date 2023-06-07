#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#define MAXLEN 1024
#define MAXTHR 10

const int port=8888;
const char *ip="127.0.0.1";

typedef struct mysocketinfo{
    int socketcon;
    char *ipaddr;
    uint16_t port;
}_mysocketinfo;

pthread_t arrthrreceiveclient[10];
struct mysocketinfo arrconsocket[10];
static int arr=0;

int exit_thr(void *_socketcon)
{
    int socketcon=*((int *)_socketcon);
    int i=0,k=0;
    printf("线程退出\n");
    for(i=0;i<=arr;i++){
        if(arrconsocket[i].socketcon==socketcon)
        {
                for(k=i;k<=arr;k++)
                {
                    arrconsocket[k]=arrconsocket[k+1];
                        arrthrreceiveclient[k]=arrthrreceiveclient[k+1];
                 }
        }
    }
}

/*写入日志文件*/
int write_to_file(char *name)
{   
    FILE *fp;
    char buffer[50];
    time_t timep;
    char time_buffer[100];

    fp=fopen("server.log","a+");
    if(fp==NULL)
    {
        exit(-1);
    }       

    sprintf(buffer,"%s\n",name);
    if(fwrite(buffer,sizeof(char),strlen(buffer),fp)<=0)
    {
        return 0;
    }

    time (&timep);
    sprintf(time_buffer,"%s\n",ctime(&timep));
    if(fwrite(time_buffer,sizeof(char),strlen(time_buffer),fp)<=0)
    {
        return 0;
    }

    fclose(fp);
    return 1;
}

//接收消息函数
void *fun_thrreceivehandler(void *socketcon){
    char buffer[MAXLEN];
    int buffer_length;
    int socketcon1;

    socketcon1=*((int*)socketcon);
    while(1){
        memset(buffer,'\0',sizeof(buffer));//或者使用函数bzero(buffer,20);

        printf("接收套接字：%d\n",socketcon1);
        buffer_length=read(socketcon1,buffer,MAXLEN-1);
        if(buffer_length<0)
        {
                if(write_to_file("read from client error")==0)
        {
            exit(-1);
        }
        exit(-1);
        }/*客户端的退出没有捕获到,此DUG待处理*/
        else if(strncmp(buffer,"NULL",4)==0)
        {
        fprintf(stdout,"套接字:%d close\n",socketcon1);
        exit_thr(&socketcon1);
        arr--;
            pthread_exit("线程退出!");   
        break;
        }
        if(strncmp(buffer,"quit",4)==0)
        {
            fprintf(stdout,"套接字:%d close\n",socketcon1);
            exit_thr(&socketcon1);
        arr--;
            pthread_exit("线程退出!");
        break;

        }
        //printf("buffer:%s\n",buffer);
        buffer[buffer_length]='\0';
        printf("客户端%d:%s\n",socketcon1,buffer);

    /*------------------------这是通过服务端向每个客户端写入数据-------------------*/
    int i=0;

    for(i=1;i<=arr;i++)
        {
        if(arrconsocket[i].socketcon!=socketcon1)
        {
    /*------------------------这是传递客户端的名字-------------------------------*/
            char client_name[50];
            sprintf(client_name,"中转%s--%d--->%s",arrconsocket[i].ipaddr,arrconsocket[i].port,buffer);

    /*------------------------------------------------------------------------*/

            int sendmsg_len=write(arrconsocket[i].socketcon,client_name,sizeof(client_name));
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

    /*------------------------------------------------------------------------*/
        sleep(1);
    }
     printf("接受数据线程结束\n");
}

//处理acceot
void *fun_thraccepthander(void *socketlisten){
    char buf[MAXLEN];
    ssize_t size;
    int sockaddr_in_size=sizeof(struct sockaddr_in);//sockaddr_in结构体的大小
    int socklisten1=*((int*)socketlisten);
    int socketcon;
    pthread_t thrreceive=0;
    struct sockaddr_in client_addr;
    char accept_message[50];

    while(1){
    socketcon=accept(socklisten1,(struct sockaddr*)(&client_addr),(socklen_t *)(&sockaddr_in_size));
    if(socketcon<0)
    {
    if(write_to_file("accept error")==0)
    {
        exit(-1);
    }
    }   
    else{
    if(write_to_file("accept success")==0)
    {
        exit(-1);
    }

    sprintf(accept_message,"ip: %s,port: %d",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        if(write_to_file(accept_message)==0)
    {
        exit(-1);
    }
    }

    printf("在线人数:%d\n",++arr);
    printf("连接客户端的套接字:%d\n",socketcon);

    arrconsocket[arr].socketcon=socketcon;
    arrconsocket[arr].ipaddr=inet_ntoa(client_addr.sin_addr);
    arrconsocket[arr].port=client_addr.sin_port;

    //接收的消息 
    if(pthread_create(&thrreceive,NULL,fun_thrreceivehandler,&socketcon)!=0)
    {
    if(write_to_file("pthread_create error")==0)
    {
        exit(-1);
    }
        break;
    }
    arrthrreceiveclient[arr]=thrreceive;
    sleep(1);
    }

    char *message;
        int res=1;
        printf("等待接受的子线程退出\n");


        if((res=pthread_join(thrreceive,(void*)&message))==0)
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
