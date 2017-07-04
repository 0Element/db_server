#include "lldb.hpp"

#include <iostream>

namespace ltools
{

std::shared_ptr<Lldb> Lldb::lldb = NULL;
std::mutex Lldb::mutex_db_;

std::shared_ptr<Lldb> Lldb::Init(std::string base_file_name)
{
    std::lock_guard<std::mutex> lock(mutex_db_);
    if (Lldb::lldb) {
        return ltools::Lldb::lldb;
    }
    else {
        Lldb::lldb = shr_lldb(new Lldb(base_file_name));
        return Lldb::lldb;
    }
}

Lldb::Lldb(std::string base_file_name)
{
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, base_file_name, &db);

    if (!status.ok()) {
        std::cerr << status.ToString() << "\n";
        throw "Error open db in levelDB";
    }
}

std::string Lldb::Get(std::string key_hash) {
    leveldb::Slice slice(key_hash);
    std::string value;

    mutex_db_.lock();
    leveldb::Status status = db->Get(leveldb::ReadOptions(), slice, &value);
    mutex_db_.unlock();
    if (status.ok()) {
        std::cout << "value:" << value << ":\n";
        std::cout << "size:" << value.size() << ":\n";
        return value;
    }
    else
        return "";
}

std::string Lldb::Get(size_t key_hash) {
    return Get(std::to_string(key_hash));
}

int Lldb::Put(std::string key_hash, std::string header_str) {
    leveldb::Slice slice_key(key_hash);
    leveldb::Slice slice_value(header_str);

    mutex_db_.lock();
    leveldb::Status status = db->Put(leveldb::WriteOptions(), slice_key, slice_value);
    mutex_db_.unlock();
    if (status.ok())
        return 0;
    else
        return -1;
}

int Lldb::Put(size_t header_hash, std::string header_str) {
    return Put(std::to_string(header_hash), header_str);
}

int Lldb::Delete(std::string key) {
    leveldb::Slice slice_key(key);

    leveldb::Status status = db->Delete(leveldb::WriteOptions(), slice_key);
    if (status.ok())
        return 0;
    else
        return -1;
}

int Lldb::Delete(size_t header_hash) {
    leveldb::Slice slice_key(std::to_string(header_hash));

    leveldb::Status status = db->Delete(leveldb::WriteOptions(), slice_key);
    if (status.ok())
        return 0;
    else
        return -1;
}

int Lldb::Each() {
    int result = 0;
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next(), result++) {
        //std::cout << it->key().ToString() << ": "  << it->value().ToString() << "\n";
        Delete(it->key().ToString());
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;

    return result;
}



}
