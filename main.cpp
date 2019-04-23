/*
 * main.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: root
 */

#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include <stdio.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

std::string message;

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): tid=%d new connection [%s] from %s\n",
           muduo::CurrentThread::tid(),
           conn->name().c_str(),
           conn->peerAddress().toIpPort().c_str());
    conn->send(message);
  }
  else
  {
    printf("onConnection(): tid=%d connection [%s] is down\n",
           CurrentThread::tid(),
           conn->name().c_str());
  }
}

void onWriteComplete(const TcpConnectionPtr& conn)
{
//  conn->send(message);
}

void onMessage(const TcpConnectionPtr& conn,
        Buffer* buf,
        Timestamp receiveTime)
{
  printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
         CurrentThread::tid(),
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toFormattedString().c_str());
  printf("      ======> %s\n", buf->retrieveAllAsString().c_str());

  buf->retrieveAll();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
      printf("Usage:\n  %s threadnums \n", argv[0]);
      return 0;
    }
    printf("main(): pid = %d\n", getpid());

    //  std::string line;
    //  for (int i = 33; i < 127; ++i)
    //  {
    //    line.push_back(char(i));
    //  }
    //  line += line;
    //
    //  for (size_t i = 0; i < 127-33; ++i)
    //  {
    //    message += line.substr(i, 72) + '\n';
    //  }

    message = "hello, i am log server\n";

    InetAddress listenAddr(32006);
    EventLoop loop;

    TcpServer server(&loop, listenAddr, "LogServer");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setWriteCompleteCallback(onWriteComplete);
    if (argc > 1) {
    server.setThreadNum(atoi(argv[1]));
    }
    server.start();

    loop.loop();
}


