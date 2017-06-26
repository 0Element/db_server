#pragma once

#include <memory>

#include <sys/epoll.h>
#include <unistd.h>

class Epoll
{
public:
    Epoll();

    virtual ~Epoll()
    {
        close(epoll_fd_);
    };

    int AddEpollClientFd(int fd);
    int AddEpollSrvFd(int fd);
    int ModEpollFd(struct epoll_event event);
    int DelEpollFd(int fd);
    struct epoll_event* EpollWait(int *nr_count);

    int GetEpollFd()
    {
        return epoll_fd_;
    };

private:
    int epoll_fd_;
    int err_;
};

typedef std::shared_ptr<Epoll> ptr_epoll_t;