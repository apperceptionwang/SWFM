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
#include "lib/pthreadpool.h"
#include "lib/transmitmeg.h"

const int thread_num = 16;
const int queue_max_num = 128;

int port =8000;


void *rec_message(void *sock_des);

/*服务端*/
int main(int argc, char** argv) 
{
    struct sockaddr_in sin;//struct sockaddr和struct sockaddr_in这两个结构体用来处理网络通信的地址。
    struct sockaddr_in pin;
    int sock_descriptor;//  套接口描述字
    int temp_sock_descriptor;
    socklen_t address_size;

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

    address_size = sizeof(sockaddr_in);
    printf("accepting connections  \n");
    struct thread_pool *pool = thread_pool_init(thread_num,queue_max_num);

    while(1)
    {   
        //用来监听的端口sock_descriptor
        temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr *)&pin, &address_size);
        if(temp_sock_descriptor== -1)
        {
           perror("call to accept");
           continue ;
        }
        thread_add_job(pool, temp_sock_descriptor,&transmit_meg);
    }

    return (EXIT_SUCCESS);
}