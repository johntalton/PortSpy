#ifndef OUTPUT_GRAPH_VIEW_H
#define OUTPUT_GRAPH_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class OutputGraph : public BView {
   public:
      OutputGraph(BRect);
      ~OutputGraph();
      void SetCount(int32 c);
      void SetPoint(int32 p,float value);
      virtual void Draw(BRect);
      virtual void MessageReceived(BMessage*);
   private:
      float *list;
      int32 count;
      float max;

};
#endif