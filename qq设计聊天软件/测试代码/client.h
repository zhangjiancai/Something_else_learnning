#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <gtk/gtk.h>

#define MAXLEN 1024

typedef struct mysocketinfo{
    int socketcon;
    unsigned long ipaddr;
    unsigned short port;
}_mysocketinfo;

const char *ip="127.0.0.1";
const int port=8888;

int socketcon;

/*---------------------------------客户端接受线程---------------------------------*/
pthread_t client_thr;
/*------------------------------------------------------------------------------*/

/*全局变量声明区-----------------------------------------------------------------*/
    GtkWidget *Send_textview/*定义发送文本区*/,
            *Rcv_textview/*定义接收文本区*/;
    GtkTextBuffer *Send_buffer/*定义发送文本缓冲区*/,
                *Rcv_buffer/*定义接收文本缓冲区*/;

/*----------------------------------------------------------------------------*/

int write_to_file(char *name)
{   
    FILE *fp;
    char buffer[50];
    time_t timep;
    char time_buffer[100];

    fp=fopen("client.log","a+");
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

void *fun_clientreceive(void *_socketcon)
{
    char buffer[MAXLEN];
    int socketcon=*((int *)_socketcon);

    while(1)
    {
        memset(buffer,'\0',sizeof(buffer));
        int buffer_length=read(socketcon,buffer,MAXLEN-1);
        if(buffer_length<=0)
        {
            if(write_to_file("client read error")==0)
                {
                exit(-1);
                }
            exit(-1);
        }
        else
        {
            GtkTextIter start,end;/*定义迭代器起点终点*/
                gchar *S_text,*R_text;/*定义文字存储变量*/

                gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Rcv_buffer),&start,&end);

            gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,"服务器:\n",10);/*插入文本到缓冲区*/
                gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,buffer,strlen(buffer));/*插入文本到缓冲区*/
                gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,"\n",1);/*插入文本到缓冲区*/
        }
    }
    pthread_exit("线程退出!");
}

