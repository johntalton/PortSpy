#include "WSView.h"

int			NrTransactions=1000;
int			Port=80;
BString		Host="127.0.0.1";
BString		Filename="index.html";

int32	TestWeb(void *Data)
{
	WSView*	View=(WSView*)Data;
	if(NrTransactions>0&&Port>0)
	{
		int		Trans=NrTransactions;	//Cache it so other threads won't collide

		BStopWatch	Watch("Webspeed",true);

		FetchUrl	MyURL(Host.String(),Filename.String(),Port);
		while(Trans>0)
		{
			MyURL.Fetch();
			View->UpdateLook();
			MyURL.SetTo(Host.String(),Filename.String(),Port);
			Trans--;
		}

		bigtime_t	Elapsed=Watch.ElapsedTime();
		char	Message[255];
		
		sprintf(Message,"Elapsed time: %f seconds\nTransactions: %d\nTransactions per second: %f\nTransaction Duration: %f seconds",(float)Elapsed/1000000,NrTransactions,(float)NrTransactions/((float)Elapsed/1000000),((float)Elapsed/1000000)/(float)NrTransactions);
		BAlert*	b=new BAlert("Result",Message,"Okay");
		b->Go();

	}
	else
	{
		BAlert*	b=new BAlert("Error","Sorry there seems to be a problem with the values","Okay");
		b->Go();
	}
	return(0);
}

WSView::WSView(BRect Frame) : BView(Frame,"WSView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW)
{
	SetViewColor(216,216,216,0);
	
	BRect	Temp(5,10,241,24);
	m_Host=new BTextControl(Temp,"Host","Host:","127.0.0.1",NULL);
	AddChild(m_Host);
	Temp.OffsetBy(0,30);

	m_File=new BTextControl(Temp,"File","File:","index.html",NULL);
	AddChild(m_File);
	Temp.OffsetBy(0,30);
	
	Temp.right=Temp.left+113;
	m_Port=new BTextControl(Temp,"Port","Port:","80",NULL);
	AddChild(m_Port);
	Temp.OffsetBy(123,0);

	m_Count=new BTextControl(Temp,"Count","Count:","100",NULL);
	AddChild(m_Count);

	m_Host->SetDivider(40);
	m_File->SetDivider(40);
	m_Port->SetDivider(40);
	m_Count->SetDivider(40);
	
	Temp.OffsetBy(0,24);
	m_Test=new BButton(Temp,"Test","Test",new BMessage(BUTTON_TEST));
	AddChild(m_Test);
	
	Temp.left=5;
	Temp.right=241;
	Temp.OffsetBy(0,30);
	Temp.bottom=Temp.top+40;
	
	Temp.InsetBy(3,5);
	Temp.OffsetBy(0,-5);
	m_Status=new BStatusBar(Temp,"Status","Status","");
	AddChild(m_Status);
}

void	WSView::AttachedToWindow(void)
{
	m_Test->SetTarget(this);
}

void	WSView::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case BUTTON_TEST:
	{
		Filename.SetTo(m_File->Text());
		Host.SetTo(m_Host->Text());
		Port=atoi(m_Port->Text());
		NrTransactions=atoi(m_Count->Text());
		
		m_Status->Reset();
		m_Status->SetMaxValue(NrTransactions);
		m_Thread=spawn_thread(TestWeb,"Web-test",B_DISPLAY_PRIORITY,(void*)this);
		resume_thread(m_Thread);
	}
	break;
	default:
		BView::MessageReceived(message);
		break;
	}
}

void	WSView::ShutdownThread()
{
	kill_thread(m_Thread);		//Make sure it doesn't run
}

void	WSView::UpdateLook()
{
	Window()->Lock();
	m_Status->Update(1,"Status");
	Window()->Unlock();
}

