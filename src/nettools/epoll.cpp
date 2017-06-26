#include <vector>
#include <iostream>

#include "epoll.hpp"

#define MAX_EVENTS 64

/*
 * create epoll descriptor
 * return epfd_ if true else -1
 */
Epoll::Epoll()
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ < 0) {
        std::cerr << "Error epoll_create1" << '\n';
        throw (std::exception());
    }
}

/*
 * add fd in epoll reactor
 * return 0 if true else -1
 */
int Epoll::AddEpollClientFd(int fd)
{
    struct epoll_event event;

    event.data.fd = fd;
    event.events = EPOLLOUT;

    if ((epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event)) < 0) {
        std::cerr << "epoll_ctl_add" << '\n';
        err_ = -1;
        return -1;
    }

    return 0;
}

/*
 * add fd in epoll reactor
 * return 0 if true else -1
 */
int Epoll::AddEpollSrvFd(int fd)
{
    struct epoll_event event;

    event.data.fd = fd;
    event.events = EPOLLIN;

    if ((epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event)) < 0) {
        std::cerr << "epoll_ctl_add" << '\n';
        err_ = -1;
        return -1;
    }

    return 0;
}

/*
 * modify fd event in epoll reactor
 * return 0 if true else -1
 */
int Epoll::ModEpollFd(struct epoll_event event)
{
    if ((epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, event.data.fd, &event)) < 0) {
        std::cerr << "epoll_ctl_mod" << '\n';
        err_ = -1;
        return -1;
    }

    return 0;
}

/*
 * del fd from epoll reactor
 * return 0 if true else -1
 */
int Epoll::DelEpollFd(int fd)
{
    if ((epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL)) < 0) {
        std::cerr << "epoll_ctl_del" << '\n';
        err_ = -1;
        return -1;
    }

    printf("del fd %d from epoll\n", fd);

    return 0;
}

/*
 * wait events in epoll reactor
 * return epoll_events if true else NULL
 */
struct epoll_event* Epoll::EpollWait(int *nr_events)
{
    struct epoll_event *events;

    events = new epoll_event[MAX_EVENTS];
    if (!events) {
        std::cerr << "malloc" << '\n';
        err_ = -1;
        return NULL;
    }

    *nr_events = epoll_wait(epoll_fd_, events, MAX_EVENTS, 5000);
    if (*nr_events < 0) {
        std::cerr << "epoll_wait" << '\n';
        delete [] events;
        err_ = -1;
        return NULL;
    }

    return events;
}
