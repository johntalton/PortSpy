/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <Alert.h>
#include <Application.h>

#include <unistd.h>

#include "Globals.h"
#include "PortSpy.h"
#include "PSWindow.h"

/*******************************************************
*   Standerd stuff here .. grap the prefs and make a 
*   default window if no other options
*******************************************************/
PortSpy::PortSpy() : BApplication(APP_SIGNATURE){
   BRect wind_pos;
   BPath path;
   int ref;
   BRect defaultSize(50,50,550,450);

   if(find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
      path.Append("PortSpy_settings");
      ref = open(path.Path(),O_RDONLY);
      if(ref >= 0){
         read(ref, (char *)&wind_pos, sizeof(wind_pos));
         close(ref);
         if(!wind_pos.Intersects(BScreen().Frame())){
            theWin = new PSWindow(defaultSize);
         }else{
            theWin = new PSWindow(wind_pos);
         }
      }else{ // hmm that was not in there
         theWin = new PSWindow(defaultSize);
      }
   }else{ // gess there are no defaults ..first time?
      theWin = new PSWindow(defaultSize);
   }
}

/*******************************************************
*
*******************************************************/
PortSpy::~PortSpy(){
}

/*******************************************************
*   Our lovely about box with hidden box
*******************************************************/
void PortSpy::AboutRequested(){
   uint32 mask;
   mask = modifiers();
   if(B_SHIFT_KEY & mask){
      (new BAlert("About PortSpy",
      "PortSpy ©\n\nIf you spent the time to find this:\n\tA. You need a hobbie\n\tB. You're a hacker, code cracker, slacker.\n\tC. You really want to register this app!",
      " A "," B ","  C  ",B_WIDTH_AS_USUAL,B_IDEA_ALERT))->Go();
   }else{
      (new BAlert("About PortSpy","PortSpy ©1999\n\nTheAbstractCompany\nhttp://www.latech.edu/~jta001/BeOS\nynop@acm.org\n\nVersion: 1.0 Beta 2.x","Thats Nice"))->Go();
   }
}

/*******************************************************
*   Ah .. do nothing .. just defalut pass off.
*******************************************************/
void PortSpy::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BApplication::MessageReceived(msg);
      break; 
   }  
}

/*******************************************************
*   Ask the window to quit (he saves prefs) and then quit
*******************************************************/
bool PortSpy::QuitRequested(){
   if(theWin->QuitRequested()){
      return true; // Ok .. fine .. leave then
   }
   return false;
}






