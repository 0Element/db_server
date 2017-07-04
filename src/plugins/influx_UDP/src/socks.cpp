#include "socks.hpp"


int conn(const char* addr, int port) {
    int sock;
    struct sockaddr_in srv;

    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;

    if (connect(sock, (struct sockaddr *) &srv, sizeof(srv)) < 0)
        return -2;

    return sock;
}

int get_sock()
{
    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return -1;

    return sock;
}

int send_to(int sock, const char *msg, int len, struct sockaddr_in &srv)
{
    if (sendto (sock, msg, len, 0,
                (struct sockaddr *) &srv, sizeof(srv)) < 0)
        return -1;

    return 0;
}

std::vector<int> get_socks(const char* addr, int port, int count) {
    std::vector<int> vec_socks;

    for (int i = 0; i < count; i++) {
        int sock = 0;
        if ((sock = conn(addr, port)) > 0) {
            //std::cerr << "Connect to " << addr << ":" << port << "\n";
            vec_socks.push_back(sock);
        }
        else
            std::cerr << i << " ERROR connect to " << addr << ":" << port << "\n";
    }

    return vec_socks;
}

void close_socks(std::vector<int> vec_socks) {
    for (int sock : vec_socks) {
        if (sock) {
            close(sock);
            //std::cerr << "Close\n";
        }
        else std::cerr << "Fail closed\n";
    }
}

v_socks_ret_t v_send_msg(v_socks_t vec_socks, const char* msg, int size) {
    v_socks_ret_t result;

    size_t len = 0;
    for (int sock : vec_socks) {
        len = send_msg(sock, msg, size);
        result.push_back(std::pair<int, int>(sock, len));

        if (len != (size_t) size)
            std::cerr << "BAD send bytes: " << len << "\n";
        //else std::cerr << "OK send bytes: " << len << "\n";
    }

std::cerr << "msg:\n" << msg << ":\n\n";

    return result;
}

int send_msg(int sock, const char* msg, int size) {
    int ret = 0, send_bytes = 0;
    int tmp_size = size;

    while (send_bytes < size) {
        ret = send(sock, &msg[send_bytes], tmp_size, 0);
        if (ret <= 0) {
            return send_bytes;
        }

        send_bytes += ret;
        tmp_size -= ret;
    }

    return send_bytes;
}

v_socks_msg_t v_recv_msg(v_socks_t vec_socks) {
    v_socks_msg_t result;

    for (int sock : vec_socks) {
        std::string buff = recv_msg(sock);
        result.push_back(std::pair<int, std::string>(sock, buff));
    }

    return result;
}

const char* recv_msg(int sock) {
    char buff[1024];

    recv(sock, buff, 1024, 0);

    //std::cerr << "buff:" << buff << "\n";

    return "";
}
