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
#include "TraceRView.h"
#include "WhoisView.h"
#include "FingerView.h"

/*******************************************************
*   Open up a prity little window and add some stuff 
*   to it. Like some tabs.  Thas all we nead I gess
*******************************************************/
PSView::PSView(BWindow *parentWin, BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   parentWindow = parentWin;
   //Shift code to attached to window so tabs work!
   BRect b = Bounds();
   
   b.bottom = b.bottom - 15;
   tabView = new BTabView(b, "tab_view");
   tabView->SetViewColor(216,216,216,0);
   
   SView = new ScannerView(tabView->Bounds());
   DView = new DNSView(tabView->Bounds());
   TView = new TraceRView(tabView->Bounds());   
   PView = new PingView(tabView->Bounds());
   WView = new WhoisView(tabView->Bounds());
   UView = new UDPScannerView(tabView->Bounds());
   FView = new FingerView(tabView->Bounds());
   
   tab = new BTab();
   tabView->AddTab(SView,tab);  
   tab->SetLabel("Scanner");
   //tab = new BTab();
   //tabView->AddTab(UView,tab);  
   //tab->SetLabel("UDPScanner");
   tab = new BTab();
   tabView->AddTab(FView,tab);  
   tab->SetLabel("Finger");
   tab = new BTab();
   tabView->AddTab(WView, tab);
   tab->SetLabel("Whois");
   //tab = new BTab();
   //tabView->AddTab(TView, tab);
   //tab->SetLabel("TraceRoute");
   tab = new BTab();
   tabView->AddTab(PView, tab);
   tab->SetLabel("Ping");
   tab = new BTab();
   tabView->AddTab(DView, tab);
   tab->SetLabel("DNS");
   
   AddChild(tabView);

   // Add in the standerd looking status bar at the bottom
   b = Bounds();
   StatusBar = new BBox(BRect(b.left-3,b.bottom-14,b.right+3,b.bottom+3),"StatusBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM); 
   AddChild(StatusBar);
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
   tabView->SetTabWidth(B_WIDTH_FROM_WIDEST);
}

/*******************************************************
*   If its our message handle it .. otherwise pass it
*   To the proper view, and he can take care of it.
*******************************************************/
void PSView::MessageReceived(BMessage *msg){
   switch(msg->what){
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
