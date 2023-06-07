#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "client.h"
const gchar *username; // 全局变量声明
void closeApp(GtkWidget *window,gpointer data)
{
    int sendmsg_len=write(socketcon,"quit",strlen("quit"));
    if(sendmsg_len<=0)
    {
        if(write_to_file("write error")==0)
        {
            exit(-1);
        }
        exit(-1);
    }
    pthread_kill(client_thr,SIGKILL);
    close(socketcon);
    gtk_main_quit();
}

void Clear_Local_message()
{
        GtkTextIter start,end;
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(Send_buffer),&start,&end);/*删除缓冲区内容*/
}

/*----------------------------------------------------------------------------*/
void Put_Local_message(const gchar *text)
{
        GtkTextIter start,end;
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Rcv_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
    // 构建要插入的字符串
//char* text = g_strdup_printf("username:%s\n", argv[1]);

// 插入文本到缓冲区
//gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer), &end, text, -1);

// 释放字符串内存
//g_free(text);
    //gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,"username:\n",10);/*插入文本到缓冲区*/
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,text,strlen(text));/*插入文本到缓冲区*/
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,"\n",1);/*插入文本到缓冲区*/
    int sendmsg_len=write(socketcon,text,strlen(text));
    if(sendmsg_len<=0)
    {
        if(write_to_file("write error")==0)
        {
            exit(-1);
        }
        exit(-1);
    }
}

/*----------------------------------------------------------------------------*/
void Show_Err(char *err)
{
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Rcv_buffer),&start,&end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(Rcv_buffer),&end,err,strlen(err));
}

/*----------------------------------------------------------------------------*/
/*发送函数----------------------------------------------------------------------*/
void on_send(GtkButton *SaveButton, GtkWidget *Send_textview)/*保存按钮的回调函数，每当‘保存’按钮被按下的时候，都会触发这个函数*/
{
    GtkTextIter start,end;/*定义迭代器起点终点*/
    gchar *S_text,*R_text;/*定义文字存储变量*/

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
    S_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&start,&end,FALSE);

    R_text = S_text;/*把发送文本交换到接收文本*/

    if(strcmp(R_text,"")!=0)
    {
        //Get_Local_message();
        Clear_Local_message();        
        Put_Local_message(username); 
        Put_Local_message(R_text);
  
    }
    else
    {
            Show_Err("消息不能为空...!\n");
    }
    free(R_text);
    //free(S_text);
}
/*----------------------------------------------------------------------------*/

/*关闭函数---------------------------------------------------------------------*/
void on_close(GtkButton *CloseButton,GtkWidget *Send_textview)
{
    int sendmsg_len=write(socketcon,"quit",strlen("quit"));
    if(sendmsg_len<=0)
    {
        if(write_to_file("write error")==0)
        {
            exit(-1);
        }
        exit(-1);
    }
    pthread_kill(client_thr,SIGKILL);
    close(socketcon);
    gtk_main_quit();
}
/*----------------------------------------------------------------------------*/

