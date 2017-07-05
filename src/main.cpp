#include <thread>
#include <iostream>
#include "pool.hpp"

#include "server.hpp"
#include "client.hpp"
#include "file_client.hpp"

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

void testConnection(std::shared_ptr<Pool> pool)
{
    auto conn = pool->Connection();

    std::string test_str = "SELECT max(id) FROM demo; " ;
    PQsendQuery(conn->Connection().get(), test_str.c_str());

    while ( auto res_ = PQgetResult(conn->Connection().get())) {
        if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
            auto ID = PQgetvalue (res_ ,0, 0);
            std::cerr << ID << "\n";
        }

        if (PQresultStatus(res_) == PGRES_FATAL_ERROR){
            std::cerr << PQresultErrorMessage(res_) << "\n";
        }

        PQclear(res_);
    }

    pool->FreeConnection(conn);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, prn_sig);
    try {
        const char *srv_addr = "127.0.0.1";
        int srv_port = 1050;

        // Pool connection into bases
        ptr_pool_t pool = std::make_shared<Pool>();
        std::vector<std::shared_ptr<std::thread>> vec;

        for ( size_t i = 0; i< 50 ; ++i ){
            vec.push_back(std::make_shared<std::thread>(std::thread(testConnection, pool)));
        }

        for(auto &i : vec) {
            i.get()->join();
        }

        // Run servers
        new Server(srv_addr, srv_port);
        new Server(srv_addr, srv_port + 1);

        for (auto srv : Server::servers) {
            srv->Run();
        }

        // Run clients
        new Client();
        new Client();
        new Client();

        for (auto cl : Client::clients) {
            cl->Run();
        }

        std::cerr << "Servers count: " << Server::servers.size() << "\n";
        std::cerr << "Clients count: " << Client::clients.size() << "\n";

        //sleep(-1);
        FileClient file_cl;
        //while(true)
            file_cl.GetMsg();
    }
    catch (std::exception) {
        std::cerr << "Exception\n";
    }

    std::cerr << "End\n";

    return 0;
}