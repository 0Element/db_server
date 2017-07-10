#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <map>
#include <condition_variable>

#include "postgres_cl.hpp"


template<class ClClass>
class Pool;

typedef std::shared_ptr<PostgresCl> ptr_pg_cl_t;
typedef Pool<PostgresCl> pool_pg_t;
typedef std::shared_ptr<pool_pg_t> ptr_pool_pg_t;

template<class ClClass>
class Pool
{
public:
    Pool(int count);
    std::shared_ptr<ClClass> PopConn();
    void PushConn(std::shared_ptr<ClClass>);

private:
    void CreatePool(int count);

    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::shared_ptr<ClClass>> q_pool;
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
        q_pool.emplace(std::make_shared<ClClass>());
    }
}

template<class ClClass>
std::shared_ptr<ClClass> Pool<ClClass>::PopConn()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (q_pool.empty()){
        condition_.wait(lock);
    }

    std::shared_ptr<ClClass> cl = q_pool.front();
    q_pool.pop();

    return cl;
}

template<class ClClass>
void Pool<ClClass>::PushConn(std::shared_ptr<ClClass> cl)
{
    std::unique_lock<std::mutex> lock(mutex_);
    q_pool.push(cl);
    lock.unlock();
    condition_.notify_one();
}
