#include "daytime.h"

#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

DaytimeServer::DaytimeServer(muduo::net::EventLoop* loop,
              const muduo::net::InetAddress& listen_addr)
    : server_(loop, listen_addr, "DaytimeServer")
{
    server_.setConnectionCallback(
        std::bind(&DaytimeServer::on_connection, this, _1));
    server_.setMessageCallback(
        std::bind(&DaytimeServer::on_message, this, _1, _2, _3));
}

void DaytimeServer::start()
{
    server_.start();
}

void DaytimeServer::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "DaytimeServer - " << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        conn->send(Timestamp::now().toFormattedString() + "\r\n");
        conn->shutdown();
    }
}

void DaytimeServer::on_message(const muduo::net::TcpConnectionPtr& conn,
                muduo::net::Buffer* buf,
                muduo::Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " discads " << msg.size()
             << " bytes received at " << time.toString();
}