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

#include <netdb.h>
#include <iostream.h>

#include "Globals.h"
#include "ScannerView.h"
#include "IPItem.h"
#include "IPListView.h"
#include "IPHistoryView.h"
#include "PingHost.h"
#include "PortRunner.h"

/*******************************************************
*   The GUI for the Scanner Window. This view also has
*   all the code that the scaner uses. it is the 
*   threader for the thingy
*******************************************************/
ScannerView::ScannerView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();

   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
   
   BRect t = Bb->Bounds();
   
   t.top = 25;
   t.bottom = 40;
   t.left += 20;
   t.right = t.left+30;
   
   IP_One = new BTextControl(t,"","","",NULL,B_FOLLOW_LEFT_RIGHT);
   //IP_One->ResizeToPreferred();
   IP_One->SetDivider(0);
   
   t.left += 32;
   t.right = t.left+30;
   
   IP_Two = new BTextControl(t,"","","",NULL,B_FOLLOW_LEFT_RIGHT);
   //IP_Two->ResizeToPreferred();
   IP_Two->SetDivider(0);
   
   t.left += 32;
   t.right = t.left+30;
 
   IP_Three = new BTextControl(t,"","","",NULL,B_FOLLOW_LEFT_RIGHT);
   //IP_Three->ResizeToPreferred();
   IP_Three->SetDivider(0);
   
   t.left += 32;
   t.right = t.left+30;
   t.top -= 10;
   t.bottom -= 10;
   
   IP_Four_Start = new BTextControl(t,"","","",NULL,B_FOLLOW_LEFT_RIGHT);
   //IP_Four_Start->ResizeToPreferred();
   IP_Four_Start->SetDivider(0);
   
   t.top += 20;   
   t.bottom += 20;
   
   IP_Four_Finish = new BTextControl(t,"","","",NULL,B_FOLLOW_LEFT_RIGHT);
   //IP_Four_Finish->ResizeToPreferred();
   IP_Four_Finish->SetDivider(0);
      
   Bb->AddChild(IP_One);
   Bb->AddChild(IP_Two);
   Bb->AddChild(IP_Three);
   Bb->AddChild(IP_Four_Start);
   Bb->AddChild(IP_Four_Finish);
   
   
   //BMenuField *IPMenu = new BMenuField(BRect(150,25,175,30),"","",IPList);
   iphv = new IPHistoryView(BRect(0,0,20,20));
   Bb->AddChild(iphv);
   
   iphv->MoveTo(IP_Four_Start->Frame().right+5,IP_Three->Frame().top);
   
   ScanButton = new BButton(BRect(180,25,10,10), "scan","Scan", new BMessage(SCAN));
   ScanButton->ResizeToPreferred();
   Bb->AddChild(ScanButton);
   
   t = Bb->Bounds();
   t.left = t.right - 200;
   t.right -= 20;
   t.top += 10;
   t.InsetBy(5,5);
   /*
   t.top = 10;
   ftp = new BCheckBox(t, "", "FTP",NULL);
   Bb->AddChild(ftp);
   ftp->ResizeToPreferred();
   t.top += 20;      
   http = new BCheckBox(t, "", "HTTP",NULL);
   Bb->AddChild(http);
   http->ResizeToPreferred();
   t.top += 20;  
   telnet = new BCheckBox(t, "", "TELNET",NULL);
   Bb->AddChild(telnet);
   telnet->ResizeToPreferred();
   
   t.top = 10; t.left += 75;
   samba = new BCheckBox(t, "", "SAMBA",NULL);
   Bb->AddChild(samba);
   samba->ResizeToPreferred();   
   t.top += 20;
   dns = new BCheckBox(t, "", "DNS",NULL);
   Bb->AddChild(dns);
   dns->ResizeToPreferred();
   t.top += 20;
   echo = new BCheckBox(t, "", "ECHO",NULL);
   Bb->AddChild(echo);
   echo->ResizeToPreferred();

   t.top = 10; t.left += 75;
   nntp = new BCheckBox(t, "", "NNTP",NULL);
   Bb->AddChild(nntp);
   nntp->ResizeToPreferred();
   t.top += 20;
   smtp = new BCheckBox(t, "", "SMTP",NULL);
   Bb->AddChild(smtp);
   smtp->ResizeToPreferred();
   t.top += 20;
   pop3 = new BCheckBox(t, "", "POP3",NULL);
   Bb->AddChild(pop3);
   pop3->ResizeToPreferred();
   
   */
   
  // BView *red = new BView(t,"",B_FOLLOW_NONE,0);
  // red->SetViewColor(255,0,0);
  // Bb->AddChild(red);
  
   portsv = new BTextControl(t,"","Ports","",NULL,B_FOLLOW_LEFT_RIGHT);
   portsv->SetDivider(25);
   
   BTextView *textV = (BTextView*)portsv->ChildAt(0);
   Limit(textV);
   textV->AllowChar(',');
   textV->AllowChar('-');
   textV->SetMaxBytes(1000);
   portsv->SetText("21,23,80-85");
   Bb->AddChild(portsv);
   /*
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
   */
   BBox *Stat = new BBox(BRect(10,65,275,120),"Box",B_FOLLOW_TOP_BOTTOM);
   Stat->SetLabel("Progress");
   AddChild(Stat);
   
   status = new BStatusBar(BRect(10,15,Stat->Bounds().right-10,100),"","0.0.0.0","0");
   Stat->AddChild(status);
   
   t = b;
   t.InsetBy(5,5);
   t.top = 135;
   t.right = t.right - B_V_SCROLL_BAR_WIDTH;
   t.bottom = t.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;
   
   ScannedList = new IPListView(t);
   ScannedList->SetViewColor(230,230,230,255);
   AddChild(new BScrollView("",ScannedList,B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW,true,true));
   
   //////////////////////////////
   // Stuff that does not work //
   //////////////////////////////
   //IP_Four_Finish->SetEnabled(false);
   /*ftp->SetEnabled(false);
   http->SetEnabled(false);
   telnet->SetEnabled(false);
   samba->SetEnabled(false);
   dns->SetEnabled(false);
   echo->SetEnabled(false);
   nntp->SetEnabled(false);
   smtp->SetEnabled(false);
   pop3->SetEnabled(false); 
  */
   BTextView *tmpV;
   tmpV = (BTextView*)IP_One->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Two->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Three->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Four_Start->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Four_Finish->ChildAt(0);
   Limit(tmpV);

}

