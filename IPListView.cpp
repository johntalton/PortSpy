/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>

#include <stdlib.h>

#include "Globals.h"
#include "IPListView.h"
#include "IPItem.h"

/*******************************************************
*
*******************************************************/
IPListView::IPListView(BRect frame):BListView(frame, "", B_SINGLE_SELECTION_LIST,B_FOLLOW_ALL_SIDES){
   SetViewColor(216,216,216,0);

}

/*******************************************************
*
*******************************************************/
IPListView::~IPListView(){
}

/*******************************************************
*
*******************************************************/
void IPListView::MouseDown(BPoint p){
   int i;
   IPItem *tmpIPItem;
   BMessage *currentMsg = Window()->CurrentMessage();
   char *ad;
   BString tmpS;
	
	if (currentMsg->what == B_MOUSE_DOWN) {
		uint32 buttons = 0;
		currentMsg->FindInt32("buttons", (int32 *)&buttons);
		uint32 modifiers = 0;
		currentMsg->FindInt32("modifiers", (int32 *)&modifiers);

		if(buttons & B_PRIMARY_MOUSE_BUTTON){ //Foce select on single click
	      i = IndexOf(p);
		   if(i >= 0){
		      Select(i);
	      }
	   }
	   if (buttons & B_SECONDARY_MOUSE_BUTTON) {
         i = IndexOf(p);
         if(i >= 0){
            Select(i);
            //Popup a menu with stuff about the IP
            BPopUpMenu *menu = new BPopUpMenu("IPMenu");
            tmpIPItem = (IPItem*)ItemAt(i);
            tmpIPItem->MakeMenu(menu);
            BMenuItem *selected = menu->Go(ConvertToScreen(p));
            if(selected){
               if(selected->Message()){
                  //Window()->PostMessage(selected->Message()->what);
                  switch(selected->Message()->what){
                  case TERM:
                     
                     break;
                  case NETP:
                     tmpS = tmpIPItem->GetHostName();
                     tmpS.Prepend("http://");
                     tmpS.Append("/");
                     ad = tmpS.String(); 
                     
                     be_roster->Launch("application/x-vnd.Be-NPOS",1,&ad);
                     break;
                  case FTP:
                     
                     break;
                  }
               }
            }
         }
	   }
   }
}

/*******************************************************
*
*******************************************************/
void IPListView::MessageReceived(BMessage *msg){
   switch(msg->what){

   default:
      BListView::MessageReceived(msg);
   }
}



