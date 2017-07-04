#include "infl.hpp"
#include "socks.hpp"

#include <algorithm>
#include <ctime>


Influx::Influx(const char* addr, int port, int count_msgs, int count_thrs)
{
    this->addr = addr;
    this->port = port;
    this->running = true;
    this->run = false;
    this->count_thrs = count_thrs;

    if (count_thrs > count_msgs)
        count_thrs = count_msgs;

    int i = 0;
    while (i++ < count_thrs) {
        number_msg.push_back(0);
        v_msgs.push_back(std::vector<std::string>());
        //mtx_msg.push_back(std::mutex());
    }

    GenMessages(count_msgs, count_thrs);
}

Influx::~Influx()
{
    //close_socks(v_socks);
}

int Influx::GetSock()
{
    return conn(addr, port);
}

void Influx::GenMessages(int count_msgs, int count_thrs)
{
    std::string src_msg1("POST /write?db=example HTTP/1.1\r\n"
                    "Host: localhost:8086\r\n"
                    "User-Agent: curl/7.47.0\r\n"
                    "Accept: */*\r\n"
                    "Content-Length: ");
    std::string src_msg2("\r\n"
                    "Content-Type: application/x-www-form-urlencoded\r\n"
                    "\r\n");
    //std::string src_msg3("cpu_load_short,host=server01,region=us-west value=\"value");
    std::string src_msg3("cpu_load_short value=\"value");

    for (int i = 0, j = 1; i < count_msgs; i++) {
        std::string msg3 = src_msg3 + std::to_string(i);
        std::string msg = src_msg1 +
            std::to_string(msg3.size() + 1) +
            src_msg2 +
            msg3 + "\"";

        v_msgs[j - 1].push_back(msg);
        if (++j > count_thrs) j = 1;
    }
}

void Influx::SendRecv()
{
    for (int i = 0; i < count_thrs; i++) {
        //std::cerr << msg << "\n";

        std::thread thr(&Influx::_SendRecv, this, i);
        thr.detach();
        vec_thrs.push_back(move(thr));
        //v_socks_ret_t v_s_rets = v_send_msg(vec_socks, msg.c_str(), msg.size());
    }
}

void Influx::_SendRecv(int i)
{
    //int sock = GetSock();

    while(running) {
        if (!run) continue;

        int sock = GetSock();
        std::string msg;
        msg = GetMsg(i);

        if (msg.empty()) break;

        send_msg(sock, msg.c_str(), msg.size());
        recv_msg(sock);
//std::cerr << "thr:" << i << "\n";
        close(sock);
    }
    count_thrs--;
    //close(sock);
}

void Influx::Join()
{
    for (std::thread &thr : vec_thrs) {
        thr.join();
    }
    //std::for_each(vec_thrs.begin(),vec_thrs.end(),do_join);
}

void Influx::Detach()
{
    for (std::thread &thr : vec_thrs) {
        thr.detach();
    }
    //std::for_each(vec_thrs.begin(),vec_thrs.end(),do_join);
}

std::string Influx::GetMsg(int i)
{
    if (number_msg[i] < (int)v_msgs[i].size()) {
        //std::lock_guard<std::mutex> lock(mtx_msg[i]);
//std::cerr << "msg:\n" << v_msgs[number_msg] << "\n\n";
        return v_msgs[i][number_msg[i]++];
    }

    return "";
}
