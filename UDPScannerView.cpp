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

#include "Globals.h"
#include "UDPScannerView.h"
#include "IPItem.h"
#include "IPListView.h"

/*******************************************************
*   The GUI for the Scanner Window. This view also has
*   all the code that the scaner uses. it is the 
*   threader for the thingy
*******************************************************/
UDPScannerView::UDPScannerView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();

   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
   
   BRect t = Bb->Bounds();
   
   t.top = 15;
   t.bottom = 40;
   t.left += 20;
   t.right = t.left+30;
   
   IP_One_Start = new BTextControl(t,"","","0",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_One_Start->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Two_Start = new BTextControl(t,"","","0",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Two_Start->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Three_Start = new BTextControl(t,"","","0",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Three_Start->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Four_Start = new BTextControl(t,"","","0",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Four_Start->SetDivider(0);

   t = Bb->Bounds();
   t.top = 35;
   t.bottom = 40;
   t.left += 20;
   t.right = t.left+30;

   IP_One_Finish = new BTextControl(t,"","","255",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_One_Finish->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Two_Finish = new BTextControl(t,"","","255",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Two_Finish->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Three_Finish = new BTextControl(t,"","","255",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Three_Finish->SetDivider(0);
   t.left += 32;
   t.right = t.left+30;
   IP_Four_Finish = new BTextControl(t,"","","255",NULL,B_FOLLOW_LEFT_RIGHT);
   IP_Four_Finish->SetDivider(0);
   
   Bb->AddChild(IP_One_Start);
   Bb->AddChild(IP_Two_Start);
   Bb->AddChild(IP_Three_Start);
   Bb->AddChild(IP_Four_Start);
   
   Bb->AddChild(IP_One_Finish);
   Bb->AddChild(IP_Two_Finish);
   Bb->AddChild(IP_Three_Finish);
   Bb->AddChild(IP_Four_Finish);
   
   IPList = new BPopUpMenu("");
   IPList->AddItem(new BMenuItem("127.0.0.1",new BMessage(IPHISTORY)));
   
   BMenuField *IPMenu = new BMenuField(BRect(150,25,175,30),"","",IPList);
   IPMenu->SetDivider(0);
   Bb->AddChild(IPMenu);
   
   ScanButton = new BButton(BRect(180,25,10,10), "scan","Scan", new BMessage(UDPSCAN));
   ScanButton->ResizeToPreferred();
   Bb->AddChild(ScanButton);
   
   t = Bb->Bounds();
   t.left = t.right - 210;
   
   t.top = 70;
   t.left = t.right - 210;
   scanrange = new BCheckBox(t, "", "Scan range",NULL);
   Bb->AddChild(scanrange);
   scanrange->ResizeToPreferred();
   
   
   t.top += 25;
   t.right = t.right - 120;
   From = new BTextControl(t,"","From","1",NULL,B_FOLLOW_LEFT_RIGHT);
   From->SetDivider(25);
   Bb->AddChild(From);
   
   t.left = t.right + 10;
   t.right = Bb->Bounds().right - 20;
   To = new BTextControl(t,"","To","128",NULL,B_FOLLOW_LEFT_RIGHT);
   To->SetDivider(20);
   Bb->AddChild(To);
   
   BBox *Stat = new BBox(BRect(10,65,275,120),"Box",B_FOLLOW_TOP_BOTTOM);
   Stat->SetLabel("Progress");
   AddChild(Stat);
   
   status = new BStatusBar(BRect(10,15,Stat->Bounds().right-10,100),"","0.0.0.0","0");
   Stat->AddChild(status);
   
   t = b;
   t.InsetBy(5,5);
   t.top = 135;
   t.right = t.right - B_V_SCROLL_BAR_WIDTH;
   t.bottom = t.bottom - B_H_SCROLL_BAR_HEIGHT -20 ;
   
   ScannedList = new IPListView(t);
   ScannedList->SetViewColor(230,230,230,255);
   AddChild(new BScrollView("",ScannedList,B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW,true,true));
  
 
   BTextView *tmpV;
   tmpV = (BTextView*)IP_One_Start->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Two_Start->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Three_Start->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Four_Start->ChildAt(0);
   Limit(tmpV);
   
   tmpV = (BTextView*)IP_One_Finish->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Two_Finish->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Three_Finish->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Four_Finish->ChildAt(0);
   Limit(tmpV);
   

}

/*******************************************************
*
*******************************************************/
UDPScannerView::~UDPScannerView(){
}

/*******************************************************
*   Little func to fix up our text boxes so they behave
*   a little more acourding to IP rules
*******************************************************/
void UDPScannerView::Limit(BTextView *asdf){
   int i;
   for(i = 30;i < 48;i++){
      asdf->DisallowChar(i);
   }
   for(i = 58;i < 130;i++){
     asdf->DisallowChar(i);
   }
   asdf->SetMaxBytes(3);
}

/*******************************************************
*   Quick and dirty func to change a string of chars 
*   into a int. Method by Chris Camp (cmc008@coes.latech.edu)
*******************************************************/
int UDPScannerView::StringToInt(const char *temp){
   int i = 0;
   int t = 0;
   if (strcmp(temp,"") == -1){ return 0; }
   char c = 'X';
   while(temp[i] != '\0'){
      c = temp[i];
      t = t * 10 + (c - 48); // 48 is the ASCII code for 0  
      i++;
   }
   return t;
}

/*******************************************************
*
*******************************************************/
/*char* ScannerView::IntToString(int a){
   BString t;
   t.SetTo("");
   t << (int32)a;
   return t.String();
}
*/

/*******************************************************
*   Just to make the later code look good and readable
*   I pulled this out. It makes a IP String from 4 ints
*******************************************************/
BString UDPScannerView::MakeIP(int a,int b,int c,int d){
   BString IP("");
   IP << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
   return IP;
}

/*******************************************************
*   This is the main scan thread. It gest spun of and
*   scans the range of IP and range of Ports. Its got
*   a lot of Window()->Lock()/Unlocks() so that the GUI
*   can update and other messages can be passes.  This 
*   took a long time to get right, and is the cause of
*   most crashes. :)
*******************************************************/
int32 UDPScannerView::ScanNow(){
   int a = 0;
   BNetEndpoint *control;
   BString portList;
   BString tmp;
   BString hostname("");
   
   Window()->Lock();   
   ScannedList->MakeEmpty();
   control = new BNetEndpoint;
   BNetAddress addr;
   IPItem *item = 0;

   if(control->InitCheck() != B_NO_ERROR){
      (new BAlert(NULL,"BNetEndpoint failed\nThis could meen networking is down!","Ok"))->Go();
      Window()->Unlock(); // almost forgot this
      return 0;
   }
   int MaxPort = StringToInt(To->Text());
   int MinPort = StringToInt(From->Text());
   int StartIP = StringToInt(IP_Four_Start->Text());
   int EndIP = StringToInt(IP_Four_Finish->Text());
   if(EndIP < StartIP){  // count in the right direction so we can update status bar easyer
      int tIP = StartIP;
      StartIP = EndIP;
      EndIP = tIP;
   }
   Window()->Unlock();
   
   int CurIP;
   for(CurIP = StartIP; CurIP <= EndIP; CurIP++){
      Window()->Lock();
      hostname.SetTo(MakeIP(StringToInt(IP_One_Start->Text()),
                            StringToInt(IP_Two_Start->Text()),
                            StringToInt(IP_Three_Start->Text()),
                            CurIP));
      Window()->Unlock();
                            
      // We need some code right here to check to see if we have a IP/Host
      // that is down. If so Dont scan it .. it will timeout ... uhh longtime
    
      Window()->Lock();
      status->Reset("","");
      status->SetMaxValue(MaxPort - MinPort);
      Window()->Unlock();
    
      portList.SetTo("");
        
      item = new IPItem(hostname.String(),portList);
        
      Window()->Lock();
      ScannedList->AddItem(item);
      Window()->Unlock();
      
      for(a = MinPort;a <= MaxPort;a++){
         tmp.SetTo("");
         tmp << (int32)a;
         Window()->Lock();
         status->Update(a-status->CurrentValue()-MinPort,hostname.String(),tmp.String());
         Window()->Unlock();
         addr.SetTo(hostname.String(), a);
         if(control->Connect(addr) == B_NO_ERROR){
            portList << (int32)a << "  ";
            item->AddPort(a);
            Window()->Lock();
            ScannedList->InvalidateItem(CurIP - StartIP); // had to do this so that it would redraw.
            Window()->Unlock();
            control->Close(); // addind this in just to be safe
         }else{
         }
      }   
      Window()->Lock();
      status->Reset("0.0.0.0","0");
      Window()->Unlock();
   }
   Window()->Lock();
   ScanButton->SetMessage(new BMessage(UDPSCAN));
   ScanButton->SetLabel("Scan");   
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
void UDPScannerView::DetachedFromWindow(){
   kill_thread(ScannerThread);
   ScanButton->SetMessage(new BMessage(UDPSCAN));
   ScanButton->SetLabel("Scan");   
   status->Reset("0.0.0.0","0");
}

/*******************************************************
*   Messages, messages messages .. man .. so about the
*   only thing we handle here is our own messages. This
*   is where the thread gets started and stoped. All 
*   user input for us comes here.
*******************************************************/
void UDPScannerView::MessageReceived(BMessage *msg){
   switch(msg->what){
   case UDPSCAN:
    //  ScanButton->SetMessage(new BMessage(UDPSTOP));
    //  ScanButton->SetLabel("Stop");
    //  ScannerThread = spawn_thread(UDPScanNow_Hook, "UDPSnooping around", B_NORMAL_PRIORITY, this);
	 //  resume_thread(ScannerThread);
      (new BAlert(NULL,"Working hard on getting this setup and running. Should be soon but no promisis :)","That stinks"))->Go();
      break;
   case UDPSTOP:
      kill_thread(ScannerThread);
      ScanButton->SetMessage(new BMessage(UDPSCAN));
      ScanButton->SetLabel("Scan");   
      status->Reset("0.0.0.0","0");
      break;
   case IPHISTORY:
      (new BAlert(NULL,"",""))->Go();
      break;
   default:
      BView::MessageReceived(msg);
   }
}



