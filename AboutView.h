#ifndef _ABOUT_VIEW_H
#define _ABOUT_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>


class AboutView : public BView {
   public:
      AboutView(BRect);
      ~AboutView();
      virtual void Draw(BRect);
		virtual void MessageReceived(BMessage*);
   private:
      BBitmap *img;

};
#endif