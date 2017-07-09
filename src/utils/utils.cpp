#include <sys/timeb.h>
#include <iostream>
#include <math.h>
#include <memory.h>

#include <dirent.h>
#include <sys/stat.h>

//#include "httprs.hpp"
//#include "kmeans/kmeans.hpp"
#include "utils.hpp"
//#include "neural/neuralnet.h"

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

namespace ltools
{

    static inline bool is_base64(unsigned char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::string base64_encode(unsigned char const* bytes_to_encode, unsigned long in_len)
    {
        std::string ret;
        long i = 0, j = 0;
        unsigned char char_array_3[3], char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                ret += '=';

        }

        return ret;

    }

    std::string base64_decode(std::string const& encoded_string)
    {
        long in_len = encoded_string.size();
        long i = 0, j = 0, in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_];
            in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
                ret += char_array_3[j];
        }

        return ret;
    }

    //////////////////////////////////////////////////////////////////////

    char* str2hex(char* pdata, long ld, char* pbuf, long lb, char pdel)
    {
        *pbuf = 0;
        long ii = 0;
        long i = 0;
        while (i < ld && ii < lb) {
            if (pdel == 0) {
                sprintf(&pbuf[ii], "%02x", *(pdata + i) & 0x00ff);
                ii += 2;
            }
            else {
                if (i == 0) {
                    sprintf(&pbuf[ii], "%02x", *pdata & 0x00ff);
                    ii += 2;
                }
                else {
                    sprintf(&pbuf[ii], "%c%02x", pdel, *(pdata + i) & 0x00ff);
                    ii += 3;
                }
            }
            i++;
        }
        return (pbuf);
    }

    char* hex2str(char* pdata, long ld, char* pbuf, long lb, char pdel)
    {
        long n = 2;
        if (pdel) n = 3;
        long n1 = ld / n;
        long i = 0;
        while (i < n1 && i < lb) {
            if (pdel) {
                char c;
                sscanf(pdata, "%02x%c", &pbuf[i], &c);
            }
            else {
                sscanf(pdata, "%02x", &pbuf[i]);
            }
            pdata += n;
            i++;
        }
        if (pdata) {
            sscanf(pdata, "%02x", &pbuf[i]);
            pbuf[i + 1] = 0;
        }
        return (pbuf);
    }

    int64_t get_milli_sec(void)
    {
        int64_t t = 0;
        struct timeb timebuffer;
        ftime(&timebuffer);
        t = timebuffer.time;
        t = t * 1000 + timebuffer.millitm;
        return t;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////
    void normalization(fann_type d[], int len)
    {
         double d1 = 0.;
         int i = 0;
         while(i < len) {
             d1 += d[i]*d[i];
             i++;
         }
         d1 = sqrt(d1)+1;
         i = 0;
         while(i < len) {
             d[i] /= d1;
             i++;
         }
         return;
    }

    void normalization1(fann_type d[], int len)
    {
         fann_type dmax = d[0], dmin = d[0];
         int i = 0;
         while(i < len) {
             if(dmin > d[i]) {
                 dmin = d[i];
             }
             if(dmax < d[i]) {
                 dmax = d[i];
             }
             i++;
         }
         i = 0;
         while(i < len) {
             d[i] =  (d[i] - dmin)/(dmax - dmin);
             i++;
         }
         return;
    }


    static bool is_specials(int sym)
    {
         bool st = false;
         if(sym > 32 && sym < '0') {
             st = true;
         }
         else if(sym > '9' && sym < 'A'){
             st = true;
         }
         else if(sym > 'Z' && sym < 'a') {
             st = true;
         }
         else if(sym > 'z' && sym < '127') {
             st = true;
         }
         return st;
    }

    static bool is_control(int sym)
    {
         bool st = false;
         if((sym >= 0 && sym < 32) || sym == 127) {
             st = true;
         }
    }

    //Calc entropy str
    static double entropy (const char *p, int len)
    {
        int alf[256];
        memset(alf, 0, sizeof(alf));
        for (const char *c = p; *c != 0; c++) {
            if((unsigned char)*c < 128) {
                alf[*c] +=1;
            }
        }
        double res = 0;
        if(len > 0) {
            for (int i=0; i< 256; i++) {
                if (alf[i]!=0) res += ((double)alf[i] / len ) * log2((double)alf[i] / len);
            }
        }
        if(res < 0) return(-res);
        return (res);
    }

    std::string makepath(const char* pnfile, const char* pdir)
    {
        std::string spath;
        std::string sdir;
        if(pdir) {
            sdir = pdir;
            if(sdir.back() == '/') sdir.pop_back();
        }
        if(pnfile) {
            std::string sfile(pnfile);
            if(sfile.front() == '/') sfile.erase(0);
            spath = sdir;
            if(spath.length() > 0) {
                spath  += "/" + sfile;
            }
        }
        return spath;
    }

    void prepareString2FeatureSet(char* pstr, unsigned plen, fann_type featureset[], int type)
    {
//        memset(featureset, 0, sizeof(fann_type)*_LenFeatureSet_);
        int i = 0;
        while(i < _LenFeatureSet_) {
           featureset[i]= 0.;
           i++;
        }

        i = 0;
         while(i < plen) {
             unsigned char ci1 = *(pstr + i);
             if(type & 0x01) {
                 if( ci1 > 32 && ci1 < 128) {
                     while(i < plen - 1) {
                         unsigned char ci2 = *(pstr + i + 1);
                         if( ci2 > 32 && ci2 < 128) {
                             ci1 ^= ci2;
//                             ci1 &= 0x7f;
                             featureset[ci1] += 1;
                             break;
                         }
                         i++;
                     }//EndWhile ci2
                 }
             }
             else {
                 if( ci1 > 32 && ci1 < 128) {
                     featureset[ci1] += 1;
                 }
             }
          i++;
         }
         normalization1(featureset, _LenFeatureSet_);
/*
         char* p1 = "GET /?mwmmMpx=.%2F..%2F..%2F..%2Fetc%2Fmotd%00html HTTP/1.1";
         if(strncmp(pstr, p1, strlen(p1)) == 0) {
printf("---------------------------------------------------------------------------------------->\n");
             i = 0;
             while(i < _LenFeatureSet_) {
                printf("%g ", featureset[i]);
                i++;
             }
             printf("\n");
         }
*/
/*
         i = 0;
         while(i < _LenFeatureSet_) {
            featureset[i] /= _LenFeatureSet_;
            i++;
         }
*/
    }

}
