#ifndef _ABSTRACTNEWS_H
#define _ABSTRACTNEWS_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "PSWindow.h"
#include "ScannerView.h"

extern const char *APP_SIGNATURE;
class PortSpy : public BApplication {
   public:
      PortSpy();
      ~PortSpy();
      virtual void AboutRequested();
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
      
   private:
      PSWindow *theWin;
      
};
#endif