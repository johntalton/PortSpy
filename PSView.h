#ifndef _ANEWS_VIEW_H
#define _ANEWS_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "ScannerView.h"
#include "UDPScannerView.h"
#include "DNSView.h"
#include "TraceRView.h"
#include "PingView.h"
#include "WhoisView.h"
#include "FingerView.h"
#include "WebSpeedView.h"
#include "AboutView.h"
#include "NTPView.h"

class PSView : public BView {
   public:
      PSView(BWindow*,BRect);
      ~PSView();
      virtual void AttachedToWindow();
      virtual void MessageReceived(BMessage*);
   private:
      BWindow *parentWindow;
      
      FingerView     *FView;
      ScannerView    *SView;
      DNSView        *DView;     
      TraceRView     *TView; 
      PingView       *PView;
      WhoisView      *WView;
      UDPScannerView *UView;
      WebSpeedView   *MView;
      AboutView      *AView;
      NTPView        *NView;
      
      BBox *StatusBar;
      BTabView *tabView;
      BTab *tab;
      
      BBox *Bb;
      BOutlineListView *NGList;
      BOutlineListView *GroupList;
      BTextView *NewsArtical;
      BTextControl *From;
      BTextControl *Subject;
};
#endif