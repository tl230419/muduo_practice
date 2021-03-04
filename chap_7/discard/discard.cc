#include "discard.h"

#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

DiscardServer::DiscardServer(muduo::net::EventLoop* loop,
              const muduo::net::InetAddress& listen_addr)
    : server_(loop, listen_addr, "DiscardServer")
{
    server_.setConnectionCallback(
        std::bind(&DiscardServer::on_connection, this, _1));
    server_.setMessageCallback(
        std::bind(&DiscardServer::on_message, this, _1, _2, _3));
}

void DiscardServer::start()
{
    server_.start();
}

void DiscardServer::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "DiscardServer - " << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
}

void DiscardServer::on_message(const muduo::net::TcpConnectionPtr& conn,
                muduo::net::Buffer* buf,
                muduo::Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " discads " << msg.size()
             << " bytes received at " << time.toString();
}