/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <StringView.h>

#include "Globals.h"
#include "PSView.h"
#include "ScannerView.h"
#include "UDPScannerView.h"
#include "DNSView.h"
//#include "TraceRView.h"
#include "WhoisView.h"
#include "FingerView.h"
#include "WebSpeedView.h"
//#include "YTabView.h"
#include "PSTabV.h"
#include "AboutView.h"

/*******************************************************
*   Open up a prity little window and add some stuff 
*   to it. Like some tabs.  Thas all we nead I gess
*******************************************************/
PSView::PSView(BWindow *parentWin, BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   parentWindow = parentWin;
   //Shift code to attached to window so tabs work!
   BRect b = Bounds();
   
  // b.bottom = b.bottom - 14;
   //tabView = new BTabView(b, "tab_view");
   PSTabV *ytv = new PSTabV(b);
   //tabView->SetViewColor(216,216,216,0);
   AddChild(ytv);
   
//   TView = new TraceRView(ytv->Bounds());   
//   UView = new UDPScannerView(ytv->Bounds());
   
   
   AView = new AboutView(ytv->Bounds());//,"",B_FOLLOW_ALL,0);
   ytv->AddTab(AView,"About");
   
   SView = new ScannerView(ytv->Bounds());
   ytv->AddTab(SView,"Scanner");
   
   FView = new FingerView(ytv->Bounds());
   ytv->AddTab(FView,"Finger");
   
   WView = new WhoisView(ytv->Bounds());
   ytv->AddTab(WView,"Whois");
   
   PView = new PingView(ytv->Bounds());
   ytv->AddTab(PView,"Ping");
   
   DView = new DNSView(ytv->Bounds());
   ytv->AddTab(DView,"DNS");
   
   NView = new NTPView(ytv->Bounds());
   ytv->AddTab(NView,"Time");
   
   MView = new WebSpeedView(ytv->Bounds());//,"",B_FOLLOW_ALL,0);
   ytv->AddTab(MView,WEBSPEED);
   
   ytv->SelectTab(0);
   
  

   // Add in the standerd looking status bar at the bottom
   b = Bounds();
   StatusBar = new BBox(BRect(b.left-3,b.bottom-14,b.right+3,b.bottom+3),"StatusBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM); 
  // AddChild(StatusBar);
}

/*******************************************************
*
*******************************************************/
PSView::~PSView(){
}

/*******************************************************
*   We need to do some stuff after we are actually 
*   attached to the window. I dont realy know why 
*   you have to be attached befor you can set the tab
*   width but thats the way they did it. Oh well.
*******************************************************/
void PSView::AttachedToWindow(){
   //tabView->SetTabWidth(B_WIDTH_FROM_WIDEST);
}

/*******************************************************
*   If its our message handle it .. otherwise pass it
*   To the proper view, and he can take care of it.
*******************************************************/
void PSView::MessageReceived(BMessage *msg){
   switch(msg->what){
   case PROXY_SETTINGS:
   case SWITCH_VIEW:
      MView->MessageReceived(msg);
      break;
   case UDPSTOP:
   case UDPSCAN: // Pass down to UDPscanner
  //    UView->MessageReceived(msg);
      break;
   case STOP:
   case SCAN: // Pass down to scanner
      SView->MessageReceived(msg);
      break;
   case NEWWHOSERVER:
   case WHO:  // Pass down to whois
      WView->MessageReceived(msg);
      break;
   case LOOKUP:
   case BY_NAME:
   case BY_IP:  // pass down to dns
      DView->MessageReceived(msg);
      break;
   case STOPPING:
   case PING: // pass down to ping
      PView->MessageReceived(msg);
      break;
   case FINGER:
      FView->MessageReceived(msg);
      break;
   case IPHISTORY: // send to current selected
//      PView->MessageReceived(msg);
//      DView->MessageReceived(msg);
 //     SView->MessageReceived(msg);
//      UView->MessageReceived(msg);
//      (tabView->TabAt(tabView->FocusTab())->View())->MessageReceived(msg);
      break;
   default:  // handle this myself
      BView::MessageReceived(msg);
   }
}
