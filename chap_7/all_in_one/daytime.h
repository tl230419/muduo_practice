#ifndef DAYTIME_H
#define DAYTIME_H

#include <muduo/net/TcpServer.h>

class DaytimeServer
{
public:
    DaytimeServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listen_addr);
    
    void start();

private:
    void on_connection(const muduo::net::TcpConnectionPtr& conn);

    void on_message(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp time);

    muduo::net::TcpServer server_;
};

#endif // DAYTIME_H