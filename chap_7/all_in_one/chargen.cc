#include "chargen.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

ChargenServer::ChargenServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listen_addr,
                  bool print)
    : server_(loop, listen_addr, "ChargenServer")
    , transferred_(0)
    , start_time_(Timestamp::now())
{
    server_.setConnectionCallback(
        std::bind(&ChargenServer::on_connection, this, _1));
    server_.setMessageCallback(
        std::bind(&ChargenServer::on_message, this, _1, _2, _3));
    server_.setWriteCompleteCallback(
        std::bind(&ChargenServer::on_write_complete, this, _1));

    if (print)
    {
        loop->runEvery(3.0, std::bind(&ChargenServer::print_thoughput, this));
    }

    string line;
    for (int i = 33; i < 127; i++)
    {
        line.push_back(char(i));
    }
    line += line;

    for (size_t i = 0; i < 127-33; i++)
    {
        message_ += line.substr(i, 72) + '\n';
    }
}
    
void ChargenServer::start()
{
    server_.start();
}

void ChargenServer::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "ChargenServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        conn->setTcpNoDelay(true);
        conn->send(message_);
    }
}

void ChargenServer::on_message(const muduo::net::TcpConnectionPtr& conn,
                muduo::net::Buffer* buf,
                muduo::Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " discards " << msg.size()
           << " bytes received at " << time.toString();
}

void ChargenServer::on_write_complete(const muduo::net::TcpConnectionPtr& conn)
{
    transferred_ += message_.size();
    conn->send(message_);
}

void ChargenServer::print_thoughput()
{
    Timestamp end_time = Timestamp::now();
    double time = timeDifference(end_time, start_time_);
    printf("%4.3f MiB/s\n", static_cast<double>(transferred_)/time/1024/1024);
    transferred_ = 0;
    start_time_ = end_time;
}