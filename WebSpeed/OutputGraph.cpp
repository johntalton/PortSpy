/*******************************************************
*   PortSpy©
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    October 19 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include <stdio.h>

#include "Globals.h"
#include "OutputGraph.h"

/*******************************************************
*
*******************************************************/
OutputGraph::OutputGraph(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW){
   //SetViewColor(230,230,230);
   SetViewColor(B_TRANSPARENT_32_BIT);
   list = NULL;
   count = -1;
   max = 0;
}

/*******************************************************
*
*******************************************************/
OutputGraph::~OutputGraph(){
   if(list != NULL){
      delete []list;
   }
}

/*******************************************************
*
*******************************************************/
void OutputGraph::SetCount(int32 c){
   count = c;
   max = 0;
   if(list != NULL){
      delete []list;
   }
   list = new float[count];
   for(int32 i = 0;i < count; i++){
      list[i] = 0;
   }
   //printf("Set count to %i\n",count);
   Invalidate();
}

/*******************************************************
*
*******************************************************/
void OutputGraph::SetPoint(int32 p,float value){
   //printf("Setting %i to %f\n",p,value);
   list[p] = value;
   if(value > max){
      max = value;
   }
   Invalidate();
}

/*******************************************************
*
*******************************************************/
void OutputGraph::Draw(BRect){
   BRect b = Bounds();
   
   
   SetHighColor(230,230,230);
   FillRect(BRect(b.left+2,b.top+2,b.right-2,b.bottom-2));
   
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT));
   StrokeLine(BPoint(b.left,b.top),BPoint(b.right,b.top));
   StrokeLine(BPoint(b.left,b.top),BPoint(b.left,b.bottom));
   
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_LIGHTEN_MAX_TINT));
   StrokeLine(BPoint(b.right,b.top+1),BPoint(b.right,b.bottom));
   StrokeLine(BPoint(b.right,b.bottom),BPoint(b.left+1,b.bottom));
   
   SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   StrokeRect(BRect(b.left+1,b.top+1,b.right-1,b.bottom-1));
   
   b.InsetBy(2,2);
   
   /*SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_2_TINT));
   SetHighColor(50,200,50);
   DrawString("All good",BPoint(20,16));
   SetHighColor(255,0,0);
   DrawString("Error",BPoint(20,16+16));
   SetHighColor(0,0,255);
   DrawString("Other",BPoint(20,16+16+16));
   */
   
   
   
   float jump = (b.Width() / count);
   float xval = 0;
   int32 scale = ((b.IntegerHeight()-50) / max);
   
   if(list != NULL){
      if(list[0] == -1){
         SetHighColor(255,0,0);
      }else{
         SetHighColor(0,0,255);
      }
      //printf("found list and count is %i\n",count);
      StrokeLine(BPoint(b.left,b.bottom),BPoint(xval+jump,b.bottom-(list[0]*scale)));
      xval = xval + jump;
      for(int32 i = 1; i < count; i++){
         
      
         if(list[i] == -1){
            if(list[i-1] == -1){
               SetHighColor(255,0,0);
               StrokeLine(BPoint(xval,b.bottom-(100)),BPoint(xval+jump,b.bottom-(100)));
               FillEllipse(BPoint(xval,b.bottom-100),1,1);
            }else{
               SetHighColor(0,0,255);
               StrokeLine(BPoint(xval,b.bottom-(list[i-1]*scale)),BPoint(xval+jump,b.bottom-(100)));
               FillEllipse(BPoint(xval,b.bottom-(list[i-1]*scale)),1,1);
            }
         }else{
            SetHighColor(50,200,50);
            StrokeLine(BPoint(xval,b.bottom-(list[i-1]*scale)),BPoint(xval+jump,b.bottom-(list[i]*scale)));
            FillEllipse(BPoint(xval,b.bottom-(list[i-1]*scale)),1,1);
         }
         xval = xval + jump;
         //printf("Strokeing at %f %f->%f\n",xval,list[i-1],list[i]);
      }
      //printf("Xval = %f, Jump is %f, Width is %f\n",xval,jump,b.Width());
      BString maxS("");
      maxS << (float)(max);
      maxS.Append("tps");
      SetHighColor(0,0,0);
      DrawString(maxS.String(),BPoint(10,b.bottom-(max*scale)));
   }else{
      //printf("Null list\n");
      
      
   }
   
   
}

/*******************************************************
*
*******************************************************/
void OutputGraph::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BView::MessageReceived(msg);
   }
}



