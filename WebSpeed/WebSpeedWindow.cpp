#include "WebSpeedWindow.h"

WebSpeedWindow::WebSpeedWindow(BRect Frame) : BWindow(Frame,"WebSpeed V1.0",B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE)
{
	BRect	Temp=Bounds();

	m_WSView=new WebSpeedView(Temp);

	AddChild(m_WSView);
}

bool	WebSpeedWindow::QuitRequested(void)
{
	//m_WSView->ShutdownThread();
	be_app->PostMessage(new BMessage(B_QUIT_REQUESTED));
	return(true);
}