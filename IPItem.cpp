/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <TranslationUtils.h>
#include <String.h>

#include <stdlib.h>

#include "Globals.h"
#include "IPItem.h"

/*******************************************************
*   Set up the item and copy the name localy so we can
*   hang on too it.
*******************************************************/
IPItem::IPItem(const char *s,BString ports):BListItem(){
   name = (char *) malloc(strlen(s)+1);
   strcpy(name, s);
   HostName.SetTo(name);
   //Ports = new BList(ports);
   HostDown = false;
   Ports.SetTo(ports);
   HostNameString.SetTo("");
}

/*******************************************************
*   Free up the name we are hanging onto.
*******************************************************/
IPItem::~IPItem(){
   free(name);

}

/*******************************************************
*   Free up the name we are hanging onto.
*******************************************************/
void IPItem::SetHostName(const char *s){
   name = (char *) malloc(strlen(s)+1);
   strcpy(name, s);
   HostNameString.SetTo(name);
}

/*******************************************************
*   Sets the itme to Hostdown .. ie it does not 
*   respond to pings. 
*******************************************************/
void IPItem::SetHostDown(){
   HostDown = true;
}

/*******************************************************
*   Add  a port to a existing item
*******************************************************/
void IPItem::AddPort(int p){
   Ports.Append(" ");
   Ports << (int32)p;
  /* switch(p){
   case 21:
      Ports.Append("(ftp)");
      break;
   case 23:
      Ports.Append("(telnet)");
      break;
   case 80:
      Ports.Append("(http)");
      break;
   }*/
   portList.AddItem((void*)p);
   //Invalidate(); // this does not work - but should 
}

/*******************************************************
*   So we draw a cute little item here
*******************************************************/
void IPItem::DrawItem(BView *owner, BRect frame, bool complete) {
   if (IsSelected() || complete) {
      rgb_color color;
      if (IsSelected()) {
         color.red = 190;
         color.green = 190;
         color.blue = 190;
      }else{
         color = owner->ViewColor();
      }
      owner->SetHighColor(color);
      owner->FillRect(frame);
   }
   owner->MovePenTo(frame.left, frame.bottom-2);
   if (IsEnabled()) {
      owner->SetLowColor(190,190,190);
      owner->SetHighColor(0,0,0);
   }else{
     owner->SetHighColor(100,0,0);
   }
   if(HostDown){
      owner->SetHighColor(100,0,0);
   }
   owner->DrawString(HostName.String());
   //owner->DrawString(" ");
   //owner->DrawString(HostNameString.String());
   owner->MovePenTo(frame.left+100, frame.bottom-2);
   //BString s;
   owner->DrawString(Ports.String());
   
   owner->MovePenTo(frame.left+100+((Ports.Length()+2)*5), frame.bottom-2); // 5 is the font width
   owner->DrawString(HostNameString.String());
}

/*******************************************************
*   Make a menu based on what ports we have
*******************************************************/
void IPItem::MakeMenu(BMenu* menu){
   BMenuItem* item;
   BMenu *submenu;
   int i;
   bool Added = false;

 //menu->AddItem(new BSeparatorItem());
   for(i = 0;i < portList.CountItems();i++){
      switch((int)portList.ItemAt(i)){ 
      case 21:
         Added = true;
         menu->AddItem(new BMenuItem("(21) FTP",new BMessage(FTP)));
         break;
      case 23:
         Added = true;
         menu->AddItem(new BMenuItem("(23) Terminal",new BMessage(TERM)));
         break;
      case 80:
         Added = true;
         menu->AddItem(new BMenuItem("(80) Net+",new BMessage(NETP)));
         break;
      }
   }
   if(!Added){
      menu->AddItem(new BMenuItem("No Standard Ports found",NULL));
   }
}

/*******************************************************
*   Return the host name so other can play with it
*******************************************************/
BString IPItem::GetHostName(){
   return HostName;
}








