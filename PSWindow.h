#ifndef _ANEWS_WINDOW_H
#define _ANEWS_WINDOW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "PSView.h"


class PSWindow : public BWindow {
   public:
      PSWindow(BRect);
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:
      BMenuBar *menubar;
      PSView *View;
      BMenu *WSmenu;

};
#endif