#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "lib/transmitmeg.h"

void *transmit_meg(void *sock_des)
{
    char recv_buf[1024];//接受缓冲区
    char send_buf[1024];//发送缓冲区
    int temp_sock_descriptor = *(int*)sock_des;
    rec_message(temp_sock_descriptor,recv_buf);
    printf("%s\n",recv_buf);
    send_message(temp_sock_descriptor,send_buf);

    return NULL;
}

void rec_message(int temp_sock_descriptor,char buf[])
{
    int recount = recv(temp_sock_descriptor,buf,1024,0);
    if(recount ==-1)
    {
        printf("客户端下线");
        return ;
    }
    if(recount == 0)
    {
        printf("网络中断\n");
        return ;
    }
    if(recount >= 1024)
    {
        recount = 1023;
    }
    buf[recount] = '\0';

        /*int PASCAL FAR recv( SOCKET s, char FAR* buf, int len, int flags);
　　        s：一个标识已连接套接口的描述字。
　　        buf：用于接收数据的缓冲区。
　　        len：缓冲区长度。
　　        flags：指定调用方式。
         */
        
   
    return ;
}

void send_message(int temp_sock_descriptor,char buf[])
{
     strcpy(buf,"I get it");

    if(send(temp_sock_descriptor,buf,strlen(buf),0) == -1)
    {
        perror("call to send");
        return ;
    }
    close(temp_sock_descriptor);
    return ;
}