#ifndef IP_ITEM_H
#define IP_ITEM_H

#include <ListItem.h>
#include <String.h>

class IPItem:public BListItem{
   public:
      IPItem(const char *s,BString);
      ~IPItem();
      void AddPort(int);
      virtual void DrawItem(BView *owner, BRect frame, bool complete = false);
      void MakeMenu(BMenu*);
      BString GetHostName();
      void SetHostName(const char*);
      void SetHostDown();
   private:
      char *name;
      BString Ports;
      BList portList;
      BString HostName;
      BString HostNameString;
      bool HostDown;
};
#endif