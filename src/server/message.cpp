#include "message.hpp"
#include "picojson.h"

#include <iostream>
#include <cstring>

map_cl_name_lqueue Message::lqueues;

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
        std::string str_buff;
        bool begin = false;
        bool for_send = false;

        if (strncmp(buff, start_flag.c_str(), start_flag.size()) == 0) { // Begin format
            //Message::messages[sock] = std::string(buff + 1, len_buff - 1);
            str_buff = std::string(buff + 1, len_buff - 1); // Erase start \0
            begin = true;
        }
        else return;

        if (strncmp(&buff[len_buff - end_flag.size()],
            end_flag.c_str(), end_flag.size()) == 0) // End format
        {
            str_buff.pop_back(); // Erase end \0
            for_send = true;
        }

        Message::messages[sock] = str_buff;

        if (for_send) {
            std::cerr << "send:" << Message::messages[sock] << ":\n";
            //Send in queue databases
            picojson::value v;
            std::string str_json = Message::messages[sock];
            const char *json = str_json.c_str();
            std::string err = picojson::parse(v, json, json + str_json.size());

            std::string db = v.get("db").to_str();
            if (db.empty()) db = "default";

            Message::lqueues[db].Push(port_msg_t(sock, Message::messages[sock]));
            Clear(sock);
        }
    }
    else {
        std::string str_buff;
        bool begin = false;
        bool for_send = false;

        if (strncmp(buff, start_flag.c_str(), start_flag.size()) == 0) {// Begin format
            //Message::messages[sock].assign(buff + 1, len_buff - 2);
            str_buff = std::string(buff + 1, len_buff - 1); // Erase start \0
            begin = true;
            Clear(sock);
        }

        if (strncmp(&buff[len_buff - end_flag.size()],
            end_flag.c_str(), end_flag.size()) == 0) // End format
        {
            if (!begin)
                str_buff = std::string(buff, len_buff);

            str_buff.pop_back(); // Erase end \0

            for_send = true;
        }

        if (!begin && !for_send)
            Message::messages[sock].assign(buff, len_buff);
        else
            Message::messages[sock].append(str_buff);

        if (for_send) {
            std::cerr << "send:" << Message::messages[sock] << ":\n";
            //Send in queue databases
            picojson::value v;
            std::string str_json = Message::messages[sock];
            const char *json = str_json.c_str();
            std::string err = picojson::parse(v, json, json + str_json.size());

            std::string db = v.get("db").to_str();
            if (db.empty()) db = "default";

            Message::lqueues[db].Push(port_msg_t(sock, Message::messages[sock]));
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

port_msg_t Message::Pop(std::string cl_name)
{
    return Message::lqueues[cl_name].Pop();
}
