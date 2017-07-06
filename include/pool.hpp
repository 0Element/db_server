#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <map>
#include <condition_variable>

#include <libpq-fe.h>

#include "postgres_cl.hpp"


template<class ClClass>
class Pool;

typedef std::shared_ptr<PostgresCl> ptr_pg_cl_t;
typedef Pool<ptr_pg_cl_t> pool_pg_t;
typedef std::shared_ptr<pool_pg_t> ptr_pool_pg_t;

typedef std::queue<ptr_pg_cl_t> q_pg_cl_t;

template<class ClClass>
class Pool
{
public:
    Pool(int count);
    ClClass PopConn();
    void PushConn(ClClass);

private:
    void CreatePool(int count);

    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<ClClass> m_pool;
};

template<class ClClass>
Pool<ClClass>::Pool(int count)
{
    CreatePool(count);
}

template<class ClClass>
void Pool<ClClass>::CreatePool(int count)
{
    std::lock_guard<std::mutex> lock(mutex_);

    for (int i = 0; i < count; ++i){
        m_pool.emplace(ClClass());
    }
}

template<class ClClass>
ClClass Pool<ClClass>::PopConn()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (m_pool.empty()){
        condition_.wait(lock);
    }

    ClClass cl = m_pool.front();
    m_pool.pop();

    return cl;
}

template<class ClClass>
void Pool<ClClass>::PushConn(ClClass cl)
{
    std::unique_lock<std::mutex> lock(mutex_);
    m_pool.push(cl);
    lock.unlock();
    condition_.notify_one();
}
