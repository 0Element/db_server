#include <memory>
#include <mutex>
#include <postgresql/libpq-fe.h>


typedef std::shared_ptr<PGconn> ptr_pgconn_t;

class PostgresCl
{
public:
    PostgresCl();
    ~PostgresCl();
    ptr_pgconn_t Conn() const;
    int Exec(std::string sql_line);

    static int count;

private:
    std::string db_host;
    int db_port;
    std::string db_name;
    std::string db_user;
    std::string db_pass;

    ptr_pgconn_t m_connection;
};
