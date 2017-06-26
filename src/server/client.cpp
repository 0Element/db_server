#include "client.hpp"
#include "message.hpp"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>


v_ptr_client_t Client::clients;

Client::Client()
{
    std::cerr << "Constructor client\n";
    epoll = ptr_epoll_t(new Epoll());

    Client::clients.push_back(ptr_client_t(this));
}

void Client::Run()
{
    quit = false;
    thr = std::thread(&Client::_Run, this);
    thr.detach();
    //thr.join();
}

void Client::_Run()
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
                CloseSock(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                std::cerr << "EPOLLHUP\n";
                CloseSock(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLIN) {
                std::cerr << "event: " << i << " IN on fd: \t" << events[i].data.fd << '\n';
                int len_buff = 0;
                char* buff = (char*) calloc(1, 1024);
                len_buff = read(events[i].data.fd, buff, 1024);
                std::string data;
                data.assign(Message::AddMsg(events[i].data.fd, buff, len_buff).c_str(), len_buff);
                if (!data.empty()) {
                    Send(events[i].data.fd, data.c_str(), data.size());
                    Message::Clear(events[i].data.fd);
                }

                delete buff;
                if (len_buff == 0) CloseSock(events[i].data.fd);
            }
        }

        delete events;
    }
}

void Client::CloseSock(int sock)
{
    close(sock);
    Message::Erase(sock);
}

void Client::Stop()
{
    std::cerr << "Stop\n";
    quit = true;
}

int Client::AddClSock(int sock) {
    if (Client::clients.empty())
        return -1;

    static size_t number = 0;
    if ((number >= Client::clients.size()) || (number < 0))
        number = 0;

    Client::clients[number++]->AddSock(sock);

    return 0;
}

void Client::AddSock(int sock)
{
    epoll->AddEpollSrvFd(sock);
}

Client::~Client()
{
    std::cerr << "Destructor client\n";
}

/*
 * only read data from sock
 * and stoping when bytes(number reading bytes) < all size buffer
 * return 0 if true else -1
 */
/*int Client::Read(int from_sock)
{
    Messages &ptr_msg = socks_msg_[from_sock];
    char buff[BUFF_SIZE];
    int bytes = 0;
    bool closed = false;
    bool complete = false;

    std::memset(buff, 0, BUFF_SIZE + 1);

    if (ptr_msg.res == NULL)
        ptr_msg.res = (char*) calloc(1, 0);

    //std::cout << "read from: " << from_sock << "\n";

    while (running_) {
        bytes = recv(from_sock, buff, BUFF_SIZE, 0);
        if (bytes < 0) {
            if (errno == EAGAIN) {
                //snd_sys_log(WARNING, "WARNING break in read_data EAGAIN");
                std::cout << "\nEAGAIN" << '\n';
                break;
            }
            if (errno == EWOULDBLOCK) {
                //snd_sys_log(WARNING, "WARNING break in read_data EWOULDBLOCK");
                std::cout << "EWOULDBLOCK" << '\n';
                complete = true;
                break;
            }
            if (errno == EBADF) {
                //snd_sys_log(WARNING, "WARNING break in read_data EBADF");
                std::cout << "EBADF" << '\n';
                closed = true;
                break;
            }
            std::cout << "errno: " << errno << "\n";
            std::cerr << "bytes < 0" << '\n';

            client_epoll_->DelEpollFd(from_sock);
            close(from_sock);

            return -1;
        }
        if (bytes == 0) {
            std::cout << "bytes == 0\n";
            closed = true;
            break;
        }

        //std::cout << "bytes > 0\n";
        //std::cout << "bytes:" << bytes << " BUFF_SIZE: " << BUFF_SIZE << '\n';

        if (ptr_msg.len_res == 0) {
            HeadInfo *header = get_header(buff, bytes);
            if (header && !header->conn_close) {
                ptr_msg.len_res_all = header->content_length + header->len_head;
                ptr_msg.len_head = header->len_head;
            }
        }

        buff[bytes] = '\0';

        ptr_msg.len_res += bytes;
        if (ptr_msg.len_res_all != 0 && ptr_msg.len_res_all == ptr_msg.len_res) {
            complete = true;
        }

        std::cout << "len_res:" << ptr_msg.len_res << "\n";
        //std::cout << " bytes: " << bytes;
        //std::cout << "  ID: " << ptr_msg->id_cl_mod << '\n';

        if ((ptr_msg.res = (char*) realloc(ptr_msg.res, ptr_msg.len_res + 1)) != NULL) {
            std::memcpy(&ptr_msg.res[ptr_msg.len_res - bytes], buff, bytes);
            ptr_msg.res[ptr_msg.len_res] = '\0';
        }
        //std::cout << "ptr_msg->res: \n" << ptr_msg->res << "\n<<<<<<<<<<<<<<\n";
        //std::cout << "buff: \n" << buff << "\n<<<<<<<<<<<<<<\n";
    }

    if (closed || complete) {
        if (closed) {
            CloseSock_(from_sock);
            //client_epoll_->DelEpollFd(from_sock);
            close(from_sock);

            ptr_msg.complete_req = 0;
            ptr_msg.complete_res = 1;
            ptr_msg.closed = 1;
        }
        else if (complete) {
            ptr_msg.complete_req = 0;
            ptr_msg.complete_res = 1;

            struct epoll_event event;
            event.data.fd = from_sock;
            event.events = EPOLLOUT;
            client_epoll_->ModEpollFd(event);
        }
        std::cout << "ID: \n" << ptr_msg.id_cl_mod << '\n';
        std::cout << "REQUEST: \n" << ptr_msg.req << '\n';
        std::cout << "REQ_LEN: \n" << ptr_msg.len_req << '\n';
        //std::cout << "RESPONSE: \n" << ptr_msg->res << '\n';
        std::cout << "RES_LEN: \n" << ptr_msg.len_res << '\n';
        std::cout << "RES_HEAD: \n" << ptr_msg.len_head << '\n';

        //
    }

    return 0;
}*/

/*
 * Send data into sock fd
 * return 0 if true else -1
 */
int Client::Send(int sock, const char* msg, int bytes)
{
    int all_bytes = 0, ret = 0;

    //std::cout << "bytes: %d all_bytes: %d\n", bytes, all_bytes;
    while (0 < bytes) {
        ret = send(sock, &msg[all_bytes], bytes, 0);
        if (ret < 0) {
            //std::cout << "ret: " << ret << "\n";
            if (errno == EAGAIN) {
                //std::cout << "send EAGAIN 11\n";
                continue;
            }
            if (errno == EWOULDBLOCK) {
                //snd_sys_log(INFO, "INFO send EWOULDBLOCK");
                continue;
            }
            if (errno == EBADF) {
                //snd_sys_log(WARNING, "WARNING send EBADF 9");
                //break;
                return -1;
            }
            if (errno == ECONNRESET) {
                //snd_sys_log(WARNING, "WARNING send ECONNRESET 104");
                //break;
                return -1;
            }
            if (errno == EFAULT) {
                //snd_sys_log(WARNING, "WARNING send EFAULT 14");
                //break;
                return -1;
            }
            //std::cout << "errno: %d\n", errno;
            //snd_sys_log(WARNING, "WARNING send errno in send");
            break;
        }
        else if (ret == 0) {
            //snd_sys_log(INFO, "INFO ret == 0");
            continue;
        }
        else if (ret > 0) {
            all_bytes += ret;
            bytes -= ret;
        }
    }

    return 0;
}
