#ifndef _WEB_SPEED_VIEW_H
#define _WEB_SPEED_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <NetworkKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include "OutputGraph.h"
#include "ProxyWindow.h"

class IPHistoryView;

#define WS_MAX_THREAD 100

struct WSstat{
   float etime; //Elapsed time for the thread
   float tpsec;     //Transactions per second
   float duration; //Duration this thread ran for
   int trans;       //Number of transaction preformed by this thread
   int32 bytes;
   
};

class WebSpeedView : public BView {
   public:
      WebSpeedView(BRect);
      ~WebSpeedView();
      virtual void AttachedToWindow();
      virtual void DetachedFromWindow();
      virtual void MessageReceived(BMessage*);
	
		void		FinishTest();
		void		UpdateStatus();

		void		ReportTest(WSstat);
      static int32 WebSpeed_Hook(void* obj){
			return ((WebSpeedView*)obj)->TestWeb();
		}
		int32 TestWeb();
   private:
      ProxyWindow *proxywin;
      
      thread_id WebSpeedThread;
      BTextView *Output;
      BStatusBar *status;

		OutputGraph *OGraph;
		BScrollView *outview;
		
		BTextControl*	m_Host;
		BTextControl*	m_File;
		BTextControl*	m_Port;
		BTextControl*	m_Count;
		BButton*			m_Test; 	
		
		IPHistoryView *iphv;
		
		//BButton *ClearOutput;
		BTextControl *ThreadCount;

      thread_id m_Thread;
		thread_id threadPool[WS_MAX_THREAD]; // Limit ourselfs to only 100 threads - thats alot
      WSstat serverStats[WS_MAX_THREAD]; // A list of all the stats for each thread
      int threadsleft;
};
#endif