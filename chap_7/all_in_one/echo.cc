#include "echo.h"

#include <muduo/base/Logging.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


EchoServer::EchoServer(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listen_addr)
    : server_(loop, listen_addr, "EchoServer")
{
    server_.setConnectionCallback(
        std::bind(&EchoServer::on_connection, this, _1));
    server_.setMessageCallback(
        std::bind(&EchoServer::on_message, this, _1, _2, _3));
}

void EchoServer::start()
{
    server_.start();
}

void EchoServer::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::on_message(const muduo::net::TcpConnectionPtr& conn,
                muduo::net::Buffer* buf,
                muduo::Timestamp time)
{
    muduo::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
             << "data received at " << time.toString();
    conn->send(msg);
}