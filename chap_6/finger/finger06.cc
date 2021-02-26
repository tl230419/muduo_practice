#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <map>

typedef std::map<std::string, std::string> UserMap;
UserMap users;
using namespace muduo;
using namespace muduo::net;

std::string get_user(const std::string& user)
{
    std::string result = "No such user";
    UserMap::iterator it = users.find(user);
    if (it != users.end())
    {
        result = it->second;
    }

    return result;
}

void OnMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp recv_time)
{
    const char* crlf = buf->findCRLF();
    if (crlf)
    {
        std::string user(buf->peek(), crlf);
        conn->send(get_user(user) + "\r\n");
        buf->retrieveUntil(crlf + 2);
        conn->shutdown();
    }
}

int main()
{
    users["schen"] = "Happy and well";
    EventLoop loop;
    TcpServer server(&loop, InetAddress(1079), "Finger");
    server.setMessageCallback(OnMessage);
    server.start();
    loop.loop();
    return 0;
}