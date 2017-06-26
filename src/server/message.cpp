#include "message.hpp"

#include <iostream>

#include <string.h>


m_ptr_message_t Message::messages;
char* Message::start_flag = "12\n";
char* Message::end_flag = "90\n";

Message::Message()
{}

Message::~Message()
{}

std::string Message::AddMsg(int sock, const char* buff)
{
    std::cerr << "buff:" << buff << ":\n";
    //std::cerr << "msg:" << Message::messages[sock] << ":\n";
    if (Message::messages[sock].empty()) {
        if (strncmp(buff, start_flag, strlen(start_flag)) == 0) // Begin format
            Message::messages[sock] = buff;
        if (strncmp(&buff[strlen(buff) - strlen(end_flag)],
            end_flag, strlen(end_flag)) == 0) // End format
        {
            return Message::messages[sock];
        }
    }
    else {
        if (strncmp(buff, start_flag, strlen(start_flag)) == 0) // Begin format
            Message::messages[sock] = buff;
        else
            Message::messages[sock] += buff;

        if (strncmp(&buff[strlen(buff) - strlen(end_flag)],
            end_flag, strlen(end_flag)) == 0) // End format
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