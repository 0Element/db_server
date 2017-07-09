
#include "prototype.h"

proto_value::proto_value(proto_types t, uint8_t* d)
{
 if(_checktypes(t))
 {
  if(d)
  {
//   char* pp = (char*)d;
   unsigned ln = strlen((char*)d);
   if(ln > 0)
   {
    isnull = false;
    if(_isbin(t))
    {
     unsigned len = atoi((char*)d);
//     char* pp1 = (char*)d+ln+1;
     proto_bin(d+ln+1, len);
    }
    else
    {
     memcpy(id.pn, (char*)d, ln); *(id.pn+ln) = 0;
     m_len = ln+1;
    }
   }
   it = t;
  }
 }
}

proto_value::proto_value(bool d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_BOOL;
 d ? *(id.pn) = '1' : *(id.pn) = '0';
 *(id.pn+1) = 0;
 m_len = 1+1;
}

proto_value::proto_value(int32_t d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_INT;
 snprintf(id.pn, _LEN_CHARNUMBER_, "%d", d);
 m_len = strlen(id.pn)+1;
}

proto_value::proto_value(uint32_t d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_UINT;
 snprintf(id.pn, _LEN_CHARNUMBER_, "%u", d);
 m_len = strlen(id.pn)+1;
}

proto_value::proto_value(int64_t d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_LONG;
 snprintf(id.pn, _LEN_CHARNUMBER_, "%lld", d);
 m_len = strlen(id.pn)+1;
}

proto_value::proto_value(uint64_t d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_ULONG;
 snprintf(id.pn, _LEN_CHARNUMBER_, "%llu", d);
 m_len = strlen(id.pn)+1;
}

proto_value::proto_value(double d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_DOUBLE;
 int ret = snprintf(id.pn, _LEN_CHARNUMBER_, "%g", d);
 if(ret > 0)isnull = false;
 m_len = strlen(id.pn)+1;
}

proto_value::proto_value(uint8_t* pd, unsigned len)
{
 proto_bin(pd, len);
}

proto_value::proto_value(char* pd, unsigned len)
{
 proto_bin((uint8_t*)pd, len);
}

proto_value::proto_value(wchar_t* pd, unsigned len)
{
 isnull = true;
 proto_bin((uint8_t*)pd, len*sizeof(wchar_t));
 it = T_STR;
}

proto_value::proto_value(char* pd)
{
 long len = strlen(pd);
 if(pd && len >= 0)
 {
  isnull = true;
  proto_bin((uint8_t*)pd, len);
 }
 it = T_CSTR;
}

proto_value::proto_value(wchar_t* pd)
{
 long len = wcslen(pd);
 if(pd && len >= 0)
 {
  isnull = true;
  proto_bin((uint8_t*)pd, len*sizeof(wchar_t));
 }
 it = T_STR;
}

void proto_value::proto_hex(uint8_t* pd, unsigned len)
{
 isnull = true;
 uint8_t* pd_out = new uint8_t[len*2+20];
 ltools::str2hex((char*)pd, len, (char*)pd_out, len*2, 0);
 proto_bin(pd_out, len*2);
 delete((char*)pd_out);
 it = T_HEX;
}

void proto_value::proto_base64(uint8_t* pd, unsigned len)
{
 isnull = true;
 std::string sb64 = ltools::base64_encode((unsigned char const*)pd, len);
 len = (long)sb64.length();
 pd = (uint8_t*)sb64.c_str();
 proto_bin(pd, len);
 it = T_BASE64;
}

void proto_value::proto_time(time_t d)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = false;
 it = T_TIME;
 snprintf(id.pn, _LEN_CHARNUMBER_, "%lu", d);
 m_len = strlen(id.pn)+1;
}

void proto_value::proto_bin(uint8_t* pd, unsigned len)
{
 std::lock_guard<std::mutex> lock(mtx);
 isnull = true;
 it = T_BIN;
 if(id.pd) delete((char*)id.pd); id.pd = NULL;
 m_len = 0;
 if(len >= 0)
 {
  if(len) isnull = false;
  snprintf(id.pn, _LEN_CHARNUMBER_, "%u", len);
  long ln = strlen(id.pn);
  id.pd = (void*)new uint8_t[len+ln+1+10];
  strncpy((char*)id.pd, (const char*)id.pn, (long)ln);
  *((char*)id.pd+ln) = 0;
  memcpy(((char*)id.pd)+ln+1, pd, len);
  m_len = len+ln+1;
 }
}

