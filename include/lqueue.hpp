#pragma once

#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>


typedef std::pair<int, std::string> port_msg_t;
typedef std::queue<port_msg_t> q_port_msg_t;
typedef std::map<int, std::string> m_port_msg_t;

class Lqueue
{
public:
    Lqueue();
    ~Lqueue();

    void Push(port_msg_t port_msg);
    port_msg_t Pop();

private:
    std::mutex mutex_msgs;
    std::mutex mutex_msgs_u;
    std::condition_variable condition_msgs;
    q_port_msg_t msgs;
};