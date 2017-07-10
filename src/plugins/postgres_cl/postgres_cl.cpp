#include <iostream>

#include "postgres_cl.hpp"


int PostgresCl::count = 0;

PostgresCl::PostgresCl()
{
    // TODO getting from config
    std::string db_host = "localhost";
    int db_port = 5432;
    std::string db_name = "testdb";
    std::string db_user = "bogdan";
    std::string db_pass = "0987654321";
    // ~TODO

    m_connection.reset(PQsetdbLogin(db_host.c_str(),
                                    std::to_string(db_port).c_str(),
                                    nullptr,
                                    nullptr,
                                    db_name.c_str(),
                                    db_user.c_str(),
                                    db_pass.c_str()),
                                    &PQfinish);

    if (PQstatus(m_connection.get()) != CONNECTION_OK &&
        PQsetnonblocking(m_connection.get(), 1) != 0)
    {
       throw std::runtime_error(PQerrorMessage(m_connection.get()));
    }

    PostgresCl::count++;
}

PostgresCl::~PostgresCl()
{
    PostgresCl::count--;
}

ptr_pgconn_t PostgresCl::Conn() const
{
    return m_connection;
}

int PostgresCl::Exec(std::string sql_line)
{
    PQsendQuery(Conn().get(), sql_line.c_str());

    while (auto res_ = PQgetResult(Conn().get())) {
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
            return -1;
        }

        PQclear(res_);
    }

    return 0;
}