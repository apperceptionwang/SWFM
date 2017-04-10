#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> //互联网地址族
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int port =8000;


void *rec_message(void *sock_des)
{
    //printf("客户端上线\n");
    int temp_sock_descriptor = *(int*)sock_des;
    char buf[1024];// 缓冲区大小
    while(1)
    {
        if(recv(temp_sock_descriptor,buf,1024,0) ==-1)
        {
            printf("客户端下线");
            return ((void *)0);
        }
        printf("received from client:%s\n",buf);
    }
    return ((void *)0);
}

int find_alive_thread();
/*服务端*/
int main(int argc, char** argv) {

    struct sockaddr_in sin;//struct sockaddr和struct sockaddr_in这两个结构体用来处理网络通信的地址。
    struct sockaddr_in pin;
    int sock_descriptor;//  套接口描述字
    int temp_sock_descriptor;
    socklen_t address_size;
    

    int i,len;

    /*
     *int socket(int domain, int type, int protocol);
     * PF_INET, AF_INET： Ipv4网络协议
     * PF_INET6, AF_INET6： Ipv6网络协议。
     * type参数的作用是设置通信的协议类型，可能的取值如下所示：
　　      SOCK_STREAM： 提供面向连接的稳定数据传输，即TCP协议。
　　      OOB： 在所有数据传送前必须使用connect()来建立连接状态。
　      　SOCK_DGRAM： 使用不连续不可靠的数据包连接。
　　      SOCK_SEQPACKET： 提供连续可靠的数据包连接。
　      　SOCK_RAW： 提供原始网络协议存取。
　      　SOCK_RDM： 提供可靠的数据包连接。
　　      SOCK_PACKET： 与网络驱动程序直接通信。
     */
    //socket函数，向系统申请一个通信端口
    sock_descriptor=socket(AF_INET,SOCK_STREAM,0);//IPV4 TCP协议
    if(sock_descriptor== -1)//申请失败
    {
        perror("call to socket");
        exit(1);
    }

    bzero(&sin,sizeof(sin));// 初始化 然后是设置套接字
    sin.sin_family = AF_INET;//协议族，在socket编程中只能是AF_INET(TCP/IP协议族)
    sin.sin_addr.s_addr=INADDR_ANY;//sin_addr存储IP地址,使用in_addr这个数据结构
                                  //s_addr按照网络字节顺序存储IP地址
                                   //in_addr32位的IPv4地址
    sin.sin_port=htons(port);//存储端口号

      //将套接字（sin） 跟端口（sock_descriptor）链接
    if(bind(sock_descriptor,(struct sockaddr *)&sin,sizeof(sin)) ==-1)
    {
        perror("call to bind");
        exit(1);
    }
    /*int PASCAL FAR listen( SOCKET s, int backlog);
　　     S：用于标识一个已捆绑未连接套接口的描述字。
　　     backlog：等待连接队列的最大长度。
     * listen()仅适用于支持连接的套接口，如SOCK_STREAM类型的。
     */
    if(listen(sock_descriptor,20) == -1) //在端口sock_descriptor监听
    {
        perror("call to listen");
        exit(1);
    }

    printf("accepting connections  \n");

    while(1)
    {   
        //用来监听的端口sock_descriptor
        
        temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr *)&pin, &address_size);
        if(temp_sock_descriptor== -1)
        {
           perror("call to accept");
           continue ;
        }
        

        char buf[1024];// 缓冲区大小
        int recount = recv(temp_sock_descriptor,buf,1024,0);
        if(recount ==-1)
        {
            printf("客户端下线");
            continue ;
        }
        if(recount == 0)
        {
            printf("网络中断");
            continue ;
        }
        if(recount >= 1024)
        {
            recount = 1023;
        }
        buf[recount] = '\0';

        printf("received from client:%s\n",buf);

        /*int PASCAL FAR recv( SOCKET s, char FAR* buf, int len, int flags);
　　        s：一个标识已连接套接口的描述字。
　　        buf：用于接收数据的缓冲区。
　　        len：缓冲区长度。
　　        flags：指定调用方式。
         */
        
    }

    return (EXIT_SUCCESS);
}