/*主函数-----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{


    if (argc < 2) {
        g_print("Usage: %s <username>\n", argv[0]);
        return 1;
    }
 username = argv[1];


/*主函数变量声明区*/
    GtkWidget *window/*定义主窗口*/,
          *Send_scrolled_win/*定义发送滚动窗口*/,
          *Rcv_scrolled_win/*定义接收滚动窗口*/;
    GtkWidget *vbox/*定义垂直盒子*/;
    GtkWidget *Button_Box/*定义按钮盒*/,
          *SendButton/*定义发送按钮*/,
          *CloseButton/*定义关闭按钮*/;
    GtkWidget *hseparator/*定义水平分割线*/;
          //*panel_v/*定义垂直分割面板*/,
          //*panel_h/*定义水平分割面板*/;

    /*------------------------------连接服务端------------------------*/

    if(write_to_file("start socket")==0)
    {
        exit(-1);
    }

    socketcon=socket(AF_INET,SOCK_STREAM,0);
    if(socketcon<0)
    {
        if(write_to_file("socket error")==0)
    {
        exit(-1);
    }
        exit(-1);
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(ip);
    server_addr.sin_port=htons(port);

    /*测试*/
    printf("客户端的套接字:%d\n",socketcon);

    int res_con=connect(socketcon,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr));
    if(res_con!=0)
    {
        if(write_to_file("connect error")==0)
    {
        exit(-1);
    }
    exit(-1);
    }

    printf("连接成功!");
    if(write_to_file("连接成功")==0)
    {
        exit(-1);
    }

    /*---------------------------------------------------------------*/ 

    /*--------------------客户端的接受消息的线程函数的执行----------------*/

    if(pthread_create(&client_thr,NULL,fun_clientreceive,&socketcon)!=0)
    {
        if(write_to_file("pthread_create error")==0)
            {
            exit(-1);
            }
        exit(-1);
    }
    sleep(1);
    /*---------------------------------------------------------------*/

    /*-------------------------这是关于窗口的初始化---------------------*/        
    gtk_init(&argc,&argv);/*GTK初始化*/

    /*------------------------------绘制主窗口----------------------------*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);/*生成主窗口*/
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(closeApp),NULL);
    //g_singal_connect(G_OBJECT(window),"destroy",G_CALLBACK(closeApp),NULL);
   // g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);/*连接信号，关闭窗口*/
    gtk_window_set_title(GTK_WINDOW(window),"聊天窗口");/*设置主窗口标题*/
    gtk_container_set_border_width(GTK_CONTAINER(window),10);/*设置主窗口边框*/
    gtk_widget_set_size_request(window,400,300);/*设置主窗口初始化大小*/
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);/*设置主窗口初始位置*/

    /*------------------------------设置Send_text view-------------------------*/
    Send_textview = gtk_text_view_new();/*生成text view*/
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(Send_textview),GTK_WRAP_WORD);/*处理多行显示的模式*/
    gtk_text_view_set_justification(GTK_TEXT_VIEW(Send_textview),GTK_JUSTIFY_LEFT);/*控制文字显示方向的,对齐方式*/
    gtk_text_view_set_editable(GTK_TEXT_VIEW(Send_textview),TRUE);/*允许text view内容修改*/
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Send_textview),TRUE);/*设置光标可见*/
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(Send_textview),5);/*设置上行距*/
    gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(Send_textview),5);/*设置下行距*/
    gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(Send_textview),5);/*设置词距*/
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(Send_textview),10);/*设置左边距*/
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(Send_textview),10);/*设置右边距*/
    Send_buffer =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(Send_textview));/*返回text view被显示的buffer*/

        /*------------------------------设置Rcv_text view-------------------------*/
    Rcv_textview = gtk_text_view_new();/*生成text view*/
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(Rcv_textview),GTK_WRAP_WORD);/*处理多行显示的模式*/
    gtk_text_view_set_justification(GTK_TEXT_VIEW(Rcv_textview),GTK_JUSTIFY_LEFT);/*控制文字显示方向的,对齐方式*/
    gtk_text_view_set_editable(GTK_TEXT_VIEW(Rcv_textview),TRUE);/*允许text view内容修改*/
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Rcv_textview),TRUE);/*设置光标可见*/
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(Rcv_textview),5);/*设置上行距*/
    gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(Rcv_textview),5);/*设置下行距*/
    gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(Rcv_textview),5);/*设置词距*/
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(Rcv_textview),10);/*设置左边距*/
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(Rcv_textview),10);/*设置右边距*/
    gtk_text_view_set_editable(GTK_TEXT_VIEW(Rcv_textview),FALSE);/*设置接收文字区不可被编辑*/
    Rcv_buffer =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(Rcv_textview));/*返回text view被显示的buffer*/   

    /*------------------------------设置发送窗口滚动条-------------------------------*/
    Send_scrolled_win = gtk_scrolled_window_new(NULL,NULL);/*生成滚动条的窗口*/
    gtk_container_add(GTK_CONTAINER(Send_scrolled_win), Send_textview);
    //gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(Send_scrolled_win),Send_textview);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Send_scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);/*滚动条属性*/

    /*------------------------------设置接收窗口滚动条-------------------------------*/
    Rcv_scrolled_win = gtk_scrolled_window_new(NULL,NULL);/*生成滚动条的窗口*/
    gtk_container_add(GTK_CONTAINER(Rcv_scrolled_win), Rcv_textview);
    //gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(Rcv_scrolled_win),Rcv_textview);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Rcv_scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);/*滚动条属性*/

    /*------------------------------设置垂直盒子------------------------------*/
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);/*生成一个垂直排布的盒子*/

    /*------------------------------设置发送按钮------------------------------*/
    SendButton = gtk_button_new_with_label("发送");/*生成发送按钮*/
    g_signal_connect(G_OBJECT(SendButton),"clicked",G_CALLBACK(on_send),(gpointer)Send_textview);/*给按钮加上回调函数*/

    /*------------------------------设置关闭按钮------------------------------*/
    CloseButton = gtk_button_new_with_label("关闭");/*生成关闭按钮*/
    g_signal_connect(G_OBJECT(CloseButton),"clicked",G_CALLBACK(on_close),(gpointer)Send_textview);

    /*------------------------------设置按钮盒子------------------------------*/    
    Button_Box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);/*生成按钮盒*/
    gtk_box_set_spacing(GTK_BOX(Button_Box),1);/*按钮之间的间隔*/
    gtk_button_box_set_layout(GTK_BUTTON_BOX(Button_Box),GTK_BUTTONBOX_END);/*按钮盒内部布局，风格是尾对齐*/
    gtk_container_set_border_width(GTK_CONTAINER(Button_Box),5);/*边框宽*/

    /*------------------------------设置分割线--------------------------------*/
    hseparator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    /*------------------------------添加到容器--------------------------------*/
    gtk_container_add(GTK_CONTAINER(vbox),Rcv_scrolled_win);/*包装滚动条窗口到主窗口*/
    gtk_container_add(GTK_CONTAINER(vbox),hseparator);/*加入一条分割线*/
    gtk_container_add(GTK_CONTAINER(vbox),Send_scrolled_win);/*包装滚动条窗口到主窗口*/   
    gtk_container_add(GTK_CONTAINER(vbox),Button_Box);/*把按钮盒包装到vbox中*/
    gtk_box_pack_start(GTK_BOX(Button_Box),CloseButton,TRUE,TRUE,5);/*把关闭按钮包装到按钮盒里面去*/
    gtk_box_pack_start(GTK_BOX(Button_Box),SendButton,TRUE,TRUE,5);/*把发送按钮包装到按钮盒里面去*/
    gtk_container_add(GTK_CONTAINER(window),vbox);/*将盒子封装到主窗口中去*/   

    /*------------------------------显示所有东西------------------------------*/
    gtk_widget_show_all(window);/*显示所有东西*/

    gtk_main();/*主循环*/

    /*-----------------------------等待线程退出-------------------------------*/
    char *client_message;
    int res=1;

    if(pthread_join(client_thr,(void *)&client_message)==0)
    {
        printf("%s\n",client_message);
    }
    else
    {
        if(write_to_file("pthread_join error")==0)
        {
            exit(-1);
        }
        exit(-1);
    }

    /*----------------------------------------------------------------------*/
    pthread_kill(client_thr,SIGKILL);
    return 0;/*退出程序*/
    }

