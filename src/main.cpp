#include <thread>
#include <iostream>
#include <exception>

#include <unistd.h>
#include <signal.h>

#include "server.hpp"
#include "client.hpp"
#include "pool.hpp"
#include "file_client.hpp"
#include "sql_adapter.hpp"


void prn_sig(int sig)
{
    std::cerr << "SIGINT\n";

    for (auto srv : Server::servers) {
        srv->Stop();
    }
}

void testConnection(ptr_pool_pg_t pool)
{
    ptr_pg_cl_t conn = pool->PopConn();

    //std::string test_str = "SELECT max(id) FROM access;";
    std::string test_str = "SELECT * FROM modsec WHERE id = 1;";
    PQsendQuery(conn->Conn().get(), test_str.c_str());

    while ( auto res_ = PQgetResult(conn->Conn().get())) {
        if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
            auto ID = PQgetvalue (res_ ,0, 0);
            std::cerr << ID << "\n";

            int rows = PQntuples(res_);
            int cols = PQnfields(res_);
            std::cerr << "rows:" << rows << ":\n";
            std::cerr << "cols:" << cols << ":\n";
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    std::cerr << "PQgetvalue:" << PQgetvalue(res_, i, j) << ":\n";
                }
            }
        }

        if (PQresultStatus(res_) == PGRES_FATAL_ERROR){
            std::cerr << PQresultErrorMessage(res_) << "\n";
        }

        PQclear(res_);
    }

    pool->PushConn(conn);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, prn_sig);

    try {
        const char *srv_addr = "127.0.0.1";
        int srv_port = 1050;

        // Pool connection into bases
        auto pool_pg = std::make_shared<pool_pg_t>(5);

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
    catch (std::exception ex) {
        std::cerr << "Exception:" << ex.what() << "\n";
    }

    std::cerr << "End\n";

    return 0;
}