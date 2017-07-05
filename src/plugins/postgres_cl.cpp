#include "postgres_cl.hpp"


int PostgresCl::count = 0;

PostgresCl::PostgresCl()
{
    db_host = "localhost";
    db_port = 5432;
    db_name = "test";
    db_user = "waf";
    db_pass = "waf";

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
       throw std::runtime_error( PQerrorMessage( m_connection.get() ) );
    }

    PostgresCl::count++;
}

PostgresCl::~PostgresCl()
{
    PostgresCl::count--;
}

ptr_pgconn_t PostgresCl::Connection() const
{
    return m_connection;
}
