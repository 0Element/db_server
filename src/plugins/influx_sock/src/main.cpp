#include <iostream>

#include "infl.hpp"

#include <unistd.h>
#include <chrono>

int main(int argc, char *argv[])
{
    std::string addr("127.0.0.1");
    int port = 8086;
    int count_msgs = 1000 * 1;
    int count_thrs = 500;
//1000
//1   53,18,18 44,22,22
//2   50,20,10 51,19,9
//3   48,20,6  47,21,7
//4   48,20,5  45,22,5
//5   45,22,4  49,20,4
//10  45,22,2  49,20,2
//20  48,20,1  47,21,1
//50  45,22,0  47,21,0
//100 44,22,0  47,21,0
//1000 12,83,0 13,76,0
     /*int tmp_sec = 0;
    int tmp_count = 0;
        if (ltm->tm_sec != tmp_sec) {
            tmp_sec = ltm->tm_sec;
            std::cerr << (i - tmp_count) << ":" << i << "\n";
            tmp_count = i;
        }*/

    Influx *infl = new Influx(addr.c_str(), port, count_msgs, count_thrs);
    //infl->GenMessages(count_msgs);
    infl->SendRecv();

    time_t timer = time(0);
    //tm *ltm = localtime(&timer);
    //std::cerr << ltm->tm_min << ":" << ltm->tm_sec << "\n";

    std::cerr << "RUN\n";
    infl->run = true;
    //infl->Join();
    //infl->Detach();

    while (infl->count_thrs > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    time_t timer2 = time(0);
    //tm *ltm2 = localtime(&timer2);
    //std::cerr << ltm2->tm_min << ":" << ltm2->tm_sec << "\n";

    time_t timer3 = timer2 - timer;
    tm *ltm3 = localtime(&timer3);
    std::cerr << ltm3->tm_min << ":" << ltm3->tm_sec << "\n";
    if (count_msgs) {
        int time_sec = (ltm3->tm_sec + ltm3->tm_min * 60);
        if (time_sec) {
            std::cerr << count_msgs/time_sec << "\n";
            std::cerr << count_msgs/time_sec/count_thrs << "\n";
        }
    }


//        std::cerr << infl->count_thrs << "\n";
    //sleep(5);

    return 0;
}

/*
POST /write?db=example HTTP/1.1
Host: localhost:8086
User-Agent: curl/7.47.0
*/
//Accept: */*
/*
Content-Length: 56
Content-Type: application/x-www-form-urlencoded

cpu_load_short,host=server01,region=us-west value="0.64"
*/
//curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server01,region=us-west value=0.64'
