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
#include "FingerView.h"

/*******************************************************
*
*******************************************************/
FingerView::FingerView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
  BRect b = Bounds();

  goodID = false;


   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Mybox",B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT);
   Bb->SetLabel("");
   AddChild(Bb);
    
   BStringView *s1 = new BStringView(BRect(100,10,b.right-20,25),"","Works just like any other finger unix    name@host or @host");
   Bb->AddChild(s1);
   
  
   BRect t = b;
   t.top = 55;
   t.left = 160;
   t.right = 285;
   Name = new BTextControl(t,"fing","","",new BMessage(FINGER));
   Name->SetDivider(0);
   Bb->AddChild(Name);
   
   FingerNow = new BButton(BRect(290,53,10,10), "finger","Finger", new BMessage(FINGER));
   FingerNow->ResizeToPreferred();
   Bb->AddChild(FingerNow);
   
   /*b = Bounds();
   b.InsetBy(8,8);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;
  */ 
   b = Bounds();
   b.InsetBy(10,10);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;
   
   FingerOut = new BTextView(b,"",BRect(0,0,b.right-20,200),B_WILL_DRAW,B_FOLLOW_ALL_SIDES);
   FingerOut->MakeEditable(false);
   FingerOut->MakeSelectable(true);
   FingerOut->MakeResizable(false);
   FingerOut->SetWordWrap(true);
   AddChild(new BScrollView("",FingerOut,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
   FingerOut->SetViewColor(230,230,230,255);
}

/*******************************************************
*
*******************************************************/
FingerView::~FingerView(){
}

/*******************************************************
*
*******************************************************/
int32 FingerView::Finger(){
   int c = 0;
   BString tmp;
   BString name;
   int i;
   BString server;
   
   Window()->Lock(); 
   FingerOut->SelectAll();
   FingerOut->Clear();
   Window()->Unlock();
 
   BNetEndpoint *control = new BNetEndpoint;
   BNetAddress addr;
 
   if(control->InitCheck() != B_NO_ERROR){
      (new BAlert(NULL,"Could not creat BNetEndpoint\nTry Again","Ok"))->Go();
      return 0;
   }
   
   // Split name at @
   Window()->Lock();
   tmp = Name->Text();
   Window()->Unlock();

   i = tmp.FindFirst("@");
   if(i < 0){ // there is no @ this must be just a name @localhost
      name = tmp;
      server.SetTo("localhost");
      Window()->Lock();
      FingerOut->Insert("You are trying to look up a \"name\" at localhost\n");
      FingerOut->Insert("Be does not curently run a fingerd. So this will fail\n");
      FingerOut->Insert("We include this for completness, and for the fact that it will some day work\n");
      Window()->Unlock();
   }else if(i == 0){ //finger all at this address @host
      server.SetTo("");
      tmp.MoveInto(server,1,tmp.CountChars()-1);
      name.SetTo("");     
   }else{  // we have a i > 0 must have a name@host setup
      name = tmp.MoveInto(name,0,i);
      server.SetTo("");
      server = tmp.MoveInto(server,1,tmp.CountChars()-1);
   }
   //name.SetTo(Name->Text());
   name.Append("\n");
   addr.SetTo(server.String(), 79); // 79 is finger port  
   
   
   if(control->Connect(addr) == B_NO_ERROR){
      control->Send(name.String(),name.Length());
      while((control->Receive(&c, 1) > 0)){
         tmp.SetTo("");
         if(c == '\r'){
         }else{
            tmp << (char(c));
         }
         Window()->Lock();
         FingerOut->Insert(tmp.String());
         Window()->Unlock();
      }
   }else{
      Window()->Lock();
      FingerOut->Insert("Server is unknown or not working\n");
      FingerOut->Insert("If you think this is a error - email me at ynop@acm.org\n");   
      Window()->Unlock();
   }
  // control->Close();
    
   goodID = false;  
   return B_OK;
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
void FingerView::DetachedFromWindow(){
   if(goodID){
      kill_thread(FingerThread);
   }
   
}


/*******************************************************
*
*******************************************************/
void FingerView::MessageReceived(BMessage *msg){
   switch(msg->what){
   case FINGER:
      if(goodID){
         kill_thread(FingerThread);
      }
      FingerThread = spawn_thread(Finger_Hook, "Fingertips", B_NORMAL_PRIORITY, this);
	  goodID = true;
	  resume_thread(FingerThread);
      break;
   default:
      BView::MessageReceived(msg);
   }
}



