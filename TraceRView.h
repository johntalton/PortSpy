#ifndef _TRACER_VIEW_H
#define _TRACER_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class TraceRView : public BView {
   public:
      TraceRView(BRect);
      ~TraceRView();
      virtual void KeyDown(const char *, int32);
      virtual void MessageReceived(BMessage*);
   private:
     

      

};
#endif