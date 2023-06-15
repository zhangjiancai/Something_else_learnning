void *fun_thraccepthander(void *socketlisten){
    char buf[MAXLEN];
    ssize_t size;
    int sockaddr_in_size = sizeof(struct sockaddr_in);
    int socklisten1 = *((int *)socketlisten);
    int socketcon;
    pthread_t thrreceive = 0;
    struct sockaddr_in client_addr;
    char accept_message[50];
    
    
    while (1){
        memset(buf, '\0', sizeof(buf));
         socketcon = accept(socklisten1, (struct sockaddr *)(&client_addr), (socklen_t *)(&sockaddr_in_size));
        if (socketcon < 0) {
            if (write_to_file("accept error") == 0) {
                exit(-1);
            }
        }else{

if (write_to_file("accept success") == 0) {
                exit(-1);
            }

            sprintf(accept_message, "ip: %s, port: %d", inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));
if (write_to_file(accept_message) == 0) {
                exit(-1);
            }
            printf("在线人数:%d\n", ++arr);
            printf("连接客户端的套接字:%d\n", socketcon);
 size = read(socketcon, buf, sizeof(buf));
                    if (write_to_file(buf) == 0) {
                    exit(-1);
                }
char* username = strtok(buf, ",");
                char* password = strtok(NULL, ",");
                char* sign = strtok(NULL, ",");
                                    if (write_to_file(username) == 0) {
                    exit(-1);
                }
                                    if (write_to_file(password) == 0) {
                    exit(-1);
                }
                                    if (write_to_file(sign) == 0) {
                    exit(-1);
                }
//zhengq 
    if (size <= 0){
        // 处理接收用户名错误
                    if (write_to_file("receive username error") == 0) {
                        exit(-1);
                    }

    }else{
        if(strcmp(sign, "1") == 0){
//
if (write_to_file("receive user login!") == 0)
            {
                exit(-1);
            }
mydb_init();
                    if (mydb_test(mysql, username, password) == 1){
                        const char* success_signal = "login_success";
                if (write(socketcon, success_signal, strlen(success_signal)) < 0) {
                // 处理发送请求失败
                if (write_to_file("send login_success error") == 0) {
                    exit(-1);
                }
        }
if (write_to_file("login success!") == 0)
            {
                exit(-1);
            }
            mydb_destory(mysql);
            //zhengque
             arrconsocket[arr].socketcon = socketcon;
                    arrconsocket[arr].ipaddr = inet_ntoa(client_addr.sin_addr);
                    arrconsocket[arr].port = client_addr.sin_port;
                    strcpy(arrconsocket[arr].username, username);
printf("arrconsocket[arr]%s" ,arrconsocket[arr].username);
                    // 创建线程处理消息
                    pthread_create(&thrreceive, NULL, fun_thrreceivehandler, &socketcon);
                    arrthrreceiveclient[arr] = thrreceive;

                    }
        }else if (strcmp(sign, "2") == 0){
//
 if (write_to_file(" receive user regist!") == 0)
            {
                exit(-1);
            }
                        //
                                mydb_init();
                                        if (mydb_insert(mysql, username, password) == 1)
        {
            if (write_to_file("register success!") == 0)
            {
                exit(-1);
            }
//pay attation            //mydb_destory(mysql);
        }else{
            //
if (write_to_file("register failed!") == 0)
            {
                exit(-1);
            }

        }
        }else{
            //
            if (write_to_file("login / register failed!") == 0)
            {
                exit(-1);
            }
        }
    }






        } //second
    } //while
}