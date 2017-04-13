void *transmit_meg(void *sock_des);
/**
*Purpose  中转数据
*Params   
*   @void *sock_des  客户端标识参数
*Return  NULL
**/
void rec_message(int temp_sock_descriptor,char buf[]);
/**
*Purpose  接受数据
*Params   
*   @int temp_sock_descriptor  客户端标识
*   @char buf[]     接受缓冲区
**/

void send_message(int temp_sock_descriptor,char buf[]);
/**
*Purpose  发送数据
*Params   
*   @int temp_sock_descriptor  客户端标识
*   @char buf[]     发送缓冲区
**/