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
#include <SupportKit.h>

#include <stdio.h>
#include <stdlib.h>

#include "FetchURL.h"
#include "Globals.h"
#include "OutputGraph.h"
#include "ProxyWindow.h"
#include "WebSpeedView.h"

#include "IPHistoryView.h"

int			NrTransactions=100;
int			Port=80;
BString		Host="127.0.0.1";
BString		Filename="index.html";

/*******************************************************
*   WebSpeed thread runner :)
*******************************************************/
int32	WebSpeedView::TestWeb(){ 
   proxy_settings ps = proxywin->GetSettings();
   
    //WebSpeedView*	View=(WebSpeedView*)Data; 
	//	char	Message[255]; 
	//sprintf(Message,"Testing %s on port %d, getting file: %s\n",Host.String(),Port,Filename.String()); 
		//Window()->Lock();
	//Output->Insert(Message);
	//Window()->Unlock();
	
   WSstat mystat;
   int Trans=NrTransactions;	//Cache it so other threads won't collide 
   int bi,totbi;
   BString tmpFileName(Filename.String());
   int tmpPort = Port;
   BString tmpHost(Host.String());
   if(ps.doProxy){
      tmpFileName.Prepend("/");
      tmpFileName.Prepend(Host.String());
      tmpFileName.Prepend("http://");
      tmpFileName.Append(":");
      tmpFileName << (int32)Port; // is this right
      tmpPort = ps.Port;
      tmpHost.SetTo(ps.Host.String());
      
   }else{
      tmpFileName.Prepend("/");
   }
	FetchUrl	MyURL(tmpHost.String(),tmpFileName.String(),tmpPort); 
	
	totbi = 0;
	BStopWatch	Watch("Webspeed",true); 
	while(Trans > 0) 
	{ 
	   bi = MyURL.Fetch();
		if(bi == -1){ 
		   mystat.tpsec = -1; // falure
		   ReportTest(mystat);
		  	FinishTest(); 
			//ReportTest("Error could not get page from Server\n\n"); 
			Window()->Lock(); // This kills our Elapsed Time :P
         Output->Insert("Error could not get page from Server\n");
         Window()->Unlock();
			return B_ERROR; 
		}
		totbi += bi;
		UpdateStatus(); // This kills our Elapsed Time :P
		Trans--; 
	} 
	bigtime_t	Elapsed = Watch.ElapsedTime(); 
	
	//FinishTest(); 
	//sprintf(Message,"Elapsed time: %f seconds\nTransactions: %d\nTransactions per second: %f\nTransaction Duration: %f seconds\n\n",(float)Elapsed/1000000,NrTransactions,(float)NrTransactions/((float)Elapsed/1000000),((float)Elapsed/1000000)/(float)NrTransactions); 
	 
	mystat.etime = (float)Elapsed/1000000; //Elapsed time: %f seconds\n 
	mystat.trans = NrTransactions;         //Transactions: %d\n         
	mystat.tpsec = (float)NrTransactions/((float)Elapsed/1000000); //Transactions per second: %f\n   
	mystat.duration = ((float)Elapsed/1000000)/(float)NrTransactions;  //Transaction Duration: %f seconds\n 
	mystat.bytes = totbi;
	
	//ReportTest(Message); 
	ReportTest(mystat);
   FinishTest(); 
	return B_OK; 
} 

