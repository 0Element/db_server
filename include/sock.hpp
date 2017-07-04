#pragma once

#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>


void make_non_block(int sock);
int Read(int sock, char* buff);
void Write(int sock);
void Close(int sock);

char* Makeresponse(char* pbuf, const char* ptmpl, int st, const char* pmes);

//static const char *presponse;