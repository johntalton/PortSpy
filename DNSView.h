#ifndef _DNS_VIEW_H
#define _DNS_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include "IPTextControl.h"

class DNSView : public BView {
   public:
      DNSView(BRect);
      ~DNSView();
      int32 DNSLookup();
      static int32 DNS_Hook(void* obj){
			return ((DNSView*)obj)->DNSLookup();
		}
		virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
   private:
      void Limit(BTextView*);
      int StringToInt(const char*);
      BString MakeIP(int,int,int,int);
   
      BBox *Bb;
      BRadioButton *ByIP;
      BRadioButton *ByName;
      BTextView *DNSOut;
      BStatusBar *status;
      BButton *Lookup;
      BTextControl *Name;
      BTextControl *IP_One;
      BTextControl *IP_Two;
      BTextControl *IP_Three;
      BTextControl *IP_Four;
      
      thread_id DNSThread;

};
#endif