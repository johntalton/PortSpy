#ifndef _PROXY_WINDOW_H
#define _PROXY_WINDOW_H

#include <String.h>

struct proxy_settings{
   BString Host;
   int Port;
   bool doProxy;
};

#include <String.h>

class ProxyWindow : public BWindow {
   public:
      ProxyWindow();
      proxy_settings GetSettings();
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:
      BTextControl *ptc;
      BTextControl *htc;
      BCheckBox *dop;
};
#endif
