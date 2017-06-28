#include "message.hpp"

#include <iostream>

#include <string.h>

Lqueue Message::lqueue;

std::string Message::start_flag = "\0{";
std::string Message::end_flag = std::string().assign("}\0", 2);

Message::Message()
{}

Message::~Message()
{}

void Message::AddMsg(int sock, const char* buff, int len_buff)
{
    //std::cerr << "msg:" << Message::messages[sock] << ":\n";
    if (Message::messages[sock].empty()) {
        if (strncmp(buff, start_flag.c_str(), start_flag.size()) == 0) { // Begin format
            Message::messages[sock] = std::string(buff, len_buff);
        }
        if (strncmp(&buff[len_buff - end_flag.size()],
            end_flag.c_str(), end_flag.size()) == 0) // End format
        {
            //Send in queue databases
            //return Message::messages[sock];
            Message::lqueue.Push(port_msg_t(sock, Message::messages[sock]));
            Clear(sock);
        }
    }
    else {
        if (strncmp(buff, start_flag.c_str(), start_flag.size()) == 0) {// Begin format
            Message::messages[sock].assign(buff, len_buff);
        }
        else
            Message::messages[sock].assign(buff, len_buff);

        if (strncmp(&buff[len_buff - end_flag.size()],
            end_flag.c_str(), end_flag.size()) == 0) // End format
        {
            std::cerr << "send:" << Message::messages[sock] << ":\n";
            //Send in queue databases
            //return Message::messages[sock];
            Message::lqueue.Push(port_msg_t(sock, Message::messages[sock]));
            Clear(sock);
        }
    }

    //return "";
}

void Message::Clear(int sock)
{
    Message::messages[sock] = "";
}

void Message::Erase(int sock)
{
    Message::messages.erase(sock);
}

port_msg_t Message::Pop()
{
    return Message::lqueue.Pop();
}
