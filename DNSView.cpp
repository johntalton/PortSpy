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

#include <netdb.h>

#include "Globals.h"
#include "DNSView.h"
#include "IPTextControl.h"

#include "IPHistoryView.h"

/*******************************************************
*   Nice little DNS lookup code. Its useing the old 
*   Be sockets and netdb stuff. So thats not kewl but
*   they dont have much for DNS in the NewtworkKit ohwell
*******************************************************/
DNSView::DNSView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();
   BRect t;

   b.InsetBy(5,5);
   Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);

   t = Bb->Bounds();
   t.InsetBy(10,10);
   t.right = 150;
   BView *tempV = new BView(t,"",B_FOLLOW_ALL_SIDES,0);
   tempV->SetViewColor(216,216,216);
   
   t = tempV->Bounds();
   t.bottom = t.bottom/2;
   ByIP = new BRadioButton(t, "", "Lookup by host name: ",new BMessage(BY_NAME));
   tempV->AddChild(ByIP);
   ByIP->SetValue(1);
   ByIP->ResizeToPreferred();
  
   t.top = t.bottom;
   t.bottom = tempV->Bounds().bottom;
   ByName = new BRadioButton(t, "", "Lookup by IP address: ",new BMessage(BY_IP));
   tempV->AddChild(ByName);
   ByName->ResizeToPreferred();
  
   Bb->AddChild(tempV);

   t = b;
   t.top = 10;
   t.left = 160;
   t.right = 285;
   Name = new BTextControl(t,"name","","",new BMessage(LOOKUP));
   Name->SetDivider(0);
   Bb->AddChild(Name);
 
   t.top = Bb->Bounds().bottom/2;
   t.left = 160;
   t.right = t.left+30;
   
   IP_One = new BTextControl(t,"","","",NULL);
   IP_One->SetDivider(0);
   
   t.left += 32;
   t.right = t.left+30;
   
   IP_Two = new BTextControl(t,"","","",NULL);
   IP_Two->SetDivider(0);
   
   t.left += 32;
   t.right = t.left+30;
   
   IP_Three = new BTextControl(t,"","","",NULL);
   IP_Three->SetDivider(0);
 
   t.left += 32;
   t.right = t.left+30;
   
   IP_Four = new BTextControl(t,"","","",new BMessage(LOOKUP));
   IP_Four->SetDivider(0);

   Bb->AddChild(IP_One);
   Bb->AddChild(IP_Two);
   Bb->AddChild(IP_Three);
   Bb->AddChild(IP_Four);
 
   IP_One->SetEnabled(false);
   IP_Two->SetEnabled(false);
   IP_Three->SetEnabled(false);
   IP_Four->SetEnabled(false);
   
   
   Lookup = new BButton(BRect(350,10,10,10), "dns","Lookup", new BMessage(LOOKUP));
   Lookup->ResizeToPreferred();
   Bb->AddChild(Lookup);
   
   iphv = new IPHistoryView(BRect(0,0,20,20));
   Bb->AddChild(iphv);
   iphv->MoveTo(Name->Frame().right + 5,Lookup->Frame().top);
   
   
   b.InsetBy(5,5);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;

   DNSOut = new BTextView(b,"",BRect(0,0,b.right-20,200),B_WILL_DRAW,B_FOLLOW_ALL_SIDES);
   DNSOut->MakeEditable(false);
   DNSOut->MakeSelectable(true);
   DNSOut->MakeResizable(false);
   DNSOut->SetWordWrap(true);
   AddChild(new BScrollView("",DNSOut,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
   DNSOut->SetViewColor(230,230,230,255);
   
   BTextView *tmpV;
   tmpV = (BTextView*)IP_One->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Two->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Three->ChildAt(0);
   Limit(tmpV);
   tmpV = (BTextView*)IP_Four->ChildAt(0);
   Limit(tmpV);

}

/*******************************************************
*
*******************************************************/
DNSView::~DNSView(){
}

/*******************************************************
*
*******************************************************/
void DNSView::AttachedToWindow(){
   iphv->SetTarget(this);
}


/*******************************************************
*   Little func to fix up our text boxes so they behave
*   a little more acourding to IP rules
*******************************************************/
void DNSView::Limit(BTextView *asdf){
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
*   into a int. Methoud by Chris Camp (cmc008@coes.latech.edu)
*******************************************************/
int DNSView::StringToInt(const char *temp){
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
/*char* DNSView::IntToString(int a){
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
BString DNSView::MakeIP(int a,int b,int c,int d){
   BString IP("");
   IP << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
   return IP;
}

/*******************************************************
*
*******************************************************/
int32 DNSView::DNSLookup(){
   
   Window()->Lock(); 
   DNSOut->SelectAll();
   DNSOut->Clear();
   Window()->Unlock();
   
   struct hostent *theHost = NULL;
   
   int i = 0; 
   h_errno = 0; 
   
   Window()->Lock(); 
   if(!ByIP->Value()){
      ulong addr = inet_addr(
        MakeIP( 
          StringToInt(
            IP_One->Text()
          ),
          StringToInt(
            IP_Two->Text()
          ),
          StringToInt(
            IP_Three->Text()
          ),
          StringToInt(
            IP_Four->Text()
          )
        ).String()
      );
      theHost = gethostbyaddr((const char *)&addr, 4, AF_INET);
   }else{
      theHost = gethostbyname(Name->Text());
   }
   Window()->Unlock(); 
   
   if (!theHost) { 
     Window()->Lock();  
     DNSOut->Insert("Lookup Failed\n");
     Window()->Unlock(); 
   }else{ 
      if (h_errno == TRY_AGAIN) {
         Window()->Lock(); 
         DNSOut->Insert("Lookup via Hosts file\n");
         Window()->Unlock(); 
      }else {
         Window()->Lock(); 
         DNSOut->Insert("Lookup via DNS server\n");
         Window()->Unlock(); 
      }
   }
   
   if(theHost == NULL){ 
      Window()->Lock(); 
      DNSOut->Insert("No hosts by that name");
      Window()->Unlock(); 
   }else{
      Window()->Lock(); 
      DNSOut->Insert("Host Name: ");
      DNSOut->Insert(theHost->h_name);
      DNSOut->Insert("\n");
      Window()->Unlock(); 
   
      while ((theHost->h_aliases)[i] != '\0'){
         Window()->Lock(); 
         DNSOut->Insert("Alias: ");
         DNSOut->Insert(theHost->h_aliases[i++]);
         DNSOut->Insert("\n");
         Window()->Unlock(); 
      }
      
      in_addr ip_address;
      
      i=0;  
      while ((theHost->h_addr_list)[i] != '\0'){
          ip_address.s_addr = *(long *)theHost->h_addr_list[i++];
          Window()->Lock(); 
          DNSOut->Insert("IP: ");
          DNSOut->Insert(inet_ntoa(ip_address));
          DNSOut->Insert("\n");
          Window()->Unlock(); 
      }  
   }
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
void DNSView::DetachedFromWindow(){
   kill_thread(DNSThread);
}

/*******************************************************
*
*******************************************************/
void DNSView::MessageReceived(BMessage *msg){
   switch(msg->what){
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
         IP_Four->SetText(str.String());
         
         BString name;
         if(msg->FindString("name",&name) != B_OK){
            name.SetTo("");
            name << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
         }
         Name->SetText(name.String());
         
         
      }
      }break;
   case LOOKUP:
      DNSThread = spawn_thread(DNS_Hook, "Looken up da name", B_NORMAL_PRIORITY, this);
	   resume_thread(DNSThread);
      break;
   case BY_NAME:
      IP_One->SetEnabled(false);
      IP_Two->SetEnabled(false);
      IP_Three->SetEnabled(false);
      IP_Four->SetEnabled(false);
      Name->SetEnabled(true);
      break;
   case BY_IP:
      IP_One->SetEnabled(true);
      IP_Two->SetEnabled(true);
      IP_Three->SetEnabled(true);
      IP_Four->SetEnabled(true);
      Name->SetEnabled(false);
      break;
   default:
      BView::MessageReceived(msg);
   }
}



