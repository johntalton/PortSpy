/*******************************************************
*   PortSpy©
*
*   @author  YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/

#include "IPHistoryView.h"
#include "IPHistory.h"

/*******************************************************
*   
*******************************************************/
IPHistoryView::IPHistoryView(BRect frame):BView(frame,"historypopup",B_FOLLOW_NONE,B_WILL_DRAW),BInvoker(){
 SetViewColor(B_TRANSPARENT_32_BIT);
}

/*******************************************************
*   
*******************************************************/
IPHistoryView::~IPHistoryView(){

}

/*******************************************************
*   
*******************************************************/
status_t IPHistoryView::AddIP(uint8 a, uint8 b, uint8 c, uint8 d, BString name){
   return iphistory.AddIP(a,b,c,d,name);
}

/*******************************************************
*   
*******************************************************/
void IPHistoryView::AttachedToWindow(){
}

/*******************************************************
*   
*******************************************************/
void IPHistoryView::Draw(BRect){
   BRect r = Bounds();
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_NO_TINT));
   FillRect(r);
   
   r.top += 5;
   r.bottom -= 5;
   r.left += 5;
   r.right -= 5;
   SetHighColor(100,100,255);
   FillTriangle(BPoint(r.left,r.top),BPoint(r.right,r.top),BPoint((r.left+r.right)/2,r.bottom));
   SetHighColor(0,0,255);
   StrokeTriangle(BPoint(r.left,r.top),BPoint(r.right,r.top),BPoint((r.left+r.right)/2,r.bottom));
}

/*******************************************************
*   @description 
*******************************************************/
void IPHistoryView::BuildMenu(BMenu *menu){
   BMenuItem *item = NULL;
   
   
   BMessage *msg = NULL;
   ip_hist *ih = NULL;
   for(int32 i = 0;i < iphistory.ips.CountItems();i++){
      ih = (ip_hist*)iphistory.ips.ItemAt(i);
      if(ih == NULL){ continue; }
      msg = new BMessage(IP_HISTORY);
      msg->AddInt8("a",ih->a);
      msg->AddInt8("b",ih->b);
      msg->AddInt8("c",ih->c);
      msg->AddInt8("d",ih->d);
      msg->AddString("name",ih->name.String());
      //msg->PrintToStream();
      item = new BMenuItem(ih->name.String(), msg );
      item->SetTarget(Target());
      menu->AddItem(item);
   }
   //menu->AddSeparatorItem();
}



/*******************************************************
*   @description 
*******************************************************/
void IPHistoryView::MouseDown(BPoint point){
   uint32 buttons = 0;
   BMessage *currentMsg = Window()->CurrentMessage();
   currentMsg->FindInt32("buttons", (int32 *)&buttons);
   
   //if(buttons & B_PRIMARY_MOUSE_BUTTON){
   //}else if (buttons & B_SECONDARY_MOUSE_BUTTON){
      BPopUpMenu* menu = new BPopUpMenu("History Popup");
      BuildMenu(menu);
      ConvertToScreen(&point);
      menu->Go(point, true);
      return;
   //}
   
   
   BView::MouseDown(point);
   
}

/*******************************************************
*   
*******************************************************/
void IPHistoryView::MessageReceived(BMessage *msg){

}
