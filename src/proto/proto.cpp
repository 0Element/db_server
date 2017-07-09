
#include "proto.h"

//proto list[dict1, dict2]
//dict1 - header
//dict2 - data

dictdata* proto_get_header(uint8_t* proto)
{
     dictdata* pdd = NULL;
     listdata ld((char*)proto);
     if(ld.geterr() == 0) {
         if(ld.size() > 0) {
            std::shared_ptr<proto_value> pv = ld.get(0);
            if(pv) {
                if(pv->getype() == T_DICT) {
                    unsigned len = 0;
                    pdd = new dictdata((char*)pv->get_bin(len));
                }
            }
         }
     }
     return pdd;
}

void proto_get(uint8_t* proto, dictdata*& ph, dictdata*& pd)
{
     ph = pd = NULL;
     listdata ld((char*)proto);
     if(ld.geterr() == 0) {
         if(ld.size() == 2) {
            std::shared_ptr<proto_value> pv = ld.get(0);
            if(pv) {
                if(pv->getype() == T_DICT) {
                    unsigned len = 0;
                    ph = new dictdata((char*)pv->get_bin(len));
                }
            }
            pv = ld.get(1);
            if(pv) {
                if(pv->getype() == T_DICT) {
                    unsigned len = 0;
                    pd = new dictdata((char*)pv->get_bin(len));
                }
            }
         }
     }
}

uint8_t* proto_set_header(listdata& ld, dictdata& dh)
{
     uint8_t* pl = NULL;
     ld.del(0);\
     unsigned len = 0;
     char* pd = dh.getstr(len);
     if(pd) {
         std::shared_ptr<proto_value> pv(new proto_value(pd, len));
         delete(pd);
         ld.insert(0, pv);
         pl = (uint8_t*)ld.getstr(len);
     }
     return pl;
}

uint8_t* proto_set(dictdata& dh, dictdata& dd, unsigned &len)
{
     uint8_t* pl = NULL;
     listdata ld;
     len = 0;
     char* pd = dh.getstr(len);
     if(pd) {
         std::shared_ptr<proto_value> pv(new proto_value());
         pv->proto_dict((uint8_t*)pd, len);
         ld.add(pv);
         delete(pd);
         pd = dd.getstr(len);
         if(pd) {
             std::shared_ptr<proto_value> pv(new proto_value());
             pv->proto_dict((uint8_t*)pd, len);
             ld.add(pv);
             delete(pd);
         }
         pl = (uint8_t*)ld.getstr(len);
     }
     return pl;
}
