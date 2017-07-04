#include <iostream>
#include <cstring>

#include "file_client.hpp"
#include "message.hpp"
#include "server.hpp"
#include "sock.hpp"


FileClient::FileClient()
{}

FileClient::~FileClient()
{}

void FileClient::GetMsg(std::string name_plug)
{
    port_msg_t port_msg;
    if (name_plug.empty())
        port_msg = Message::Pop("default" /*"file_client"*/);
    else
        port_msg = Message::Pop(name_plug);

    std::cerr << "Print 1 from mesage :" << port_msg.first << ":\n";
    std::cerr << "Print 2 from mesage :" << port_msg.second << ":\n";

    Write(port_msg.first);
}
