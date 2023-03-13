#ifndef _UDP_SERVER_HPP
#define _UDP_SERVER_HPP

#include "log.hpp"
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SIZE 1024

class UdpServer
{
public:
    UdpServer(uint16_t port, std::string ip = "") : _port(port), _ip(ip), _sock(-1)
    {
    }
    bool initServer()
    {
        // 从这里开始，就是新的系统调用，来完成网络功能啦
        //  1. 创建套接字
        _sock = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET->FP_INET
        if (_sock < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(2);
        }
        // 2. bind: 将用户设置的ip和port在内核中和我们当前的进程强关联
        // "192.168.110.132" -> 点分十进制字符串风格的IP地址
        // 每一个区域取值范围是[0-255]: 1字节 -> 4个区域
        // 理论上，表示一个IP地址，其实4字节就够了！！
        // 点分十进制字符串风格的IP地址 <-> 4字节
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // 服务器的IP和端口未来也是要发送给对方主机的 -> 先要将数据发送到网络！
        local.sin_port = htons(_port);
        // 1. 同上，先要将点分十进制字符串风格的IP地址 ->  4字节
        // 2.  4字节主机序列 -> 网络序列
        // 有一套接口，可以一次帮我们做完这两件事情, 让服务器在工作过程中，可以从任意IP中获取数据
        local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
        if (bind(_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "init udp server done ... %s", strerror(errno));
        // done
        return true;
    }
    void Start()
    {
        // 作为一款网络服务器，永远不退出的！
        // 服务器启动-> 进程 -> 常驻进程 -> 永远在内存中存在，除非挂了！
        // echo server: client给我们发送消息，我们原封不动返回
        char buffer[SIZE];
        for (;;)
        {
            // 注意：
            //  peer,纯输出型参数
            struct sockaddr_in peer;
            bzero(&peer, sizeof(peer));
            // 输入: peer 缓冲区大小
            // 输出: 实际读到的peer
            socklen_t len = sizeof(peer);
            char result[256];
            std::string cmd_echo;
            // start. 读取数据
            ssize_t s = recvfrom(_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                buffer[s] = 0; // 我们目前数据当做字符串
                // 1. 输出发送的数据信息
                // 2. 是谁？？
                //  你发过来的字符串是指令 ls -a -l, rm -rm ~
                // if(strcasestr(buffer, "rm") != nullptr || strcasestr(buffer, "rmdir") != nullptr)
                // {
                //     std::string err_message = "坏人.... ";
                //     std::cout << err_message << buffer << std::endl;
                //     sendto(_sock, err_message.c_str(), err_message.size(), 0, (struct sockaddr *)&peer, len);
                //     continue;
                // }
                // FILE *fp = popen(buffer, "r");
                // if (nullptr == fp)
                // {
                //     logMessage(ERROR, "popen: %d:%s", errno, strerror(errno));
                //     continue;
                // }
                // while (fgets(result, sizeof(result), fp) != nullptr)
                // {
                //     cmd_echo += result;
                // }
                // fclose(fp);
                uint16_t cli_port = ntohs(peer.sin_port);      // 从网络中来的！
                std::string cli_ip = inet_ntoa(peer.sin_addr); // 4字节的网络序列的IP->本主机的字符串风格的IP，方便显示
                // printf("[%s:%d]# %s\n", cli_ip.c_str(), cli_port, buffer);
                char key[64];
                snprintf(key, sizeof(key), "%s-%u", cli_ip.c_str(), cli_port); // 127.0.0.1-8080
                // std::string key_string = key;
                auto it = _users.find(key);
                if (it == _users.end())
                {
                    // exists
                    _users.insert({key, peer});
                }
            }
            // 分析和处理数据，TODO
            // end. 写回数据
            // sendto(_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&peer, len);
            // sendto(_sock, cmd_echo.c_str(), cmd_echo.size(), 0, (struct sockaddr *)&peer, len);
            for (auto &iter : _users)
            {
                sendto(_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&(iter.second), sizeof(iter.second));
            }
        }
    }
    ~UdpServer()
    {
        if (_sock >= 0)
            close(_sock);
    }

private:
    // 一个服务器，一般必须需要ip地址和port(16位的整数)
    uint16_t _port;
    std::string _ip;
    int _sock;
    std::unordered_map<std::string, struct sockaddr_in> _users;
};
#endif