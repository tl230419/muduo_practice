#ifndef CHARGEN_H
#define CHARGEN_H

#include <muduo/net/TcpServer.h>

class ChargenServer
{
public:
    ChargenServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listen_addr,
                  bool print = false);
    
    void start();

private:
    void on_connection(const muduo::net::TcpConnectionPtr& conn);
    void on_message(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp time);

    void on_write_complete(const muduo::net::TcpConnectionPtr& conn);
    void print_thoughput();

    muduo::net::TcpServer server_;
    muduo::string message_;
    int64_t transferred_;
    muduo::Timestamp start_time_;
};

#endif // CHARGEN_H