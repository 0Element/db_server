
#pragma once

#include <exception>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <stdio.h>

#define _magichead_ld_  "ld"

class proto_value;
typedef  std::vector<std::shared_ptr<proto_value> >  t_protolist;
class listdata
{
 int err;
 t_protolist  m_protolist;
 public:
  listdata(char* pl = nullptr)
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
 virtual ~listdata()
  {
   m_protolist.clear();
  }
  unsigned size()
  {
   return m_protolist.size();
  }
  int geterr(){return err;}
  bool parse(char* pl);
  bool add(std::shared_ptr<proto_value> pv);
  bool insert(unsigned n, std::shared_ptr<proto_value> pv);
  std::shared_ptr<proto_value> get(unsigned n);
  std::shared_ptr<proto_value> del(unsigned n);
  char* getstr(unsigned& len);
  static unsigned getlenstr(char* pl);

 };
