#ifndef _FINGER_VIEW_H
#define _FINGER_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class FingerView : public BView {
   public:
      FingerView(BRect);
      ~FingerView();
      int32 Finger();
      static int32 Finger_Hook(void* obj){
			return ((FingerView*)obj)->Finger();
		}
		virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
   private:
      BTextControl *Name;
      BButton *FingerNow;
      BTextView *FingerOut;     
      thread_id FingerThread;
      bool goodID;
};
#endif