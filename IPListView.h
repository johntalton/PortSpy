#ifndef _IP_LIST_VIEW_H
#define _IP_LIST_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class IPListView : public BListView {
   public:
      IPListView(BRect);
      ~IPListView();
      virtual void MouseDown(BPoint);
      virtual void MessageReceived(BMessage*);
   private:
     
};
#endif