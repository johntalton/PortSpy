#include <InterfaceKit.h>
#include <AppKit.h>
#include "WebSpeedView.h"


class WebSpeedWindow : public BWindow
{
public:
	WebSpeedWindow(BRect Frame);
	bool	QuitRequested(void);
private:
	BTabView*	m_TabView;
	WebSpeedView*	m_WSView;	
};