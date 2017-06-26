#include "server.hpp"
#include "client.hpp"
#include "for_sock.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <exception>

#include <stdio.h>
#include <errno.h>

v_ptr_srv_t Server::servers;

Server::Server(const char* addr, int port)
{
    std::cerr << "Constructor Server\n";
    struct sockaddr_in srv;
    std::memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error sock");
        throw (std::exception());
    }
    make_non_block(sock);

    if (bind(sock, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("error bind");
        throw (std::exception());
    }
    if (listen(sock, 1) < 0) {
        perror("error listen");
        throw (std::exception());
    }

    epoll = ptr_epoll_t(new Epoll());
    epoll->AddEpollSrvFd(sock);

    servers.push_back(ptr_srv_t(this));
}

void Server::Run()
{
    quit = false;
    thr = std::thread(&Server::_Run, this);
    thr.detach();
    //thr.join();
}

void Server::_Run()
{
    int i;
    int nr_events;
    struct epoll_event *events;

    while(!quit) {
        std::cerr << "Run\n";

        events = epoll->EpollWait(&nr_events);

        for (i = 0; i < nr_events; i++) {
            std::cerr << "Event: " << i << "\n";

            if (events[i].events & EPOLLRDHUP) {
                std::cerr << "EPOLLRDHUP\n";
            }
            if (events[i].events & EPOLLERR) {
                std::cerr << "EPOLLERR\n";
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                std::cerr << "EPOLLHUP\n";
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLIN) {
                std::cerr << "event: " << i << " IN on fd: \t" << events[i].data.fd << '\n';
                if (events[i].data.fd == sock) {
                    std::cerr << "Accept client\n";
                    struct sockaddr_in cl;
                    socklen_t cl_len = sizeof(cl);

                    std::memset(&cl, 0, cl_len);
                    int cl_sock = accept(sock, (struct sockaddr*) &cl, &cl_len);
                    if (cl_sock > 0) {
                        //cl_socks.push_back(cl_sock);
                        make_non_block(cl_sock);
                        if (Client::AddClSock(cl_sock) < 0) {
                            std::cerr << "FAIL add cl socket: \t" << events[i].data.fd << '\n';
                        }
                    }
                }
            }
        }

        delete events;
    }

    /*while(!quit) {
        struct sockaddr_in cl;
        socklen_t cl_len = sizeof(cl);

        std::memset(&cl, 0, cl_len);
        int cl_sock = accept(sock, (struct sockaddr*) &cl, &cl_len);
        if (cl_sock > 0)
            cl_socks.push_back(cl_sock);
    }*/
}

void Server::Stop()
{
    std::cerr << "Stop\n";
    quit = true;
}

Server::~Server()
{
    if (sock) close(sock);
    /*for (int cl_sock : cl_socks) {
        if (cl_sock) close(sock);
    }*/

    std::cerr << "Destructor Server\n";
}
