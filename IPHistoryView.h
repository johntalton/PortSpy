#ifndef _HISTORY_VIEW_H
#define _HISTORY_VIEW_H

#include <InterfaceKit.h>

#define IP_HISTORY 'ipht'

#include "IPHistory.h"

class IPHistoryView : public BView, public BInvoker {
public:
   IPHistoryView(BRect);
   virtual ~IPHistoryView();
   virtual void AttachedToWindow();
   virtual void MouseDown(BPoint point);
   virtual void Draw(BRect);
   virtual void MessageReceived(BMessage*);
   
   status_t AddIP(uint8 a, uint8 b, uint8 c, uint8 d, BString name);
private:
   void BuildMenu(BMenu *menu);
private:
   BHandler *target;
};
#endif