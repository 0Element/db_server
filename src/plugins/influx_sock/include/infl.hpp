#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <atomic>


class Influx
{
public:
    Influx(const char* addr, int port, int count_msgs, int count_thrs);
    ~Influx();

    void SendRecv();

    void Join();
    void Detach();

    bool run;
    std::atomic<int> count_thrs;

private:
    void GenMessages(int count_msgs, int count_thrs);

    int GetSock();
    std::string GetMsg(int i);
    void _SendRecv(int i);

    std::vector<std::thread> vec_thrs;
    std::vector<std::vector<std::string>> v_msgs;
    std::vector<std::mutex> mtx_msg;
    std::vector<int> number_msg;

    const char *addr;
    int port;

    bool running;
};
