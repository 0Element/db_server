#include "message.hpp"

#include <iostream>

#include <string.h>


m_ptr_message_t Message::messages;
std::string Message::start_flag = "\0{";
std::string Message::end_flag = std::string().assign("}\0", 2);

Message::Message()
{}

Message::~Message()
{}

std::string Message::AddMsg(int sock, const char* buff, int len_buff)
{
    //std::cerr << "msg:" << Message::messages[sock] << ":\n";
    if (Message::messages[sock].empty()) {
        if (strncmp(buff, start_flag.c_str(), start_flag.size()) == 0) { // Begin format
            Message::messages[sock] = std::string(buff, len_buff);
        }
        if (strncmp(&buff[len_buff - end_flag.size()],
            end_flag.c_str(), end_flag.size()) == 0) // End format
        {
            return Message::messages[sock];
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
            return Message::messages[sock]; // Return for send
        }
    }

    return "";
}

void Message::Clear(int sock)
{
    Message::messages[sock] = "";
}

void Message::Erase(int sock)
{
    Message::messages.erase(sock);
}