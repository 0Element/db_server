#include <iostream>

#include "file_client.hpp"
#include "message.hpp"


FileClient::FileClient()
{}

FileClient::~FileClient()
{}

void FileClient::Print()
{
    port_msg_t port_msg = Message::Pop();
    std::cerr << "Print 1 from mesage :" << port_msg.first << ":\n";
    std::cerr << "Print 2 from mesage :" << port_msg.second << ":\n";
}