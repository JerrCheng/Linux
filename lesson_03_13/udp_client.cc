#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " serverIp serverPort\n" << std::endl;
}

// ./udp_client 127.0.0.1 8080
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }
    // client要不要bind？？要，但是一般client不会显示的bind，程序员不会自己bind
    // client是一个客户端 -> 普通人下载安装启动使用的-> 如果程序员自己bind了->
    // client 一定bind了一个固定的ip和port，万一，其他的客户端提前占用了这个port呢？？
    // client一般不需要显示的bind指定port，而是让OS自动随机选择(什么时候做的呢？)
    std::string message;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    char buffer[1024];
    while(true)
    {
        //多线程
        std::cout << "请输入你的信息# ";
        std::getline(std::cin, message);
        if(message == "quit") break;
        // 当client首次发送消息给服务器的时候，OS会自动给client bind他的IP和PORT
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof server);

        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t s = recvfrom(sock, buffer, sizeof buffer, 0, (struct sockaddr*)&temp, &len);
        if(s > 0)
        {
            buffer[s] = 0;
            std::cout << "server echo# " << buffer << std::endl;
        }
    }
    close(sock);
    return 0;
}