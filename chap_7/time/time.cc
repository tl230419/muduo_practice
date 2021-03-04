#include "time.h"

#include <muduo/base/Logging.h>
#include <muduo/net/Endian.h>

#include <sys/time.h>

using namespace muduo;
using namespace muduo::net;

TimeServer::TimeServer(muduo::net::EventLoop* loop,
              const muduo::net::InetAddress& listen_addr)
    : server_(loop, listen_addr, "TimeServer")
{
    server_.setConnectionCallback(
        std::bind(&TimeServer::on_connection, this, _1));
    server_.setMessageCallback(
        std::bind(&TimeServer::on_message, this, _1, _2, _3));
}

void TimeServer::start()
{
    server_.start();
}

void TimeServer::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "TimeServer - " << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        time_t now = time(NULL);
        int32_t be32 = sockets::hostToNetwork32(static_cast<int32_t>(now));
        conn->send(&be32, sizeof be32);
        conn->shutdown();
    }
}

void TimeServer::on_message(const muduo::net::TcpConnectionPtr& conn,
                muduo::net::Buffer* buf,
                muduo::Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " discads " << msg.size()
             << " bytes received at " << time.toString();
}