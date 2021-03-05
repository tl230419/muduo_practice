#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>

#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class ChargenClient : noncopyable
{
public:
    ChargenClient(EventLoop* loop, const InetAddress& listen_addr)
        : loop_(loop)
        , client_(loop, listen_addr, "ChargenClient")
    {
        client_.setConnectionCallback(
            std::bind(&ChargenClient::on_connection, this, _1)
        );
        client_.setMessageCallback(
            std::bind(&ChargenClient::on_message, this, _1, _2, _3)
        );
    }

    void connect()
    {
        client_.connect();
    }

private:
    void on_connection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> "
                 << conn->peerAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");
        if (!conn->connected())
        {
            loop_->quit();
        }
    }

    void on_message(const TcpConnectionPtr& conn, Buffer* buf, Timestamp recv_time)
    {
        buf->retrieveAll();
    }

    EventLoop* loop_;
    TcpClient client_;
};

int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc > 1)
    {
        EventLoop loop;
        InetAddress server_addr(argv[1], 2019);

        ChargenClient chargen_client(&loop, server_addr);
        chargen_client.connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip\n", argv[0]);
    }
    return 0;
}