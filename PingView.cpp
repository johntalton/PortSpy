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
#include <net/socket.h>
#include <netdb.h>
#include <string.h>

#include "Globals.h"
#include "PingView.h"
#include "IPHistoryView.h"

#define PING_MINSIZE 32
#define PING_MAXSIZE (65536 - 100)

typedef struct echo_packet {
	unsigned seq;
	unsigned now[2];	/* unaligned long long */
	char data[PING_MAXSIZE - (3 * sizeof(int))]; /* actual variable-length */
} echo_packet_t;


/*******************************************************
*
*******************************************************/
PingView::PingView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
   BRect b = Bounds();

   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);
   
   
   BStringView *s1 = new BStringView(BRect(0,0,1,1),"","Enter and Domain name or IP address (x.x.x.x) and Ping away.");
   s1->ResizeToPreferred();
   Bb->AddChild(s1);
   
   s1->MoveTo((Bb->Bounds().Width() - s1->Bounds().Width()) / 2,10);
   
   
   Name = new BTextControl(BRect(0,0,100,20),"name","","",new BMessage(PING));
   Name->SetDivider(0);
   Bb->AddChild(Name);
   
   iphv = new IPHistoryView(BRect(0,0,20,20));
   Bb->AddChild(iphv);
   
   PingIt = new BButton(BRect(0,0,1,1), "ping","Ping", new BMessage(PING));
   PingIt->ResizeToPreferred();
   Bb->AddChild(PingIt);
   
   int32 size = Name->Bounds().Width() + 5 + 
                iphv->Bounds().Width() + 5 +
                PingIt->Bounds().Width();
   
   int32 start = (Bb->Bounds().Width() - size) / 2;
   
   Name->MoveTo(start,50);
   iphv->MoveTo(Name->Frame().right+5,50);
   PingIt->MoveTo(iphv->Frame().right+5,50-3);
   
   
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
PingView::~PingView(){
}

/*******************************************************
*
*******************************************************/
void PingView::AttachedToWindow(){
   iphv->SetTarget(this);
}

