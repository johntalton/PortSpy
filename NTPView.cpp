/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <NetworkKit.h>
#include <String.h>

#include <stdio.h>
#include <stdlib.h>
#include <socket.h>
#include <netdb.h>
#include <string.h>

#include "Globals.h"
#include "NTPView.h"

struct ntp_packet {
   char li_vn_mode;	//leap, version, mode
   char stratum;	// peer stratum
   char ppoll;		//peer polling interval
   signed char precision; //peer clock precison
   long rootdelay; //distance to primary clock (64 bits)
   long rootdispersion; //clock dispersion
   unsigned int refID; //refernec clock ID
   long reftime1;
   long reftime2; //last clock update
   long org1;
   long org2; //originate time stamp
   long rec1;
   long rec2; //received time stamp;
   unsigned long xm1;
   uint32 xm2; //tramsit time stamp

};


/*******************************************************
*
*******************************************************/
NTPView::NTPView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();

   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
   
   BStringView *s1 = new BStringView(BRect(80,10,b.right-10,25),"","Enter the host to use to get the current time from");
   Bb->AddChild(s1);
   
   
   BRect t = b;
   t.top = 55;
   t.left = 160;
   t.right = 285;
   Name = new BTextControl(t,"name","","",new BMessage(TIME));
   Name->SetDivider(0);
   Bb->AddChild(Name);
   
   PingIt = new BButton(BRect(290,53,10,10), "time","Time", new BMessage(TIME));
   PingIt->ResizeToPreferred();
   Bb->AddChild(PingIt);
 
   b = Bounds();
   b.InsetBy(5,5);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;

   PingOut = new BTextView(b,"",BRect(0,0,b.right-20,200),B_WILL_DRAW,B_FOLLOW_ALL_SIDES);
   PingOut->MakeEditable(false);
   PingOut->MakeSelectable(true);
   PingOut->MakeResizable(false);
   PingOut->SetWordWrap(true);
   AddChild(new BScrollView("",PingOut,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
   PingOut->SetViewColor(230,230,230,255);
}

/*******************************************************
*
*******************************************************/
NTPView::~NTPView(){
}

/*******************************************************
*
*******************************************************/
void NTPView::AttachedToWindow(){
   PingIt->SetTarget(this);
   Name->SetTarget(this);
}

/*******************************************************
*
*******************************************************/
int32 NTPView::GetTime(){
   
   Window()->Lock(); 
   PingOut->SelectAll();
   PingOut->Clear();
   Window()->Unlock();
   
   Window()->Lock(); 
   PingOut->Insert("Its late!\n");
   Window()->Unlock(); 
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   Window()->Lock();
   PingIt->SetMessage(new BMessage(PING));
   PingIt->SetLabel("Ping");
   Window()->Unlock();
   
   return 0;
}

/*******************************************************
*   Man this took a long time to fix. Simple little 
*   DetachedFromWindow method. This is what keeps the 
*   app from crashing (hard) when you are running a
*   thead and you switch tabs.  You see the BTabView
*   detaches this view form the window then you select
*   a diffrent tab - thus causeing ALL output or input
*   from this view to die. It should probably not be 
*   this way, a BTabView should Hide() and Show() the
*   views it contains so that activity could continue 
*   while the user does something else.  
*******************************************************/
void NTPView::DetachedFromWindow(){
   kill_thread(PingThread);
   PingIt->SetMessage(new BMessage(TIME));
   PingIt->SetLabel("Ping"); 
}


/*******************************************************
*
*******************************************************/
void NTPView::MessageReceived(BMessage *msg){
   //printf("MessageReceived in ping\n");
   switch(msg->what){
   case TIME:
      kill_thread(PingThread);
      //PingIt->SetMessage(new BMessage(STOPTIME));
      PingIt->SetLabel("Cancel");
      PingThread = spawn_thread(Time_Hook, "tick tock tick", B_NORMAL_PRIORITY, this);
	   resume_thread(PingThread);
      break;
   case STOPPING:
      kill_thread(PingThread);
      PingIt->SetMessage(new BMessage(TIME));
      PingIt->SetLabel("Time"); 
      break;
   default:
      //printf("default ping\n");
      //msg->PrintToStream();
      BView::MessageReceived(msg);
   }
}



