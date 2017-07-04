#pragma once

#include <vector>
#include <memory>
#include <thread>

#include "epoll.hpp"
#include "message.hpp"


class Client;
typedef std::shared_ptr<Client> ptr_client_t;
typedef std::vector<ptr_client_t> v_ptr_client_t;

class Client
{
public:
    Client();
    ~Client();

    void Run();
    void Stop();
    void AddSock(int sock);

    static int AddClSock(int sock);
    static v_ptr_client_t clients;

private:
    void _Run();
    //int Read(int sock);
    //int Send(int sock, const char* buff, int len);
    void CloseSock(int sock);

    ptr_msg_t messages;
    std::thread thr;
    ptr_epoll_t epoll;
    bool quit;
};
