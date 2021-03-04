#include "daytime.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listen_addr(2013);
    DaytimeServer server(&loop, listen_addr);
    server.start();
    loop.loop();
    return 0;
}