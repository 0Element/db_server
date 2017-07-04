#include "lldb.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>


std::mutex mutex_db;


void f_db(std::string name, int count)
{
    time_t timer;
    ltools::shr_lldb lldb;

    try {
        lldb = ltools::Lldb::Init("/tmp/reqdb");
    }
    catch (std::exception) {
        std::cerr << "Error connect to db\n";
    }

    int i = 0;
    int tmp_sec = 0;
    int tmp_count = 0;
    while(true) {
        i++;
        timer = time(0);

        tm *ltm = localtime(&timer);

        lldb->Put("key" + std::to_string(i), "value" + std::to_string(i));

        if (ltm->tm_sec != tmp_sec) {
            tmp_sec = ltm->tm_sec;
            std::cerr << (i - tmp_count) << "\n";
            tmp_count = i;
        }
        //std::cerr << "ltm->tm_sec: " << ltm->tm_sec << "\n";

        if (i > count) break;
    }

    std::cout << "count:" << lldb->Each() << ":\n";
}

int main(int argc, char const *argv[])
{
    std::thread thr(f_db, "thr", 1000000);
    thr.join();

/*    std::thread thr1(f_db, "thr1");
    std::thread thr2(f_db, "thr2");
    std::thread thr3(f_db, "thr3");

    thr1.join();
    thr2.join();
    thr3.join();

    std::thread thr4(f_db, "thr1");
    std::thread thr5(f_db, "thr2");
    std::thread thr6(f_db, "thr3");

    thr4.join();
    thr5.join();
    thr6.join();
*/
    return 0;
}
