#ifndef _SCANNER_VIEW_H
#define _SCANNER_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <NetworkKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include "IPListView.h"

class IPHistoryView;

class ScannerView : public BView {
   public:
      ScannerView(BRect);
      ~ScannerView();
      virtual void AttachedToWindow();
      virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
      int32 ScanNow();
      static int32 ScanNow_Hook(void* obj){
			return ((ScannerView*)obj)->ScanNow();
		}
   private:
      int StringToInt(const char*);
      BString MakeIP(int,int,int,int);
      void Limit(BTextView*);
      
      IPHistoryView *iphv;
      
      thread_id ScannerThread;
      
      BButton *ScanButton;
      
      BPopUpMenu *IPList;
      
      IPListView *ScannedList;
      BTextControl *IP_One;
      BTextControl *IP_Two;
      BTextControl *IP_Three;
      BTextControl *IP_Four_Start;
      BTextControl *IP_Four_Finish;
      
      
      BTextControl *portsv;
      
    /*  BCheckBox *ftp;
      BCheckBox *http;
      BCheckBox *telnet;
      BCheckBox *samba;
      BCheckBox *dns;
      BCheckBox *echo;
      BCheckBox *nntp;
      BCheckBox *smtp;
      BCheckBox *pop3;

      BCheckBox *scanrange;
      
      BTextControl *From;
      BTextControl *To;
*/      
      BStatusBar *status;
};
#endif