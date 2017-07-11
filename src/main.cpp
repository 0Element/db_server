#include <thread>
#include <iostream>
#include <exception>

#include <unistd.h>
#include <signal.h>

#include "server.hpp"
#include "client.hpp"
#include "pool.hpp"
#include "sql_adapter.hpp"
#include "sock.hpp"

#include "file_client.hpp"
#include "postgres_cl.hpp"

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

void test_pool_pg(ptr_pool_pg_t pool_pg, std::string sql_line, int port)
{
    // Pool connection into bases
    ptr_pg_cl_t conn = pool_pg->PopConn();

    if (conn->Exec(sql_line) != 0) {
        throw (std::exception());
    }
    Write(port, 0, "ok");

    pool_pg->PushConn(conn);
}

void test_pool(std::string sql_line, int port)
{
    ptr_pool_pg_t pool_pg = std::make_shared<pool_pg_t>(5);

    std::thread thr = std::thread(test_pool_pg, pool_pg, sql_line, port);
    thr.join();
}

/*void test_sql()
{
    ptr_pool_pg_t pool_pg = std::make_shared<pool_pg_t>(5);

    std::thread thr;
    std::string sql_line;

    set_condition("id_buf = \'123\'");

    /////////// VECTOR ////////////
    std::vector<std::string> keys = {"id_buf","from_ip","method"};
    std::vector<std::string> vals = {"\'111\'","\'12.7.7.7\'","\'GET\'"};

    sql_line = create_sql(INSERT, keys, vals);
    std::cerr << "SQL INSERT:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(SELECT, keys, vals);
    std::cerr << "SQL SELECT:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(UPDATE, keys, vals);
    std::cerr << "SQL UPDATE:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(DELETE, keys, vals);
    std::cerr << "SQL DELETE:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    /////////// STRING ////////////
    std::string str_keys("id_buf,from_ip,method");
    std::string str_vals("\'111\',\'12.7.7.7\',\'GET\'");

    sql_line = create_sql(INSERT, str_keys, str_vals);
    std::cerr << "SQL INSERT:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(SELECT, str_keys, str_vals);
    std::cerr << "SQL SELECT:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(UPDATE, str_keys, str_vals);
    std::cerr << "SQL UPDATE:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();

    sql_line = create_sql(DELETE, str_keys, str_vals);
    std::cerr << "SQL DELETE:" << sql_line <<":\n";

    thr = std::thread(test_pool_pg, pool_pg, sql_line);
    thr.join();
}*/

int main(int argc, char const *argv[])
{
    signal(SIGINT, prn_sig);

    try {
        const char *srv_addr = "127.0.0.1";
        int srv_port = 1050;

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

        //test_sql();

        PostgresCl pcl;

        while (1) {
            port_msg_t port_msg = pcl.GetMsg();
            std::cerr << "port_msg.first:" << port_msg.first << ":\n";
            std::cerr << "port_msg.second:" << port_msg.second << ":\n";
            try {
                std::string sql_line = get_sql(port_msg.second);
                if (sql_line.empty()) throw(std::exception());

                std::cerr << "main sql_line: " << sql_line << "\n";
                test_pool(sql_line, port_msg.first);
            } catch (...){
                std::cerr << "catch\n";
                Write(port_msg.first, -1, "parse json error");
            }
        }

        //test_proto();
    }
    catch (std::exception ex) {
        std::cerr << "Exception:" << ex.what() << "\n";
    }

    std::cerr << "End\n";

    return 0;
}