#include <stdio.h>
#include <NetworkKit.h>
#include <StopWatch.h>
#include <KernelKit.h>
#include "FetchURL.h"
#include <AppKit.h>

#include "WebSpeedWindow.h"



class	WebSpeed : public BApplication
{
public:
	WebSpeed();
};

WebSpeed::WebSpeed() : BApplication("application/x.vnd-MWN-WebSpeed")
{
	BRect	Frame(100,100,600,500);
	WebSpeedWindow*	Win=new WebSpeedWindow(Frame);
	Win->Show();
}


int	main(void)
{
	WebSpeed	App;
	App.Run();
	return(0);
}