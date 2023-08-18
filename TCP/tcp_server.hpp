#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "log.hpp"

static void service(int sock, const std::string &clientip, const uint16_t &clientport)
{
    //echo server
    char buffer[1024];
    while(true)
    {
        // read && write 可以直接被使用！
        ssize_t s = read(sock, buffer, sizeof(buffer)-1);
        if(s > 0)
        {
            buffer[s] = 0; //将发过来的数据当做字符串
            std::cout << clientip << ":" << clientport << "# " << buffer << std::endl;
        }
        else if(s == 0) //对端关闭连接
        {
            logMessage(NORMAL, "%s:%d shutdown, me too!", clientip.c_str(), clientport);
            break;
        }
        else{ // 
            logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
            break;
        }

        write(sock, buffer, strlen(buffer));
    }
}

class TcpServer
{
private:
    const static int gbacklog = 20;  //后面再说
public:
    TcpServer(uint16_t port, std::string ip=""):listensock(-1), _port(port), _ip(ip)
    {}
    void initServer()
    {
        // 1. 创建socket -- 进程和文件
        listensock = socket(AF_INET, SOCK_STREAM, 0);
        if(listensock < 0)
        {
            logMessage(FATAL, "create socket error, %d:%s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "create socket success, listensock: %d", listensock); // 3

        // 2. bind -- 文件 + 网络
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
        if(bind(listensock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind error, %d:%s", errno, strerror(errno));
            exit(3);
        }

        // 3. 因为TCP是面向连接的，当我们正式通信的时候，需要先建立连接
        if(listen(listensock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(4);
        }

        logMessage(NORMAL, "init server success");
    }
    void start()
    {
        signal(SIGCHLD, SIG_IGN); // 对SIGCHLD，主动忽略SIGCHLD信号，子进程退出的时候，会自动释放自己的僵尸状态
        while(true)
        {
            // sleep(1);
            // 4. 获取连接
            struct sockaddr_in src;
            socklen_t len = sizeof(src);
            // fd(李四，王五等提供服务的服务员) vs _sock(张三 --- 获取新连接)
            int servicesock = accept(listensock, (struct sockaddr*)&src, &len);
            if(servicesock < 0)
            {
                logMessage(ERROR, "accept error, %d:%s", errno, strerror(errno));
                continue;
            }
            // 获取连接成功了
            uint16_t client_port = ntohs(src.sin_port);
            std::string client_ip = inet_ntoa(src.sin_addr);
            logMessage(NORMAL, "link success, servicesock: %d | %s : %d |\n",\
                servicesock, client_ip.c_str(), client_port);
            // 开始进行通信服务啦
            // version 1 -- 单进程循环版 -- 只能够进行一次处理一个客户端，处理完了一个，才能处理下一个
            // 很显然，是不能够直接被使用的！ -- 为什么？ 单进程
            // service(servicesock, client_ip, client_port);

            // version 2.0 -- 多进程版 --- 创建子进程
            // 让子进程给新的连接提供服务，子进程能不能打开父进程曾经打开的文件fd呢？1 0
            pid_t id = fork();
            assert(id != -1);
            if(id == 0)
            {
                // 子进程， 子进程会不会继承父进程打开的文件与文件fd呢？1, 0
                // 子进程是来进行提供服务的，需不需要知道监听socket呢？
                close(listensock);
                service(servicesock, client_ip, client_port);
                exit(0); // 僵尸状态
            }
            close(servicesock); // 如果父进程关闭servicesock，会不会影响子进程？？下节课
            // 父进程
            // waitpid(); // 
        }
    }
    ~TcpServer(){}
private:
    uint16_t _port;
    std::string _ip;
    int listensock;
};