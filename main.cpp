/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include "PortSpy.h"

// Application's signature
const char *APP_SIGNATURE = "application/x-vnd.YNOP-PortSpy";

/*******************************************************
*   Main.. What else do you want from me.
*******************************************************/
int main(int argc, char* argv[]){
   PortSpy *app = new PortSpy();
   
   app->Run();
   delete app;
   return B_NO_ERROR;
}