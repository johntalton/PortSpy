/*******************************************************
*   PortSpy©
*
*   @author  YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include "IPHistory.h"

IPHistory iphistory;

/*******************************************************
*   
*******************************************************/
IPHistory::IPHistory(){

   AddIP(127,0,0,1,"Localhost");
  /* AddIP(192,168,1,1,"Brain");
   AddIP(192,168,1,37,"Abstract");
   AddIP(192,168,0,1,"Plenty");*/
}

/*******************************************************
*   
*******************************************************/
IPHistory::~IPHistory(){

}

/*******************************************************
*   
*******************************************************/
status_t IPHistory::AddIP(uint8 a, uint8 b, uint8 c, uint8 d, BString name){
   
   // test to see if this ip is already in the list !!!
   ip_hist *item = NULL;
   for(int32 i = 0; i < ips.CountItems();i++){
      item  = (ip_hist*)ips.ItemAt(i);
      if(item){
         if((item->a == a) &&
            (item->b == b) &&
            (item->c == c) &&
            (item->d == d)){
            // same ip :P
            //item->name = name;
            return B_ERROR;
         }
      }
   }
   
   
   ip_hist *iph = new ip_hist;
   iph->a = a;
   iph->b = b;
   iph->c = c;
   iph->d = d;
   iph->name = name;
   
   ips.AddItem(iph);
   
   return B_OK;
}



