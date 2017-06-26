#include "for_sock.hpp"

#include <exception>


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
