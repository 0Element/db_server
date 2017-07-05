#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <condition_variable>

#include <libpq-fe.h>

#include "postgres_cl.hpp"


class Pool;
typedef std::shared_ptr<Pool> ptr_pool_t;
typedef std::shared_ptr<PostgresCl> ptr_pg_cl_t;
typedef std::queue<ptr_pg_cl_t> q_pg_cl_t;

class Pool
{
public:
    Pool();
    ptr_pg_cl_t Connection();
    void FreeConnection(ptr_pg_cl_t);

private:
    void CreatePool();

    std::mutex mutex_;
    std::condition_variable condition_;
    q_pg_cl_t m_pool;

    const int POOL = 10;
};
