#include <muduo/base/Logging.h>
#include <muduo/net/Endian.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>

#include <utility>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class TimeClient : noncopyable
{
public:
    TimeClient(EventLoop* loop, const InetAddress& server_addr)
        : loop_(loop)
        , client_(loop, server_addr, "TimeClient")
    {
        client_.setConnectionCallback(
            std::bind(&TimeClient::on_connection, this, _1)
        );
        client_.setMessageCallback(
            std::bind(&TimeClient::on_message, this, _1, _2, _3)
        );
    }

    void connect()
    {
        client_.connect();
    }

private:
    EventLoop* loop_;
    TcpClient client_;

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
        if (buf->readableBytes() >= sizeof(int32_t))
        {
            const void* data = buf->peek();
            int32_t be32 = *static_cast<const __int32_t*>(data);
            buf->retrieve(sizeof(__int32_t));
            time_t time = sockets::networkToHost32(be32);
            Timestamp ts(implicit_cast<__uint64_t>(time) * Timestamp::kMicroSecondsPerSecond);
            LOG_INFO << "Server time = " << time << ", " << ts.toFormattedString(); 
        }
        else
        {
            LOG_INFO << conn->name() << " no enough data " << buf->readableBytes()
                     << " at " << recv_time.toFormattedString();
        }
    }
};

int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc > 1)
    {
        EventLoop loop;
        InetAddress server_addr(argv[1], 2037);

        TimeClient time_client(&loop, server_addr);
        time_client.connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip\n", argv[0]);
    }
    return 0;
}