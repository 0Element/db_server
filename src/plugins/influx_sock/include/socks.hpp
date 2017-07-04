#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>

typedef std::vector<std::pair<int, std::string>> v_socks_msg_t;
typedef std::vector<std::pair<int, int>> v_socks_ret_t;
typedef std::vector<int> v_socks_t;


int conn(const char* addr, int port);
v_socks_t get_socks(const char* addr, int port, int count);
void close_socks(v_socks_t vec_socks);

v_socks_ret_t v_send_msg(v_socks_t vec_socks, const char* msg, int size);
int send_msg(int sock, const char* msg, int size);

v_socks_msg_t v_recv_msg(v_socks_t vec_socks);
const char* recv_msg(int sock);
