#pragma once

#include <mutex>
#include <map>
#include <memory>
#include <string>
#include <stdint-gcc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.hpp"

#define _LEN_CHARNUMBER_ 128
typedef enum proto_types
{
 T_NONE                = 0,
 T_BOOL                = 1, //uint8_t 0/1
 T_INT                    , //int32_t
 T_UINT                   , //uint32_t
 T_LONG                   , //int64_t
 T_ULONG                  , //uint64_t
 T_DOUBLE                 , //double
 T_TIME                   , //time_t
 T_STR                    , //(uint32_t)(wchar_t) unicode
 T_CSTR                   , //char*
 T_BIN                    , //(uint32_t)(unsigned char)
 T_HEX                    , //bin->hex
 T_BASE64                 , //bin->base64
 T_LIST                   , //std::list->
 T_DICT                   , //std::map->
 T_ENDTYPES               ,
} proto_types;

#define  _isbin(t) ((t) == T_CSTR || (t) == T_STR || (t) == T_BIN || (t) == T_HEX || (t) ==  T_BASE64 || (t) == T_LIST || (t) == T_DICT)
#define  _checktypes(t) ((t) > T_NONE && (t) < T_ENDTYPES)

typedef struct proto_data
{
 char      pn[_LEN_CHARNUMBER_+1];
 void*     pd;//T_CSTR; T_STR; T_BIN; T_HEX; T_BASE64; T_LIST; T_DICT

 proto_data()
 {
  pn[0] = 0;
  pd = NULL;
 }

 virtual ~proto_data()
 {
  if(pd) delete((char*)pd);
 }
} proto_data;

typedef struct proto_item
{
 bool isnull;
 unsigned long m_len;
 proto_types it;
 proto_data  id;
 proto_item()
 {
  m_len = 0;
  isnull = true;
  it = T_NONE;
  memset(&id, 0, sizeof(id));
 }
 proto_types getype()
 {
  return it;
 }
} proto_item;

class proto_value : public proto_item
{
 std::mutex mtx;
 public:

 proto_value(proto_types t = T_NONE, uint8_t* d = NULL);
 proto_value(bool d);
 proto_value(int32_t d);
 proto_value(uint32_t d);
 proto_value(int64_t d);
 proto_value(uint64_t d);
 proto_value(double d);
 proto_value(uint8_t* pd, unsigned len);
 proto_value(char* pd, unsigned len);
 proto_value(wchar_t* pd, unsigned len);
 proto_value(char* pd);
 proto_value(wchar_t* pd);
 void proto_hex(uint8_t* pd, unsigned len);
 void proto_base64(uint8_t* pd, unsigned len);
 void proto_time(time_t d);
 void proto_bin(uint8_t* pd, unsigned len);
 void proto_list(uint8_t* pd, unsigned len);
 void proto_dict(uint8_t* pd, unsigned len);
 void setnull();
 bool is_null(){return(isnull);}
 proto_types get_type(){return(it);}
 unsigned getlen();
 void* getvalue();
 bool get_bool();
 int32_t get_int();
 uint32_t get_uint();
 int64_t get_long();
 uint64_t get_ulong();
 double get_double();
 uint8_t* get_bin(unsigned& len);
 char* get_cstr(unsigned& len);
 wchar_t* get_str(unsigned& len);
 bool get_hex(std::string& spd, unsigned& len);
 bool get_base64(std::string& spd, unsigned& len);

};

