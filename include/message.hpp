#pragma once

#include <memory>

#include "lqueue.hpp"


class Message;
typedef std::shared_ptr<Message> ptr_msg_t;
typedef std::map<std::string, Lqueue> map_cl_name_lqueue;

class Message
{
public:
    Message();
    ~Message();

    void AddMsg(int sock, const char* buff, int len_buff);
    void Clear(int sock);
    void Erase(int sock);

    static port_msg_t Pop(std::string client_name = "default");

private:
    Message(const Message&);
    Message& operator=(Message&);

    m_port_msg_t messages;
    static map_cl_name_lqueue lqueues;

    static std::string start_flag;
    static std::string end_flag;
};