void proto_value::proto_list(uint8_t* pd, unsigned len)
{
 proto_bin(pd, len);
 it = T_LIST;
}

void proto_value::proto_dict(uint8_t* pd, unsigned len)
{
 proto_bin(pd, len);
 it = T_DICT;
}

void proto_value::setnull()
{
 std::lock_guard<std::mutex> lock(mtx);
 if(!isnull)
 {
  switch(it)
  {
   case T_STR   :
   case T_CSTR  :
   case T_BIN   :
   case T_HEX   :
   case T_BASE64:
   case T_LIST  :
   case T_DICT  :
   {
    if(id.pd) delete((char*)id.pd);
    id.pd = NULL;
   }
   break;
   default:
   break;
  }
 }
 *id.pn = 0;
 isnull = true;
}

unsigned proto_value::getlen()
{
 return m_len;
}

void* proto_value::getvalue()
{
 std::lock_guard<std::mutex> lock(mtx);
 void* st = NULL;
 if(!_isbin(it)) {
     st = id.pn;
 }
 else {
     st = id.pd;
 }
 return st;
}

bool proto_value::get_bool()
{
 std::lock_guard<std::mutex> lock(mtx);
 bool st = false;
 if(!isnull && it == T_BOOL) {
    if(id.pn[0]) st = true;
 }
 return st;
}

int32_t proto_value:: get_int()
{
 std::lock_guard<std::mutex> lock(mtx);
 int32_t st = 0;
 if(!_isbin(it))
 {
  st = atoi(id.pn);
 }
 return st;
}

uint32_t proto_value::get_uint()
{
 std::lock_guard<std::mutex> lock(mtx);
 uint32_t st = 0;
 if(!_isbin(it))
 {
  st = (unsigned)atoi(id.pn);
 }
 return st;
}

int64_t proto_value::get_long()
{
 std::lock_guard<std::mutex> lock(mtx);
 int64_t st = 0;
 if(!_isbin(it))
 {
  st = (int64_t)atoll(id.pn);
 }
 return st;
}

uint64_t proto_value::get_ulong()
{
 std::lock_guard<std::mutex> lock(mtx);
 unsigned st = 0;
 if(it == T_ULONG)
 {
  st = (uint64_t)atoll(id.pn);
 }
 return st;
}
double proto_value::get_double()
{
 std::lock_guard<std::mutex> lock(mtx);
 double st = 0;
 if(it == T_DOUBLE)
 {
  st = atof(id.pn);
 }
 return st;
}

uint8_t* proto_value::get_bin(unsigned &len)
{
 std::lock_guard<std::mutex> lock(mtx);
 len = 0;
 uint8_t* pd = NULL;
 if(_isbin(it))
 {
  char* p1 = strchr((char*)id.pd, 0);
  if(p1)
  {
   pd = (uint8_t*)(p1+1);
   len = atol((char*)id.pd);
  }
 }
 return pd;
}

char* proto_value::get_cstr(unsigned &len)
{
 char* pd = (char*)get_bin(len);
 return pd;
}

wchar_t* proto_value::get_str(unsigned &len)
{
 wchar_t* pd = NULL;
 if(it == T_STR)
 {
  pd = (wchar_t*)get_bin(len);
  len = len / 2;
 }
 return pd;
}

bool proto_value::get_hex(std::string& spd, unsigned &len)
{
 std::lock_guard<std::mutex> lock(mtx);
 bool st = false;
 len = 0;
 if(it == T_HEX)
 {
  char* p1 = strchr((char*)id.pd, 0);
  if(p1)
  {
   st = true;
   unsigned long len1 = atol((char*)id.pd);
   if(len1)
   {
    char* pd1 = p1+1;
    char* pbuf = new char[len1+1];
    ltools::hex2str(pd1, len1, pbuf, len1, 0);
    len = len1/2;
    spd.assign(pbuf, len);
    delete(pbuf);
   }
  }
 }
 return st;
}

bool proto_value::get_base64(std::string& spd, unsigned& len)
{
 bool st = false;
 std::lock_guard<std::mutex> lock(mtx);
 len = 0;
 if(it == T_BASE64)
 {
  char* p1 = strchr((char*)id.pd, 0);
  if(p1)
  {
   st = true;
   unsigned long len1 = atol((char*)id.pd);
   if(len1)
   {
    char* pd1 = p1+1;
    std::string s1(pd1, len1);
    spd = ltools::base64_decode(s1);
    len = spd.length();
   }
  }
 }
 return st;
}
