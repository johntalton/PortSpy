#ifndef _WHOIS_VIEW_H
#define _WHOIS_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>

class WhoisView : public BView {
   public:
      WhoisView(BRect);
      ~WhoisView();
      virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
      static int32 Who_Hook(void* obj){
			return ((WhoisView*)obj)->Who();//
		}
		int32 Who();
   private:
     thread_id WhoisThread;
     BTextView *WhoOut;
     BRadioButton *full;
     BRadioButton *min;
     BTextControl *Name;     
     BStatusBar *status;
     BButton *WhoButton;
     BString WhoServer;
     BPopUpMenu *whoisServerList;
};
#endif