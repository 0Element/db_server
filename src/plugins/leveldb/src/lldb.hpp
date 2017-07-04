#pragma once

#include <memory>
#include <mutex>
#include <leveldb/db.h>

namespace ltools
{

class Lldb;

typedef std::shared_ptr<Lldb> shr_lldb;

class Lldb
{
public:
    virtual ~Lldb() {
        delete db;
    };

    static shr_lldb Init(std::string base_file_name);

    std::string Get(size_t header_hash);
    std::string Get(std::string key_hash);

    int Put(size_t header_hash, std::string header_str);
    int Put(std::string key_hash, std::string header_str);

    int Each();

    int Delete(size_t header_hash);
    int Delete(std::string key);

private:
    Lldb(std::string base_file_name);

    leveldb::DB *db;
    static shr_lldb lldb;
    static std::mutex mutex_db_;
};

}