/*******************************************************
*   Little func to fix up our text boxes so they behave
*   a little more acourding to IP rules
*******************************************************/
void PingView::Limit(BTextView *asdf){
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
int PingView::StringToInt(const char *temp){
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
/*char* PingView::IntToString(int a){
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
BString PingView::MakeIP(int a,int b,int c,int d){
   BString IP("");
   IP << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
   return IP;
}

/*******************************************************
*
*******************************************************/
int32 PingView::PingHost(){
   
   Window()->Lock(); 
   PingOut->SelectAll();
   PingOut->Clear();
   Window()->Unlock();
   
   int s;
	int count = INT_MAX;//
	int size = PING_MINSIZE;
	int wait_time = 1;
	struct sockaddr_in dst;
	struct sockaddr_in from;
	int fromlen;
	echo_packet_t *data;
 	struct fd_set fds;
 	struct timeval tv;
	int datalen;
	struct hostent *h;
	int seq;
	bigtime_t wait_to;
	bigtime_t this_wait_time;
	bigtime_t now;
	int silent;
	int timedout;
	int i;
	
	BString TimeString("");
	
   data = (echo_packet_t*)malloc(size);
   
   BString hname;
   Window()->Lock();
   hname.SetTo(Name->Text());
   Window()->Unlock();
   
   iphv->AddIP(0,0,0,0,hname.String());
   
   h = gethostbyname(hname.String());
	if (h == NULL) {
     Window()->Lock(); 
     PingOut->Insert("Thats not a real host!\n");
     PingIt->SetMessage(new BMessage(PING));
     PingIt->SetLabel("Ping");
     Window()->Unlock(); 
     return 0;
	}
	
	memcpy(&dst.sin_addr, h->h_addr, h->h_length);
  // printf("\"%i\"\n",dst.sin_port);
   
   
   // from here on out ... ?
   // You are not expected to understand this
   
   s = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
   if(s < 0){
      // Cant creat socket
      return 0;
   }
   
   FD_ZERO(&fds);  //What the hell is this
   FD_SET(s, &fds);  // What does this do - and why.
   
   
   for (seq = 1; seq <= count; seq++){ // Start pinging
     // printf("starting pinging\n");
      union {
			bigtime_t now;
			unsigned nowi[2];
		} u;
		
		data->seq = seq;

		for (i = sizeof(bigtime_t); i < size; i++) {
		//	data->data[i] = i;
		}

		u.now = system_time();
		data->now[0] = u.nowi[0];
		data->now[1] = u.nowi[1];
		if (sendto(s, (char *)data, size, 0, (struct sockaddr *)&dst, sizeof(dst)) < 0) {
         Window()->Lock(); 
         PingOut->Insert("We failed to even send a packet out :(\n");
         PingIt->SetMessage(new BMessage(PING));
         PingIt->SetLabel("Ping");
         Window()->Unlock(); 
			//dprintf("sendto failed: %08x\n", errno);
			//perror("ping send");
			closesocket(s);
			return 0;
		}
		this_wait_time = wait_time * 1000000;
		wait_to = u.now + this_wait_time;
		silent = 0;
		for (;;) {
			struct fd_set this_fds;

			timedout = 0;
 			tv.tv_sec = (this_wait_time / 1000000);
 			tv.tv_usec = (this_wait_time - (tv.tv_sec * 1000000));

			this_fds = fds;
			switch (select(s + 1, &this_fds, NULL, NULL, &tv)) {
			case -1:
				//perror("select");
				closesocket(s);
				return 0;
				//exit(1);
			case 0:
				if (!silent) {
				   Window()->Lock(); 
               PingOut->Insert("Ping - Uh .. no Pong.. thats a down host\n");
               Window()->Unlock(); 
					//fprintf(stderr, "ping timed out\n");
				}
				timedout++;
				break;
			}
			silent++;
			if (timedout) {
				break;
			}

			fromlen = sizeof(from);
			datalen = recvfrom(s, (char *)data, size,0, (struct sockaddr *)&from, &fromlen);
			if (datalen < 0) {
				//perror("ping receive");
				closesocket(s);
				//exit(1);
			}
			now = system_time();
			//printf("Host %-15s got sent %d bytes ",inet_ntoa(from.sin_addr), datalen);
			//printf("Ping - Pong on host %-15s",inet_ntoa(from.sin_addr));
			TimeString.SetTo("");
			TimeString << (int32)(seq);
			Window()->Lock(); 
			PingOut->Insert(TimeString.String());
         PingOut->Insert(" Ping - Pong on host ");
         PingOut->Insert(hname.String());
         Window()->Unlock(); 
			if (datalen < size) {
				//printf("short packet! sent %d bytes, got %d\n", size, datalen);
			} else {
				u.nowi[0] = data->now[0];
				u.nowi[1] = data->now[1];
				//printf(" %2d round-trip-time %4.1f msecs\n",data->seq, (now - u.now) / 1000.0);
			   //printf("took about %4.1f msecs\n", (now-u.now)/1000.0);
			   TimeString.SetTo("");
			   TimeString << (int32)((now-u.now)/1000.0);
			   Window()->Lock(); 
            PingOut->Insert(" took about ");
            PingOut->Insert(TimeString.String());
            PingOut->Insert(" msecs\n");
            Window()->Unlock(); 
			}
			this_wait_time = (wait_to - system_time());
			//if (this_wait_time <= 0) {
			//	break;
			//}
		}
	}
	closesocket(s);
   PingIt->SetMessage(new BMessage(PING));
   PingIt->SetLabel("Ping");
   
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
void PingView::DetachedFromWindow(){
   kill_thread(PingThread);
   PingIt->SetMessage(new BMessage(PING));
   PingIt->SetLabel("Ping"); 
}


/*******************************************************
*
*******************************************************/
void PingView::MessageReceived(BMessage *msg){
   //printf("MessageReceived in ping\n");
   switch(msg->what){
   case IP_HISTORY:{
      uint8 a,b,c,d;
      if((msg->FindInt8("a",(int8*)&a) == B_OK) &&
         (msg->FindInt8("b",(int8*)&b) == B_OK) &&
         (msg->FindInt8("c",(int8*)&c) == B_OK) &&
         (msg->FindInt8("d",(int8*)&d) == B_OK)){
         
         //msg->PrintToStream();
         
         BString str("");
         str << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
         Name->SetText(str.String());
      }
      }break;
   case PING:
      
      
      kill_thread(PingThread);
      PingIt->SetMessage(new BMessage(STOPPING));
      PingIt->SetLabel("Stop");
      PingThread = spawn_thread(Ping_Hook, "Ping,ping,ping", B_NORMAL_PRIORITY, this);
	   resume_thread(PingThread);
      break;
   case STOPPING:
      kill_thread(PingThread);
      PingIt->SetMessage(new BMessage(PING));
      PingIt->SetLabel("Ping"); 
      break;
   default:
      //printf("default ping\n");
      //msg->PrintToStream();
      BView::MessageReceived(msg);
   }
}



