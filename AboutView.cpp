/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>
#include <TranslationUtils.h>

#include "Globals.h"
#include "AboutView.h"

/*******************************************************
*  
*******************************************************/
AboutView::AboutView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_WILL_DRAW){
   SetViewColor(B_TRANSPARENT_32_BIT); // go tran so we have control over drawing
   img = BTranslationUtils::GetBitmap("portspy15.jpg");
}
 

/*******************************************************
*
*******************************************************/
AboutView::~AboutView(){
}

/*******************************************************
*
*******************************************************/
void AboutView::Draw(BRect){
   if(img == NULL){
      FillRect(Bounds());
   }else{
      SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
      FillRect(Bounds());
      BRect cent = img->Bounds();
      BRect b = Bounds();
      float x = (b.Width() - cent.Width()) / 2;
      float y = (b.Height() - cent.Height()) / 2;
      cent.OffsetBy(x,y);
      
      
      DrawBitmap(img,img->Bounds(),cent);
      
   }
   
   
   
}

/*******************************************************
*
*******************************************************/
void AboutView::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BView::MessageReceived(msg);
   }
}



