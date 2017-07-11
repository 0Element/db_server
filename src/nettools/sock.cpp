#include "sock.hpp"

#include <exception>
#include <cstring>

#include <unistd.h>
#include <stdio.h>


static const char* presponse = "{\"type\":\"response\", \"status\": \"%d\",\"message\": \"%s\"}";

char* Makeresponse(char* pbuf, const char* ptmpl, int st, const char* pmes)
{
    sprintf(pbuf, ptmpl, st, pmes);
    return pbuf;
}

void make_non_block(int sock)
{
    int st = -1;
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1)
        throw (std::exception());
    st = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    if (st == -1)
        throw (std::exception());
}

int Read(int sock, char* buff)
{
    int len_buff = 0;
    //char* buff = (char*) calloc(1, 1024);
    len_buff = read(sock, buff, 1024);

    return len_buff;
}

void Write(int sock, int status, const char* msg)
{
    char pbuf[1024];
    Makeresponse(pbuf, presponse, status, msg);
    send(sock, pbuf, strlen(pbuf), 0);
}

void Close(int sock)
{
    close(sock);
}
