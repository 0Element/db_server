#include "lqueue.hpp"


Lqueue::Lqueue()
{}

Lqueue::~Lqueue()
{}

void Lqueue::Push(port_msg_t port_msg)
{
    msgs.push(port_msg);
    condition_msgs.notify_one();
}

port_msg_t Lqueue::Pop()
{
    std::lock_guard<std::mutex> lock(mutex_msgs);
    while(msgs.empty()) {
        std::unique_lock<std::mutex> uni_lock(mutex_msgs_u);
        condition_msgs.wait(uni_lock);
    }

    port_msg_t result(msgs.front());
    msgs.pop();

    return result;
}
