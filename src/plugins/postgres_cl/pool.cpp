#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include "pool.hpp"


Pool::Pool()
{
    CreatePool();
}

void Pool::CreatePool()
{
    std::lock_guard<std::mutex> lock(mutex_);

    for (int i = 0; i < POOL; ++i){
        m_pool.emplace(std::make_shared<PostgresCl>());
    }
}

std::shared_ptr<PostgresCl> Pool::Connection()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (m_pool.empty()){
        condition_.wait(lock);
    }

    ptr_pg_cl_t conn_ = m_pool.front();
    m_pool.pop();

    return conn_;
}

void Pool::FreeConnection(ptr_pg_cl_t conn_)
{
    std::unique_lock<std::mutex> lock(mutex_);
    m_pool.push(conn_);
    lock.unlock();
    condition_.notify_one();
}
