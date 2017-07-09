
#pragma once

#include "prototype.h"
#include "listdata.h"
#include "dictdata.h"

//////protocol atributes ///////////
#define  proto_sock_src      "sock_src"    //int
#define  proto_func_exec     "func_exec"   //string
#define  proto_lib_funcs     "lib_funcs"   //string
#define  proto_type_query    "type_query"  //uint32_t
#define  proto_type_query_only_request 0x01
#define  proto_type_query_zip_pack     0x02

////////////////////////////////////

dictdata* proto_get_header(uint8_t* proto);
void      proto_get(uint8_t* proto, dictdata*& ph, dictdata*& pd);
uint8_t*  proto_set_header(listdata& ld, dictdata& dh);
uint8_t*  proto_set(dictdata& dh, dictdata& dd, unsigned &len);


