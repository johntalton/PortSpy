/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <String.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "PortRunner.h"


/*******************************************************
*
*******************************************************/
PortRunner::PortRunner(const char *str){
   Set(str);
}

/*******************************************************
*
*******************************************************/
PortRunner::~PortRunner(){
   // delte everthing
   RemoveList();
}
   
/*******************************************************
*
*******************************************************/
void PortRunner::Set(const char *str){
   BString s(str);
   RemoveList();
   char name[20];
   char *ptr = s.LockBuffer(0);
   while(sscanf(ptr, "%[-0-9]", name) == 1) {
      //printf("\"%s\"\n",name);
      int start,end;
      if(sscanf(name,"%i-%i",&start,&end) == 2){
         //printf("Adding range %i -> %i\n",start,end);
         AddPortRange(start,end);
      }else{
         //printf("Adding %i\n",atoi(name));
         AddPort(atoi(name));
      }
      
      ptr += strlen(name)+1;
   }
   
   cur = head;
   
   s.UnlockBuffer();
}

/*******************************************************
*
*******************************************************/
void PortRunner::AddPort(int p){
   PortItem *i = new PortItem;
   i->next = NULL;
   i->multi = false;
   i->sport = p;
   i->eport = p;
   i->curp = -1;
   
   if(head){
      tail->next = i;
      tail = tail->next;
   }else{
      head = i;
      tail = head;
   }
   if(cur == NULL){
      cur = head;
   }
   count++;
   
}

/*******************************************************
*
*******************************************************/
void PortRunner::AddPortRange(int s, int e){
   if(e < s){ int t = s; s = e; e = t; }
   PortItem *i = new PortItem;
   i->next = NULL;
   i->multi = true;
   i->sport = s;
   i->eport = e;
   i->curp = -1;
   
   if(head){
      tail->next = i;
      tail = tail->next;
   }else{
      head = i;
      tail = head;
   }
   if(cur == NULL){
      cur = head;
   }
   
   count += e - s + 1;
}

/*******************************************************
*
*******************************************************/
void PortRunner::PrintPorts(){
   PortItem *p = head; // dont desturb cur pointer :P
   while(p){
      if(p->multi){
         printf("%i -> %i, ",p->sport,p->eport);
      }else{
         printf("%i, ",p->sport);
      }
      p = p->next;
   }
   printf("\n");
   
   int a = 0;
   while((a = GetNextPort()) != 0){
      printf("%i,",a);
   }
   printf(" (%i)\n",count);
}

/*******************************************************
*
*******************************************************/
int PortRunner::GetNextPort(){
   if(cur == NULL){
      return 0;
   }
   
   if(cur->multi){
      // check to see if we have gone though
      // all the ones in this range
      if(cur->curp == -1){
         // havent started yet
         cur->curp = cur->sport;
         return cur->curp++;
      }else if(cur->curp >= cur->eport){
         // already finished ..
         cur->curp = -1;
         int port = cur->eport;
         cur = cur->next;
         return port;
      }else{
         // somewher in the middle
         return cur->curp++;
      }
   }else{
      int port = cur->sport;
      cur = cur->next;
      return port;
   }
   
   return 0; // never get here :P
}

/*******************************************************
*
*******************************************************/
int PortRunner::CountPorts(){
   return 1;
}

/*******************************************************
*
*******************************************************/
void PortRunner::RemoveList(){
   PortItem *p = head;
   cur = p;
   while(p){
      p = p->next;
      delete cur;
      cur = p;
   }
   head = tail = cur = NULL;
   count = 0;
}












