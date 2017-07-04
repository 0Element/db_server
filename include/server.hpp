#pragma once

#include <vector>
#include <thread>
//#include <mutex>
#include <memory>

#include "epoll.hpp"


class Server;
typedef std::shared_ptr<Server> ptr_srv_t;
typedef std::vector<ptr_srv_t> v_ptr_srv_t;

class Server
{
public:
    Server(const char* addr, int port);
    ~Server();

    void Run();
    void Stop();
    int GetClSock();
    void Read(int sock);
    static void Write(int sock);

    static v_ptr_srv_t servers;

private:
    void _Run();

    std::thread thr;
    int sock;
    ptr_epoll_t epoll;
    bool quit;
    //std::queue<int> cl_socks;
};