/*******************************************************
*   The GUI for the Scanner Window. This view also has
*   all the code that the scaner uses. it is the 
*   threader for the thingy
*******************************************************/
WebSpeedView::WebSpeedView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);

   proxywin = new ProxyWindow();
   

   BRect b = Bounds();
   b.InsetBy(5,5);
   BBox *Bb = new BBox(BRect(b.left,b.top,b.right,125),"Box",B_FOLLOW_TOP_BOTTOM);
   Bb->SetLabel("");
   AddChild(Bb);

	BRect	Temp(10,10,211,24);
	m_Host=new BTextControl(Temp,"Host","Host:","127.0.0.1",NULL);
	m_Host->SetDivider(40);
	Bb->AddChild(m_Host);

   iphv = new IPHistoryView(BRect(0,0,20,20));
   Bb->AddChild(iphv);
   
	iphv->MoveTo(m_Host->Frame().right+5,m_Host->Frame().top);

	Temp.OffsetBy(0,27);

	m_File=new BTextControl(Temp,"File","File:","index.html",NULL);
	m_File->SetDivider(40);
	Bb->AddChild(m_File);

	Temp.Set(250,10,370,24);
	m_Port=new BTextControl(Temp,"Port","Port:","80",NULL);
	m_Port->SetDivider(40);
	Bb->AddChild(m_Port);

	Temp.OffsetBy(0,27);
	m_Count=new BTextControl(Temp,"Count","Count:","100",NULL);
	m_Count->SetDivider(40);
	Bb->AddChild(m_Count);

   
   Temp.OffsetBy(30,50);
	ThreadCount = new BTextControl(Temp,"ThreadCount","Thread Count:","1",NULL);
	ThreadCount->SetDivider(65);
	Bb->AddChild(ThreadCount);
   
   Temp = ThreadCount->Frame();
   Temp.left = Temp.right + 10;
   BCheckBox *cb = new BCheckBox(Temp, "", "ShowGraph",new BMessage(SWITCH_VIEW));
   Bb->AddChild(cb);
   cb->ResizeToPreferred();
   
   m_Test=new BButton(BRect(0,0,1,1),"Start","Start",new BMessage(WSSTART));
	m_Test->ResizeToPreferred();
	Bb->AddChild(m_Test);
	m_Test->MoveTo(Bb->Bounds().right - m_Test->Bounds().Width() - 20, 20);

	
   //Temp = m_Test->Frame();
   //Temp.top = Temp.bottom + 5;
	//ClearOutput=new BButton(Temp,"Clear","Clear",new BMessage(WSCLEAR));
	//ClearOutput->ResizeToPreferred();
	//Bb->AddChild(ClearOutput);
   
   
	BBox *Stat = new BBox(BRect(10,65,275,120),"Box",B_FOLLOW_TOP_BOTTOM);
	Stat->SetLabel("Progress");
	AddChild(Stat);//Bb->
   
	status = new BStatusBar(BRect(10,13,Stat->Bounds().right-10,30),"","","");
	Stat->AddChild(status);
   
   b = Bounds();
   b.InsetBy(10,10);
   b.top = 135;
   b.right = b.right - B_V_SCROLL_BAR_WIDTH;
   b.bottom = b.bottom - B_H_SCROLL_BAR_HEIGHT;// -20 ;

   Output = new BTextView(b,"",BRect(0,0,b.right-20,200),B_WILL_DRAW,B_FOLLOW_ALL_SIDES);
   Output->MakeEditable(false);
   Output->MakeSelectable(true);
   Output->MakeResizable(false);
   Output->SetWordWrap(true);
   AddChild(outview = new BScrollView("",Output,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
   Output->SetViewColor(230,230,230,255);
   //outview->Hide();
   
   b.top -= 2;
   b.left -= 2;
   b.right += B_V_SCROLL_BAR_WIDTH + 2;
   b.bottom += B_H_SCROLL_BAR_HEIGHT + 2;

   OGraph = new OutputGraph(b);
   OGraph->Hide();
   AddChild(OGraph);
   
   


}

/*******************************************************
*
*******************************************************/
WebSpeedView::~WebSpeedView(){
}

/*******************************************************
*
*******************************************************/
void WebSpeedView::FinishTest(){
   threadsleft--;
   if(threadsleft <= 0){
      Window()->Lock();
      m_Test->SetMessage(new BMessage(WSSTART));
      m_Test->SetLabel("Start");
      status->Reset();
      Window()->Unlock();
   }
}

/*******************************************************
*
*******************************************************/
void WebSpeedView::UpdateStatus(){
	Window()->Lock();
	status->Update(1,"");
	Window()->Unlock();
}

/*******************************************************
*
*******************************************************/
void WebSpeedView::ReportTest(WSstat s){
   char Message[255];
   sprintf(Message,"Elapsed time: %f seconds\nTransactions: %d\nTransactions per second: %f\nTransaction Duration: %f seconds\nTotal bytes read: %i\n\n",s.etime,s.trans,s.tpsec,s.duration,s.bytes);  
	Window()->Lock();
	Output->Insert(Message);
	
	OGraph->SetPoint(threadsleft-1,s.tpsec);
	//printf("%s",Message);
	Window()->Unlock();
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
void WebSpeedView::DetachedFromWindow(){

	kill_thread(m_Thread);
	m_Test->SetMessage(new BMessage(WSSTART));
	m_Test->SetLabel("Start");

   //kill_thread(WebSpeedThread);
   //ScanButton->SetMessage(new BMessage(UDPSCAN));
   //ScanButton->SetLabel("Scan");   
   status->Reset("","");
}
/*******************************************************
*   
*******************************************************/
void WebSpeedView::AttachedToWindow(){
	m_Test->SetTarget(this);	//Grab the button
	iphv->SetTarget(this);
	//ClearOutput->SetTarget(this);
}

/*******************************************************
*   Messages, messages messages .. man .. so about the
*   only thing we handle here is our own messages. This
*   is where the thread gets started and stoped. All 
*   user input for us comes here.
*******************************************************/
void WebSpeedView::MessageReceived(BMessage *msg){
   BString thrdName;
   int32 thread_count = 0;
   
   switch(msg->what){
   case VIEW_NOT_ACTIVE:
      //(new BAlert(NULL,"webspeed not active",""))->Go();
      break;
   case VIEW_ACTIVE:
      //(new BAlert(NULL,"webspeed active",""))->Go();
      break;
   case IP_HISTORY:{
      uint8 a,b,c,d;
      if((msg->FindInt8("a",(int8*)&a) == B_OK) &&
         (msg->FindInt8("b",(int8*)&b) == B_OK) &&
         (msg->FindInt8("c",(int8*)&c) == B_OK) &&
         (msg->FindInt8("d",(int8*)&d) == B_OK)){
         
         BString name;
         //if(msg->FindString("name",&name) != B_OK){
            name.SetTo("");
            name << (int32)a << "." << (int32)b << "." << (int32)c << "." << (int32)d;
         //}
         m_Host->SetText(name.String());
         
         
      }
      }break;
   case PROXY_SETTINGS:
      proxywin->Lock();
      if(proxywin->IsHidden()){
         proxywin->Show();
      }
      proxywin->Unlock();
      break;
	case WSSTART:
	{
		Filename.SetTo(m_File->Text());
		Host.SetTo(m_Host->Text());
		Port=atoi(m_Port->Text());
		NrTransactions=atoi(m_Count->Text());
		thread_count = atoi(ThreadCount->Text());

      //Do some needed "error" checking		
		if(thread_count <= 0){
		   (new BAlert(NULL,"Uh, you must use at least one thread!\nRunning with 1 thread","Ok"))->Go();
		   thread_count = 1;
		}
		if(thread_count > WS_MAX_THREAD){
		   (new BAlert(NULL,"What are you trying to do swamp the Web Server\nWe are limiting you to 100 threads","I like DoS"))->Go();
		   thread_count = 100;
		}
		if(NrTransactions <= 0){
		   (new BAlert(NULL,"Count must be larger than 0. Please try to get that right.","Sorry"))->Go();
		   break;
		}
      if(m_Port <= 0){
         (new BAlert("Error","Sorry there seems to be a problem with the values","Okay"))->Go();
      }

      Output->SelectAll();
      Output->Clear();
		status->Reset();
		status->SetMaxValue((NrTransactions*thread_count));
		
		OGraph->SetCount(thread_count);
		
		threadsleft = thread_count;
		
		for(int i = 0;i < thread_count;i++){
		   thrdName.SetTo("Web-Test#");
		   thrdName << (int32)i;
   		m_Thread = spawn_thread(WebSpeed_Hook,thrdName.String(),B_DISPLAY_PRIORITY,(void*)this);
	   	threadPool[i] = m_Thread;
		   resume_thread(m_Thread);
		}
		
		
      m_Test->SetMessage(new BMessage(WSSTOP));
      m_Test->SetLabel("Stop");
      
  //  ScannerThread = spawn_thread(UDPScanNow_Hook, "UDPSnooping around", B_NORMAL_PRIORITY, this);
	  // resume_thread(ScannerThread);
	}
   break;
	case WSSTOP:
		kill_thread(m_Thread);
      m_Test->SetMessage(new BMessage(WSSTART));
      m_Test->SetLabel("Start");
      //ScanButton->SetMessage(new BMessage(UDPSCAN));
      //ScanButton->SetLabel("Scan");   
      status->Reset();
      break;
   case WSCLEAR:
      Output->SelectAll();
      Output->Clear();
      break;
   case SWITCH_VIEW:
      if(OGraph->IsHidden()){
         Output->Hide();
         OGraph->Show();
      }else{
         OGraph->Hide();
         Output->Show();
      }
      break;
 	default:
      BView::MessageReceived(msg);
   }
}



