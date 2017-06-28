#pragma once

#include <memory>

#include "lqueue.hpp"


class Message;
typedef std::shared_ptr<Message> ptr_msg_t;

class Message
{
public:
    Message();
    ~Message();

    void AddMsg(int sock, const char* buff, int len_buff);
    void Clear(int sock);
    void Erase(int sock);

    static port_msg_t Pop();

private:
    Message(const Message&);
    Message& operator=(Message&);

    m_port_msg_t messages;
    static Lqueue lqueue;

    static std::string start_flag;
    static std::string end_flag;
};
