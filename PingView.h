#ifndef _PING_VIEW_H
#define _PING_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class IPHistoryView;

class PingView : public BView {
public:
   PingView(BRect);
   virtual ~PingView();
   int32 PingHost();
   static int32 Ping_Hook(void* obj){
      return ((PingView*)obj)->PingHost();
   }
   virtual void DetachedFromWindow();
   virtual void AttachedToWindow();
   virtual void MessageReceived(BMessage*);
private:
   void Limit(BTextView*);
   int StringToInt(const char*);
   BString MakeIP(int,int,int,int);
   
   BTextControl *Name;
   BButton *PingIt;
   BTextView *PingOut;     
   thread_id PingThread;
   IPHistoryView *iphv;
};
#endif