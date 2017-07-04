#pragma once

#include "db_plugin.hpp"


class FileClient : public Plugin
{
public:
    FileClient();
    ~FileClient();

    //void Print();
    virtual void GetMsg(std::string name_plug = "");

private:
};

char* makeresponse(char* pbuf, const char* ptmpl, int st, const char* pmes);
