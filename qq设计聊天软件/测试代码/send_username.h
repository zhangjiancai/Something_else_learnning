
void *send_username(void *buffer){

Message *msg = (Message *)malloc(sizeof(Message));

strcpy(msg->sender, strtok(buffer, ","));
strcpy(msg->receiver, strtok(NULL, ","));
strcpy(msg->message, strtok(NULL, ","));
/*
// 解析接收到的消息
Message msg;
memcpy(&msg.sender, strtok(buffer, ","), 50);
memcpy(&msg.receiver, strtok(NULL, ","), 50);
memcpy(&msg.message, strtok(NULL, ","), MAXLEN);
*/
/*
		char* sender = strtok(buffer, ",");
                char* receiver = strtok(NULL, ",");
                char* message = strtok(NULL, ",");
*/
/*
strncpy(msg.sender, strtok(buffer, ","), sizeof(msg.sender) - 1);
msg.sender[sizeof(msg.sender) - 1] = '\0';  // 确保以 null 结尾

strncpy(msg.receiver, strtok(NULL, ","), sizeof(msg.receiver) - 1);
msg.receiver[sizeof(msg.receiver) - 1] = '\0';  // 确保以 null 结尾
//char* password = strtok(NULL, ",");
strncpy(msg.message, strtok(NULL, ","), sizeof(msg.message) - 1);
msg.message[sizeof(msg.message) - 1] = '\0';  // 确保以 null 结尾
//char* sign = strtok(NULL, ",");

                                    if (write_to_file(sender) == 0) {
                    exit(-1);
                }
                                    if (write_to_file(receiver) == 0) {
                    exit(-1);
                }
                                    if (write_to_file(message) == 0) {
                    exit(-1);
                }
                */
// 查找接收者并发送消息
int i;

for (i = 1; i <= arr; i++) {
    if (strcmp(arrconsocket[i].username, msg->receiver) == 0) {
        // 找到接收者，发送消息
        int sendmsg_len = write(arrconsocket[i].socketcon, msg, sizeof(Message));
        if (sendmsg_len > 0) {
            printf("向客户端%s:%d发送成功\n", arrconsocket[i].username, arrconsocket[i].port);
        } else {
            printf("向客户端%s:%d发送失败\n", arrconsocket[i].username, arrconsocket[i].port);
        }
        break;
    }
    
    
} 
/*
for (i = 1; i <= arr; i++) {
    if (strcmp(arrconsocket[i].username, receiver) == 0) {
        // 找到接收者，发送消息
        int sendmsg_len = write(arrconsocket[i].socketcon, message, sizeof(message));
        if (sendmsg_len > 0) {
            printf("向客户端%s:%d发送成功\n", arrconsocket[i].username, arrconsocket[i].port);
        } else {
            printf("向客户端%s:%d发送失败\n", arrconsocket[i].username, arrconsocket[i].port);
        }
        break;
    }
    
    
} */
}
