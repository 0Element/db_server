#pragma once

#include <string>
//#include <doublefann.h>
#include <floatfann.h>

namespace ltools
{
    std::string base64_encode(unsigned char const*, unsigned long len);
    std::string base64_decode(std::string const& s);

    char* str2hex(char* pdata, long ld, char* pbuf, long lb, char pdel = 0);
    char* hex2str(char* pdata, long ld, char* pbuf, long lb, char pdel = 0);

    int64_t get_milli_sec(void);

    void getsetsymbols(double pd[], const char* ps, int len);

    class Normalization
    {
     unsigned nobjs;
     unsigned nallobjs;
     unsigned nitems;
     double** pobjs;
     double* pmin;
     double* pmax;
     double* pmean;
    public:
     Normalization(unsigned nitems, unsigned nallobjs = 64*1024)
     {
          nobjs = 0;
          this->nitems = nitems;
          this->nallobjs  = nallobjs;
          pobjs = (double**)new double[nallobjs+1];
          memset(pobjs, 0, sizeof(double)*(nallobjs+1));
          pmin  = new double[nitems+1];
          memset(pmin, 0, sizeof(double)*(nitems+1));
          pmax  = new double[nitems+1];
          memset(pmax, 0, sizeof(double)*(nitems+1));
          pmean = new double[nitems+1];
          memset(pmean, 0, sizeof(double)*(nitems+1));
     }
     virtual ~Normalization()
     {
         if(pmin)  delete(pmin);
         if(pmax)  delete(pmax);
         if(pmean) delete(pmean);
          if(pobjs)  {
              int i = 0;
              while(i < nallobjs) {
                   if(pobjs[i]) delete(pobjs[i]);
                   i++;
              }
              delete(pobjs);
          }
     }
     unsigned getnitems() {return(nitems);}
     unsigned getnallobjs() {return(nallobjs);}
     unsigned getnobjs() {return(nobjs);}
     int add(double pd[])
     {
          int st = -1;
          if(nobjs < nallobjs-1) {
              if(nobjs == 0)  {
                  memcpy(pmin,  pd, sizeof(double)*nitems);
                  memcpy(pmax,  pd, sizeof(double)*nitems);
               }
              pobjs[nobjs] = new double[nitems+1];
              memcpy(pobjs[nobjs], pd, sizeof(double)*nitems);
              int i = 0;
              while(i < nitems) {
                  if(pmin[i] > pd[i]) pmin[i] = pd[i];
                  if(pmax[i] < pd[i]) pmax[i] = pd[i];
                  pmean[i] += pd[i];
                  i++;
              }

              nobjs++;
          }
          return st;
     }
     void calc()
     {
          unsigned i = 0;
          while(i < nitems) {
              unsigned j = 0;
              while(j < nobjs) {
                   pobjs[j][i] = (pobjs[j][i] - pmin[i]) / (pmax[i] - pmin[i]);
                  j++;
              }
              i++;
          }
     }

    };
/////////////////////////FeatureSet//////////////////////////////////////
#define _LenFeatureSet_ 128
    void normalization(fann_type d[], int len);
    void normalization1(fann_type d[], int len);
    std::string makepath(const char* pnfile, const char* pdir = NULL);
    void prepareString2FeatureSet(char* pstr, unsigned plen, fann_type featureset[_LenFeatureSet_], int type = 0x01);
    int prepareTextFile2FeatureSetFile(std::string path_reqs, int attak, FILE* pf, int step = 0);
    double* restoreData(const char* path, int& vecLen);

}