/*******************************************************
*
*******************************************************/
ScannerView::~ScannerView(){
}

/*******************************************************
*
*******************************************************/
void ScannerView::AttachedToWindow(){
   iphv->SetTarget(this);
}

/*******************************************************
*   Little func to fix up our text boxes so they behave
*   a little more acourding to IP rules
*******************************************************/
void ScannerView::Limit(BTextView *asdf){
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
int ScannerView::StringToInt(const char *temp){
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
BString ScannerView::MakeIP(int a,int b,int c,int d){
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
int32 ScannerView::ScanNow(){
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
   ulong addrForDNS;
   struct hostent *theHost;
   bool DoDNS = true;

   if(control->InitCheck() != B_NO_ERROR){
      (new BAlert(NULL,"BNetEndpoint failed\nThis could meen networking is down!","Ok"))->Go();
      Window()->Unlock(); // almost forgot this
      return 0;
   }
   /*
   int MaxPort = StringToInt(To->Text());
   int MinPort = StringToInt(From->Text());*/
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
      //Window()->Lock();
      LockLooper();
      hostname.SetTo(MakeIP(StringToInt(IP_One->Text()),
                            StringToInt(IP_Two->Text()),
                            StringToInt(IP_Three->Text()),
                            CurIP));

      status->Reset("","");
      char *ptext = (char*)portsv->Text();
      PortRunner pr(ptext);
      //pr.PrintPorts();
      //status->SetMaxValue(MaxPort - MinPort);
      status->SetMaxValue(pr.CountPorts());

      //DoDNS = DNSLookup->Value();
      
      UnlockLooper();
      
      
      portList.SetTo("");
        
      item = new IPItem(hostname.String(),portList);
      
      Window()->Lock();
      ScannedList->AddItem(item);
      Window()->Unlock();
      
      // We need some code right here to check to see if we have a IP/Host
      // that is down. If so Dont scan it .. it will timeout ... uhh longtime
      // PingHost returns true if the host is up and false if the host 
      // is down .. we pass it a string to the host name.
      LockLooper();
      status->Reset("Pinging host ->",hostname.String());
      UnlockLooper();
      bool hostup = PingHost(hostname.String());
      LockLooper();
      status->Reset("","");
      UnlockLooper();
      if(!hostup){
         Window()->Lock();
         item->SetHostDown();
         item->SetHostName("Down host");
         ScannedList->InvalidateItem(CurIP - StartIP); // had to do this so that it would redraw.
         Window()->Unlock();
         continue;
      }
      
      // This does our DNS lookup if the user wants                      
      if(DoDNS){
         LockLooper();
         status->Reset("Resolveing DNS for ->",hostname.String());
         UnlockLooper();
         addrForDNS = inet_addr(hostname.String());
         theHost = gethostbyaddr((const char *)&addrForDNS, 4, AF_INET);
         if(theHost){
            item->SetHostName(theHost->h_name);
         }else{
            item->SetHostName("No Name");
         }
         LockLooper();
         ScannedList->InvalidateItem(CurIP - StartIP);
         status->Reset("","");
         UnlockLooper();
      }
      printf("port scann\n");
      //for(a = MinPort;a <= MaxPort;a++){
      while((a = pr.GetNextPort()) != 0){
         printf("scanning on port %i\n",a);
         tmp.SetTo("");
         tmp << (int32)a;
         Window()->Lock();
         status->Update(1,hostname.String(),tmp.String());
         Window()->Unlock();
         addr.SetTo(hostname.String(), a);
         if(control->Connect(addr) == B_OK){
            portList << (int32)a << "  ";
            item->AddPort(a);
            Window()->Lock();
            ScannedList->InvalidateItem(CurIP - StartIP); // had to do this so that it would redraw.
            Window()->Unlock();
            control->Close(); // addind this in just to be safe
         }else{
            //Window()->Lock();
            //item->SetHostDown();
            //ScannedList->InvalidateItem(CurIP - StartIP); // had to do this so that it would redraw.
            //Window()->Unlock();
         }
      }   
      Window()->Lock();
      status->Reset("Finished","");
      Window()->Unlock();
   }
   Window()->Lock();
   ScanButton->SetMessage(new BMessage(SCAN));
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
void ScannerView::DetachedFromWindow(){
   kill_thread(ScannerThread);
   ScanButton->SetMessage(new BMessage(SCAN));
   ScanButton->SetLabel("Scan");   
   status->Reset("0.0.0.0","0");
}

/*******************************************************
*   Messages, messages messages .. man .. so about the
*   only thing we handle here is our own messages. This
*   is where the thread gets started and stoped. All 
*   user input for us comes here.
*******************************************************/
void ScannerView::MessageReceived(BMessage *msg){
   switch(msg->what){
      case VIEW_ACTIVE:
      //(new BAlert(NULL,"scanner active",""))->Go();
      break;
   case IP_HISTORY:{
      uint8 a,b,c,d;
      if((msg->FindInt8("a",(int8*)&a) == B_OK) &&
         (msg->FindInt8("b",(int8*)&b) == B_OK) &&
         (msg->FindInt8("c",(int8*)&c) == B_OK) &&
         (msg->FindInt8("d",(int8*)&d) == B_OK)){
         
         //msg->PrintToStream();
         
         BString str("");  str << (int32)a;
         //(new BAlert(NULL,str.String(),""))->Go();
         IP_One->SetText(str.String());
         str.SetTo(""); str << (int32)b;
         IP_Two->SetText(str.String());
         str.SetTo(""); str << (int32)c;
         IP_Three->SetText(str.String());
         str.SetTo(""); str << (int32)d;
         IP_Four_Start->SetText(str.String());
         IP_Four_Finish->SetText(str.String());
      }
      }break;
   case SCAN:
      {
      BString str(IP_One->Text()); str.Append(".");
      str.Append(IP_Two->Text()); str.Append(".");
      str.Append(IP_Three->Text()); str.Append(".");
      str.Append(IP_Four_Start->Text());
      iphv->AddIP(StringToInt(IP_One->Text()),StringToInt(IP_Two->Text()),StringToInt(IP_Three->Text()),StringToInt(IP_Four_Start->Text()),str.String());
      }
      
      ScanButton->SetMessage(new BMessage(STOP));
      ScanButton->SetLabel("Stop");
      ScannerThread = spawn_thread(ScanNow_Hook, "Snooping around", B_NORMAL_PRIORITY, this);
	   resume_thread(ScannerThread);
      break;
   case STOP:
      kill_thread(ScannerThread);
      ScanButton->SetMessage(new BMessage(SCAN));
      ScanButton->SetLabel("Scan");   
      status->Reset("0.0.0.0","0");
      break;
   case IPHISTORY:
      break;
   default:
      BView::MessageReceived(msg);
   }
}



