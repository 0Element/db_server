
#include "dictdata.h"
#include "prototype.h"


  bool dictdata::parse(char* pd)
  {
   bool st = false;
   if(pd)
   {
	int len = 0;
    if(strncmp(pd, _magichead_dd_, strlen(_magichead_dd_)) == 0)
	{
 	 len = strlen(_magichead_dd_);
 	 int nb = atoi(pd+len);
 	 if(nb > 0)
 	 {
      st = true;
      len += strlen(pd+len)+1;
//begin body dict
      pd = pd+len;
      len = 0;
//
      while(len < nb)
      {
//get type
   	   proto_types pt = (proto_types)*(pd+len); len++;
       if(_checktypes(pt))
       {
//get key
    	std::string key(pd+len);
        len += key.length()+1;
//get data
//        char* p1 = pd+len;
        std::shared_ptr <proto_value> pv(new proto_value(pt, (uint8_t*)(pd+len)));
        len += pv->m_len;
        add(key, pv);
       }
      }
 	 }
	}
   }
   return st;
  }

 bool dictdata::add(std::string key, std::shared_ptr<proto_value> pv)
 {
  bool st = false;
  try
  {
   m_protomap.insert(std::pair<std::string, std::shared_ptr<proto_value>>(key, pv));
   st = true;
  }
  catch(...){}
  return st;
 }

 std::shared_ptr<proto_value> dictdata::get(unsigned n)
 {
  std::shared_ptr<proto_value> pv;
  try
  {
   if(m_protomap.size())
   {
    if(n >= m_protomap.size())
    {
     t_protomap::const_reverse_iterator  it = m_protomap.rbegin();
     pv = it->second;
    }
    else
    {
     t_protomap::const_iterator it = m_protomap.begin();
     std::advance(it, n);
     pv = it->second;
    }
   }
  }
  catch(...){}
  return pv;
 }

 std::shared_ptr<proto_value> dictdata::get(std::string key)
 {
  std::shared_ptr<proto_value> pv;
  if(m_protomap.count(key))
  {
   pv = m_protomap[key];
  }
  return pv;
 }

 bool dictdata::del(std::string key)
 {
  bool st = false;
  if(m_protomap.size())
  {
   if(m_protomap.count(key))
   {
    st = true;
   }
  }
  return st;
 }

 bool dictdata::del(unsigned n)
 {
  bool st = false;
  if(m_protomap.size())
  {
   if(n >= m_protomap.size())
   {
    n = m_protomap.size() - 1;
   }
   t_protomap::const_iterator it = m_protomap.begin();
   std::advance(it, n);
   m_protomap.erase(it);
   st = true;
  }
  return st;
 }

 char* dictdata::getstr(unsigned& len)
 {
  char* pd = NULL;
  len = 0;
  if(m_protomap.size())
  {
//get len
   t_protomap::const_iterator it = m_protomap.begin();
   while(it != m_protomap.end())
   {
    std::shared_ptr<proto_value> pv = it->second;
//           data   type     key0
    len+=pv->m_len + 1 + it->first.length()+1;
    it++;
   }
   pd = new char[len+128];
   unsigned len1 = strlen(_magichead_dd_);;
   memcpy(pd, _magichead_dd_, len1);
   sprintf(pd+len1, "%d", len);  len1+= strlen(pd+len1); *(pd+len1) = 0; len1++;
   it = m_protomap.begin();
   while(it != m_protomap.end())
   {
    std::shared_ptr<proto_value> pv = it->second;
    //set type
    *(pd+len1) = (char)pv->it; len1++;
    //set key
    strncpy(pd+len1, it->first.c_str(), it->first.length()); len1+=it->first.length();
    *(pd+len1) = 0; len1++;
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

 unsigned dictdata::getlenstr(char* pd)
 {
      unsigned len = 0;
      if(pd) {
          unsigned len1 = strlen(_magichead_dd_);
          if(strncmp(pd, _magichead_dd_, len1) == 0) {
              len = atoi(pd+len1) + strlen(pd) + 1;
          }
      }
      return len;
 }

