#include "chargen.h"
#include "daytime.h"
#include "discard.h"
#include "echo.h"
#include "time_server.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;  // one loop shared by multiple servers

    ChargenServer chargenServer(&loop, InetAddress(2019));
    chargenServer.start();

    DaytimeServer daytimeServer(&loop, InetAddress(2013));
    daytimeServer.start();

    DiscardServer discardServer(&loop, InetAddress(2009));
    discardServer.start();

    EchoServer echoServer(&loop, InetAddress(2007));
    echoServer.start();

    TimeServer timeServer(&loop, InetAddress(2037));
    timeServer.start();

    loop.loop();

    return 0;
}