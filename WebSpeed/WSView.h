#include <InterfaceKit.h>
#include <AppKit.h>
#include <SupportKit.h>
#include <stdlib.h>
#include "FetchURL.h"

#define	BUTTON_TEST		1000

class WSView : public BView
{
public:
	WSView(BRect Frame);
	void	AttachedToWindow(void);
	void	MessageReceived(BMessage* message);
	void	UpdateLook();
	void	ShutdownThread();
private:
	BTextControl*	m_Host;
	BTextControl*	m_File;
	BTextControl*	m_Port;
	BTextControl*	m_Count;
	BButton*		m_Test; 	
	BStatusBar*		m_Status;

	thread_id		m_Thread;
};