/*******************************************************
*   YTVDemo©
*
*   A simple demo to show how kewl my tabs are ....not
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @version 1.0
*   @date    May 10, 2000
*******************************************************/
#include <InterfaceKit.h>

#include "PSTabV.h"
#include "PSWindow.h"
#include "Globals.h"

/*******************************************************
*
*******************************************************/
PSTabV::PSTabV(BRect frame):YTabView(frame,"tab_view", TAB_NORTH, WIDTH_FROM_WIDEST, B_FOLLOW_ALL, B_WILL_DRAW|B_FRAME_EVENTS){

}

/*******************************************************
*
*******************************************************/
PSTabV::~PSTabV(){
}

/*******************************************************
*
*******************************************************/
status_t PSTabV::SelectTab(const uint32 loc){
   //BString s;
   //s.SetTo(GetLabelForTab(loc));
   
   if(SelectedTab() == loc){
      return B_OK;
   }
   
   PSWindow *psw = (PSWindow*)Window();
   BView *view = ItemAt();
   psw->PostMessage(new BMessage(VIEW_NOT_ACTIVE),view);
   view = ItemAt(loc);
   psw->PostMessage(new BMessage(VIEW_ACTIVE),view);
   
  /* if(s.Compare(WEBSPEED) == 0){
      psw->PostMessage(new BMessage(SHOW_WS_MENU));
   }else{
      psw->PostMessage(new BMessage(HIDE_WS_MENU));
   }*/
   
   return YTabView::SelectTab(loc);
}