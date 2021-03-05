#include "echo.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <unistd.h>

int main()
{
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listen_addr(2007);
    EchoServer server(&loop, listen_addr);
    server.start();
    loop.loop();
    return 0;
}