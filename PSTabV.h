#ifndef _YNOPTabV_VIEW_H
#define _YNOPTabV_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "YTabView.h"

class PSTabV : public YTabView {
   public:
      PSTabV(BRect);
      PSTabV(BMessage *msg):YTabView(msg){};
      ~PSTabV();
      //virtual void DrawTab(BRect rect,const tab_item &item,int32 current);
      virtual status_t SelectTab(const uint32 loc);
};
#endif