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

#include "proto.h"


void prn_sig(int sig)
{
    std::cerr << "SIGINT\n";

    for (auto srv : Server::servers) {
        srv->Stop();
    }
}

void test_proto()
{
    // Add in dictdata
    dictdata dh;
    std::shared_ptr<proto_value> pv1(new proto_value("111"));
    std::shared_ptr<proto_value> pv2(new proto_value("2222"));

    dh.add("dh1", pv1);
    dh.add("dh2", pv2);

    auto pv1_get = dh.get("dh1");
    auto pv2_get = dh.get("dh2");

    std::cerr << "dh get1:" << (char*)pv1_get->getvalue() << ":\n";
    std::cerr << "dh get2:" << (char*)pv2_get->getvalue() << ":\n";
}

void test_pool_pg(ptr_pool_pg_t pool_pg)
{
    // Pool connection into bases
    ptr_pg_cl_t conn = pool_pg->PopConn();

    //std::string test_str = "SELECT max(id) FROM access;";
    std::string sql_line = "SELECT * FROM modsec WHERE id = 1;";

    if (conn->Exec(sql_line) != 0) {
        throw (std::exception());
    }

    pool_pg->PushConn(conn);
}

void test_pool()
{
    ptr_pool_pg_t pool_pg = std::make_shared<pool_pg_t>(5);

    std::thread thr = std::thread(test_pool_pg, pool_pg);
    thr.join();
}

void test_sql()
{
    char* sql_line;
    std::vector<std::string> keys = {"key1","key2","key3"};
    std::vector<std::string> vals = {"val1","val2","val3"};

    sql_line = create_sql(INSERT, keys, vals);
    std::cerr << "SQL INSERT:" << sql_line <<":\n";
    free(sql_line);

    sql_line = create_sql(SELECT, keys, vals);
    std::cerr << "SQL SELECT:" << sql_line <<":\n";
    free(sql_line);

    sql_line = create_sql(UPDATE, keys, vals);
    std::cerr << "SQL UPDATE:" << sql_line <<":\n";
    free(sql_line);

    sql_line = create_sql(DELETE, keys, vals);
    std::cerr << "SQL DELETE:" << sql_line <<":\n";
    free(sql_line);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, prn_sig);

    try {
        const char *srv_addr = "127.0.0.1";
        int srv_port = 1050;

        test_sql();
        //test_pool();
        //test_proto();

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