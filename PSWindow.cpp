/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>

#include <unistd.h>

#include "Globals.h"
#include "PSWindow.h"
#include "PSView.h"

/*******************************************************
*   Our wonderful BWindow, ya its kewl like that.
*   we dont do much here but set up the menubar and 
*   let the view take over.  We also nead some message
*   redirection and handling
*******************************************************/
PSWindow::PSWindow(BRect frame) : BWindow(frame,"PortSpy   ",B_TITLED_WINDOW,B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_ASYNCHRONOUS_CONTROLS){//
   BRect r;
   BMenu *menu;
   BMenuItem *item;
   
   r = Bounds();
   // Creat a standard menubar
   menubar = new BMenuBar(r, "MenuBar");
   // Standard File menu
   menu = new BMenu("File");
   menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("About PortSpy", new BMessage(B_ABOUT_REQUESTED), 'A'));
   item->SetTarget(be_app);
   menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
   // add File menu now
   menubar->AddItem(menu);
   // Stander Options menu
   menu = new BMenu("Options");
   menu->AddItem(item=new BMenuItem("Open Terminal", new BMessage(OPEN_TERM), 'T'));
   menu->AddItem(new BMenuItem("Crash",new BMessage(CRASH)));
   // add Options menu now
   menubar->AddItem(menu);
   
   // Attach the menu bar to he window
   AddChild(menubar);
   
   // Do a little claculating to take menubar int account
   r = Bounds();
   r.bottom = r.bottom - menubar->Bounds().bottom;
   View = new PSView(this,r);
   View->MoveBy(0, menubar->Bounds().Height() + 1);
   AddChild(View);
 
   View->MakeFocus(true);
   Show(); // this should be in ReadToRun ...
}

/*******************************************************
*   More nothingness. pass menu msg down to View.
*   like new game and pause and stuff like that.
*******************************************************/
void PSWindow::MessageReceived(BMessage* msg){
   switch(msg->what){
   case WHO:
   case STOP:
   case LOOKUP:
   case BY_NAME:
   case BY_IP:
   case PING:
   case STOPPING:
   case UDPSCAN:
   case UDPSTOP:
   case NEWWHOSERVER:
   case IPHISTORY:
   case SCAN:  // Pass all of this off to the view
   case FINGER:
      View->MessageReceived(msg);
      break;
   case OPEN_TERM:
      // Lanch a terminl window.
      be_roster->Launch("application/x-vnd.Be-SHEL");
      break;
   case CRASH:
      (new BAlert(NULL,"I don't need a menu item for that - I can do it on my own :)","Thats true"))->Go();
   default:
      BWindow::MessageReceived(msg);
   }
}

/*******************************************************
*   Someone asked us nicely to quit. I gess we should
*   so clean up. save our setings (position of win)
*   and tell the main be_app to shut it all down .. bye
*******************************************************/
bool PSWindow::QuitRequested(){
   BPath path;
   int ref;

   if(find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
      path.Append("PortSpy_settings");
      ref = creat(path.Path(), 0777);
      if (ref >= 0) {
         BRect frm = Frame();
         write(ref, (char *)&frm, sizeof(frm));
         close(ref);
      }
   }
   be_app->Lock();
   be_app->Quit();  // do it this way. So WE can save prefs
   be_app->Unlock();
   //be_app->PostMessage(B_QUIT_REQUESTED);
   return true;
}
