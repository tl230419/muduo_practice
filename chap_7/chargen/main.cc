#include "chargen.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <unistd.h>

int main()
{
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listen_addr(2019);
    ChargenServer server(&loop, listen_addr, true);
    server.start();
    loop.loop();
    return 0;
}