/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <InterfaceKit.h>
#include <AppKit.h>

#include <stdlib.h>

#include "ProxyWindow.h"

#define ENABLE_PROXY 'epxy'
#define CLOSE_PROXY_SETTINGS 'cspx'

/*******************************************************
*
*******************************************************/
ProxyWindow::ProxyWindow():BWindow(BRect(100,100,400,220),"Proxy Settings",B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL ,B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE){

   BView *v = new BView(Bounds(),"",B_FOLLOW_ALL,0);
   v->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
   AddChild(v);
   BRect b = v->Bounds();
   BBox *bb = new BBox(BRect(b.left-1,b.top-1,b.right+1,b.bottom+1),"",B_FOLLOW_ALL);
   v->AddChild(bb);
   
   b = bb->Bounds();
   
   BFont *curfont = new BFont(be_plain_font);
   
   dop = new BCheckBox(BRect(15,15,20,b.right),"","Enable Proxy",new BMessage(ENABLE_PROXY),B_FOLLOW_TOP);
   dop->ResizeToPreferred();
   bb->AddChild(dop);
   
   BString name;
   name.SetTo("Port");
   ptc = new BTextControl(BRect(0,0,10,10),"",name.String(),"8000",NULL);
   ptc->SetDivider((int)ceil(curfont->StringWidth(name.String())));
   ptc->ResizeToPreferred();
   ptc->MoveTo(b.right-ptc->Bounds().Width()-15,b.top+50);
   bb->AddChild(ptc);
   
   name.SetTo("Host ");
   htc = new BTextControl(BRect(10,b.top+50,ptc->Frame().left-10,30),"",name.String(),"my.proxy.net",NULL);
   htc->SetDivider((int)ceil(curfont->StringWidth(name.String())));
   //htc->ResizeToPreferred();
   bb->AddChild(htc);

   htc->SetEnabled(false);
   ptc->SetEnabled(false);

   BButton *canc = new BButton(BRect(0,0,1,1),"","Close",new BMessage(CLOSE_PROXY_SETTINGS),B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
   canc->ResizeToPreferred();
   canc->MoveTo(b.right-canc->Bounds().Width()-10,b.bottom-canc->Bounds().Height()-10);
   bb->AddChild(canc);
   
   
   BScreen scr;
   MoveTo(scr.Frame().Width()/2-b.Width()/2,scr.Frame().Height()/2-b.Height()/2);
   
   
   Run();
}

/*******************************************************
*
*******************************************************/
void ProxyWindow::MessageReceived(BMessage *msg){
   switch(msg->what){
   case ENABLE_PROXY:
      if(dop->Value()){
         htc->SetEnabled(true);
         ptc->SetEnabled(true);
      }else{
         htc->SetEnabled(false);
         ptc->SetEnabled(false);
      }
      break;
   case CLOSE_PROXY_SETTINGS:
      Lock();
      if(!IsHidden()){
         Hide();
      }
      Unlock();
      break;
   default:
      BWindow::MessageReceived(msg);
      break;
   }
}

/*******************************************************
*
*******************************************************/
bool ProxyWindow::QuitRequested(){
   return true;
}

/*******************************************************
*
*******************************************************/
proxy_settings ProxyWindow::GetSettings(){
   proxy_settings ps;
   ps.Host.SetTo(htc->Text());
   ps.Port = atoi(ptc->Text());
   ps.doProxy = dop->Value();
   return ps;
}







