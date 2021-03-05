#ifndef TIME_SERVER_H
#define TIME_SERVER_H

#include <muduo/net/TcpServer.h>

class TimeServer
{
public:
    TimeServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listen_addr);
    
    void start();

private:
    void on_connection(const muduo::net::TcpConnectionPtr& conn);

    void on_message(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp time);

    muduo::net::TcpServer server_;
};

#endif // TIME_SERVER_H