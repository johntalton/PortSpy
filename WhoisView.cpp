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

#include "Globals.h"
#include "WhoisView.h"

/*******************************************************
*   Whois who.  We set up a prity little GUI and spin
*   off some thread when the users wants something .. 
*   nice hhuu
*******************************************************/
WhoisView::WhoisView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();
   BRect t;

   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
  
   WhoButton = new BButton(BRect(260,22,10,10), "who","Whois", new BMessage(WHO));
   WhoButton->ResizeToPreferred();
   Bb->AddChild(WhoButton);
   

   t = b;
   t.top = 25;
   t.left = 10;
   t.right = 250;
   Name = new BTextControl(t,"","Enter IP/Name to lookup: ","internic.net",new BMessage(WHO));
   Bb->AddChild(Name);

   WhoServer.SetTo("whois.internic.net");
   whoisServerList = new BPopUpMenu("whois.internic.net");
   whoisServerList->AddItem(new BMenuItem("whois.internic.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.nsiregistry.com",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.nic.fr",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.apnic.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.arin.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.ra.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.ripe.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.6bone.net",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.nic.mil",new BMessage(NEWWHOSERVER)));
   whoisServerList->AddItem(new BMenuItem("whois.networksolutions.com",new BMessage(NEWWHOSERVER)));
   //whoisServerList->AddItem(new BMenuItem("whois.dns.nl",new BMessage(NEWWHOSERVER)));
   
   whoisServerList->AddItem(new BMenuItem("Some Other Whois Server",new BMessage(NEWWHOSERVER)));

 
   t = b;
   BMenuField *ServerMenu = new BMenuField(BRect(t.right-152,22,t.right,40),"","",whoisServerList);
   ServerMenu->SetDivider(0);
   Bb->AddChild(ServerMenu);


   t = b;
   t.top = 65;
   t.left = t.right - 150;
   t.bottom = t.top + 40;
   t.right -= 10;
   BView *tempV = new BView(t,"",B_FOLLOW_ALL_SIDES,0);
   Bb->AddChild(tempV);
   tempV->SetViewColor(216,216,216,0);

   t.top = 0;
   t.left = tempV->Bounds().left;
   full = new BRadioButton(t, "", "Full output of whois",NULL);
   tempV->AddChild(full);
   full->SetValue(1);
   full->ResizeToPreferred();
  
   t.top += 25;
   min = new BRadioButton(t, "", "Minimal output of whois",NULL);
   tempV->AddChild(min);
   min->ResizeToPreferred();
  
   
   BBox *Stat = new BBox(BRect(10,65,275,120),"Box",B_FOLLOW_TOP_BOTTOM);
   Stat->SetLabel("Progress");
   AddChild(Stat);
   
   status = new BStatusBar(BRect(10,15,Stat->Bounds().right-10,100),"","Connection Status","");
   Stat->AddChild(status); 
  
   
   b.InsetBy(5,5);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;

   WhoOut = new BTextView(b,"",BRect(0,0,b.right-20,200),B_WILL_DRAW,B_FOLLOW_ALL_SIDES);
   WhoOut->MakeEditable(false);
   WhoOut->MakeSelectable(true);
   WhoOut->MakeResizable(false);
   WhoOut->SetWordWrap(true);
   AddChild(new BScrollView("",WhoOut,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
   WhoOut->SetViewColor(230,230,230,255);
  
}

/*******************************************************
*
*******************************************************/
WhoisView::~WhoisView(){
}

/*******************************************************
*   Main Thread that does the whois work.  The thread 
*   has lots of locking involved to alow us to update
*   the GUI and to alow other messgaes to run while we
*   run in the background.  this function took alot of 
*   work to get right..
*******************************************************/
int32 WhoisView::Who(){
   int c = 0;
   bool done = false;
   BString tmp;
   BString name;

   Window()->Lock(); 
   WhoOut->SelectAll();
   WhoOut->Clear();
   Window()->Unlock();
 
   BNetEndpoint *control = new BNetEndpoint;
   BNetAddress addr;
 
   if(control->InitCheck() != B_NO_ERROR){
      (new BAlert(NULL,"Could not creat BNetEndpoint\nTry Again","Ok"))->Go();
      return 0;
   }
   Window()->Lock();
   status->Reset("","");
   //status->Reset("Fetching Whois info","");
   status->SetMaxValue(1500);
   Window()->Unlock();
   addr.SetTo(WhoServer.String(), 43);  // "whois.internic.net" Whois port is 43  
   if(control->Connect(addr) == B_NO_ERROR){
      name.SetTo(Name->Text());
      name.Append("\n");
      control->Send(name.String(),name.Length());
      while(done == false && (control->Receive(&c, 1) > 0)){
         Window()->Lock();
         status->Update(1,"Fetching Whois info","");
         Window()->Unlock();
         tmp.SetTo("");
         tmp << (char(c));
         Window()->Lock();
         WhoOut->Insert(tmp.String());
         Window()->Unlock();
      }
   }else{
      Window()->Lock();
      WhoOut->Insert("Hmm, could not connect to that whois server.\nIf you have any other problems or questions contact me: ynop@acm.org\n");   
      WhoOut->Insert("If you would like to add a whois server mail me and I will add it in. Current ver does not have any way to input it yourself");
      Window()->Unlock();
   }
   Window()->Lock();
   status->Reset("Connection Status","");
   WhoButton->SetEnabled(true);
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
void WhoisView::DetachedFromWindow(){
   kill_thread(WhoisThread);
   status->Reset("Connection Status","");
   WhoButton->SetEnabled(true);
}

/*******************************************************
*   Messages for us come here - and we handle them. 
*   this is also where we spin off the thread for the
*   whois 
*******************************************************/
void WhoisView::MessageReceived(BMessage *msg){
   BMenuItem *tmpI;
   
   switch(msg->what){
   case WHO:
      WhoisThread = spawn_thread(Who_Hook, "Who wants to know?", B_NORMAL_PRIORITY, this);
      // Lock the whois button so we dont get a bunch of threads.
      WhoButton->SetEnabled(false);
	   resume_thread(WhoisThread);
      break;
   case NEWWHOSERVER:
      tmpI = whoisServerList->FindMarked();
      WhoServer.SetTo(tmpI->Label());
      break;
   default:
      BView::MessageReceived(msg);
   }
}



