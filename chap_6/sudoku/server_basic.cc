#include "sudoku.h"

#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

#include <utility>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class SudokuServer
{
public:
    SudokuServer(EventLoop* loop, const InetAddress& listen_addr)
        : server_(loop, listen_addr, "SudokuServer")
        , start_time_(Timestamp::now())
    {
        server_.setConnectionCallback(
            std::bind(&SudokuServer::on_connection, this, _1));
        server_.setMessageCallback(
            std::bind(&SudokuServer::on_message, this, _1, _2, _3));
    }

    void start()
    {
        server_.start();
    }

private:
    void on_connection(const TcpConnectionPtr& conn)
    {
        LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "Up" : "Down");
    }

    void on_message(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        LOG_TRACE << conn->name();
        size_t len = buf->readableBytes();
        while (len >= kCells + 2)
        {
            const char* crlf = buf->findCRLF();
            if (crlf)
            {
                string request(buf->peek(), crlf);
                buf->retrieveUntil(crlf + 2);
                len = buf->readableBytes();
                if (!process_request(conn, request))
                {
                    conn->send("Bad Request!\r\n");
                    conn->shutdown();
                    break;
                }
            }
            else if (len > 100)
            {
                conn->send("Id too long!\r\n");
                conn->shutdown();
                break;
            }
            else
            {
                break;
            }
        }
    }

    bool process_request(const TcpConnectionPtr& conn, const string& request)
    {
        string id;
        string puzzle;
        bool good_request = true;

        string::const_iterator colon = find(request.begin(), request.end(), ':');
        if (colon != request.end())
        {
            id.assign(request.begin(), colon);
            puzzle.assign(colon + 1, request.end());
        }
        else
        {
            puzzle = request;
        }

        if (puzzle.size() == implicit_cast<size_t>(kCells))
        {
            LOG_DEBUG << conn->name();
            string result = solveSudoku(puzzle);
            if (id.empty())
            {
                conn->send(result + "\r\n");
            }
            else
            {
                conn->send(id + ":" + result + "\r\n");
            }
        }
        else
        {
            good_request = false;
        }

        return good_request;
    }

    TcpServer server_;
    Timestamp start_time_;
};

int main()
{
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    EventLoop loop;
    InetAddress listen_addr(9981);
    SudokuServer server(&loop, listen_addr);

    server.start();
    loop.loop();
    return 0;
}