/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>

#include "Globals.h"
#include "TraceRView.h"

/*******************************************************
*
*******************************************************/
TraceRView::TraceRView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();
   
   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
   
   
   BRect t = Bb->Bounds();
   BStringView *s = new BStringView(BRect(25,t.bottom/2-20,t.right-10,t.bottom/2),"","At this time it seems to me that TraceRoute (under BeOS sockets) is impossible to accomplish");
   Bb->AddChild(s);
   BStringView *s2 = new BStringView(BRect(100,t.bottom/2-20,t.right-10,t.bottom/2+20),"","due to the inablility to write raw packets. What do you think?");
   Bb->AddChild(s2);
}

/*******************************************************
*
*******************************************************/
TraceRView::~TraceRView(){
}

/*******************************************************
*
*******************************************************/
void TraceRView::KeyDown(const char *bytes, int32 numBytes){
   if(numBytes==1){
      switch(bytes[0]){
      default:
         BView::KeyDown(bytes, numBytes);
         break;
      }
   }
}

/*******************************************************
*
*******************************************************/
void TraceRView::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BView::MessageReceived(msg);
   }
}



