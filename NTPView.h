#ifndef _NTP_VIEW_H
#define _NTP_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class NTPView : public BView {
   public:
      NTPView(BRect);
      ~NTPView();
      virtual void AttachedToWindow();
      static int32 Time_Hook(void* obj){
			return ((NTPView*)obj)->GetTime();
		}
		virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
   private:
      int32 GetTime();
      
      
      BTextControl *Name;
      BButton *PingIt;
      BTextView *PingOut;     
 
      thread_id PingThread;
      

};
#endif