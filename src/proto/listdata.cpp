
#include "listdata.h"
#include "prototype.h"

  bool listdata::parse(char* pl)
  {
   bool st = false;
   if(pl)
   {
	int len = 0;
    if(strncmp(pl, _magichead_ld_, strlen(_magichead_ld_)) == 0)
	{
 	 len = strlen(_magichead_ld_);
 	 int nb = atoi(pl+len);
 	 if(nb > 0)
 	 {
      st = true;
      len += strlen(pl+len)+1;
//begin body list
      pl = pl+len;
      len = 0;
//
      while(len < nb)
      {
   	   proto_types pt = (proto_types)*(pl+len); len++;
       if(_checktypes(pt))
       {
        std::shared_ptr <proto_value> pv(new proto_value(pt, (uint8_t*)(pl+len)));
        add(pv);
        len += pv->m_len;
       }
      }
 	 }
	}
   }
   return st;
  }

  bool listdata::add(std::shared_ptr<proto_value> pv)
  {
   bool st = false;
   try
   {
    m_protolist.push_back(pv);
    st = true;
   }
   catch(...){};

   return st;
  }

  bool listdata::insert(unsigned n, std::shared_ptr<proto_value> pv)
  {
   bool st = false;
   if(n >= m_protolist.size())
   {
	st =  add(pv);
   }
   else
   {
    try
    {
     t_protolist::iterator it = m_protolist.begin();
     m_protolist.insert(it, n, pv);
     st = true;
    }
    catch(...){};
   }
   return st;
  }

  std::shared_ptr<proto_value> listdata::get(unsigned n)
  {
   std::shared_ptr<proto_value> pv;
   try
   {
    if(n >= m_protolist.size())
    {
     pv =  m_protolist.back();
    }
    else
    {
     pv = m_protolist[n];
    }
   }
   catch(...){};
   return pv;
  }

  std::shared_ptr<proto_value> listdata::del(unsigned n)
  {
   std::shared_ptr<proto_value> pv = get(n);
   if(pv)
   {
    m_protolist.erase(m_protolist.begin()+n);
   }
   return pv;
  }

  char* listdata::getstr(unsigned& len)
  {
   char* pd = NULL;
   len = 0;
   if(m_protolist.size())
   {
 //get len
    t_protolist::iterator it = m_protolist.begin();
    while(it != m_protolist.end())
    {
     std::shared_ptr<proto_value> pv = *it;
 //           data   type
     len+=pv->m_len + 1;
     it++;
    }
    pd = new char[len+128];
    unsigned len1 = strlen(_magichead_ld_);;
    memcpy(pd, _magichead_ld_, len1);
    sprintf(pd+len1, "%d", len);  len1+= strlen(pd+len1); *(pd+len1) = 0; len1++;
    it = m_protolist.begin();
    while(it != m_protolist.end())
    {
     std::shared_ptr<proto_value> pv = *it;
     //set type
     *(pd+len1) = (char)pv->it; len1++;
     //set data
     if(_isbin(pv->it))
     {
      memcpy(pd+len1, pv->id.pd, pv->m_len); len1 += pv->m_len;
     }
     else
     {
      memcpy(pd+len1, pv->id.pn, pv->m_len+1); len1 += pv->m_len+1;
     }
     it++;
    }
    len = len1;
   }
   return pd;
  }

  unsigned listdata::getlenstr(char* pl)
  {
       unsigned len = 0;
       if(pl) {
           unsigned len1 = strlen(_magichead_ld_);
           if(strncmp(pl, _magichead_ld_, len1) == 0) {
               len = atoi(pl+len1) + strlen(pl) + 1;
           }
       }
       return len;
  }
