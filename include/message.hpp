#pragma once

#include <map>


typedef std::map<int, std::string> m_ptr_message_t;

class Message
{
public:
    static std::string AddMsg(int sock, const char* buff);
    static void Clear(int sock);
    static void Erase(int sock);

    static m_ptr_message_t messages;
    static char* start_flag;
    static char* end_flag;

private:
    Message();
    ~Message();
    Message(const Message&);
    Message& operator=(Message&);
};
