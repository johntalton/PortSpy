#ifndef IP_HISTORY_H
#define IP_HISTORY_H

#include <String.h>

#include "YList.h"

struct ip_hist{
   uint8 a;
   uint8 b;
   uint8 c;
   uint8 d;
   BString name;
};

class IPHistory{
public:
   IPHistory();
   virtual ~IPHistory();
   status_t AddIP(uint8 a, uint8 b, uint8 c, uint8 d, BString name);
public:
   YList ips;
};
extern IPHistory iphistory;
#endif