#ifndef _UDP_SCANNER_VIEW_H
#define _UDP_SCANNER_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <NetworkKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include "IPListView.h"

class UDPScannerView : public BView {
   public:
      UDPScannerView(BRect);
      ~UDPScannerView();
      virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
      int32 ScanNow();
      static int32 UDPScanNow_Hook(void* obj){
			return ((UDPScannerView*)obj)->ScanNow();
		}
   private:
      int StringToInt(const char*);
      BString MakeIP(int,int,int,int);
      void Limit(BTextView*);
      
      thread_id ScannerThread;
      
      BButton *ScanButton;
      
      BPopUpMenu *IPList;
      
      IPListView *ScannedList;
      
      BTextControl *IP_One_Start;
      BTextControl *IP_Two_Start;
      BTextControl *IP_Three_Start;
      BTextControl *IP_Four_Start;
            
      BTextControl *IP_One_Finish;
      BTextControl *IP_Two_Finish;
      BTextControl *IP_Three_Finish;
      BTextControl *IP_Four_Finish;
            
      BCheckBox *scanrange;
      
      BTextControl *From;
      BTextControl *To;
      
      BStatusBar *status;
};
#endif