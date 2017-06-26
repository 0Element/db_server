#include <iostream>

#include "server.hpp"
#include "client.hpp"

#include <exception>

#include <unistd.h>
#include <signal.h>


void prn_sig(int sig)
{
    std::cerr << "SIGINT\n";

    for (auto srv : Server::servers) {
        srv->Stop();
    }
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, prn_sig);
    try {
        const char *srv_addr = "127.0.0.1";
        int srv_port = 1050;

        new Server(srv_addr, srv_port);
        new Server(srv_addr, srv_port + 1);
        std::cerr << "Servers count: " << Server::servers.size() << "\n";

        for (auto srv : Server::servers) {
            srv->Run();
        }

        new Client();
        new Client();
        new Client();

        for (auto cl : Client::clients) {
            cl->Run();
        }

        std::cerr << "Clients count: " << Client::clients.size() << "\n";

        sleep(-1);
    }
    catch (std::exception) {
        std::cerr << "Exception\n";
    }

    std::cerr << "End\n";

    return 0;
}