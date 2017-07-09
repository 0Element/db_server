
#pragma once

#include <map>
#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#define _magichead_dd_  "dd"

class proto_value;
typedef  std::map<std::string, std::shared_ptr<proto_value> > t_protomap;
class dictdata
{
 int err;
 t_protomap  m_protomap;
 public:
 dictdata(char* pl = nullptr)
 {
  err = 0;
  if(pl)
  {
   if(!parse(pl))
   {
    err = -1;
   }
  }
 }
 virtual ~dictdata()
  {
   m_protomap.clear();
  }
  unsigned size()
  {
   return m_protomap.size();
  }
  bool parse(char* pd);
  bool add(std::string key, std::shared_ptr<proto_value> pv);
  std::shared_ptr<proto_value> get(unsigned n);
  std::shared_ptr<proto_value> get(std::string key);
  bool del(std::string key);
  bool del(unsigned n);
  char* getstr(unsigned& len);
  static unsigned getlenstr(char* pd);
 };
