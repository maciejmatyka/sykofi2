

#include "fx.h"
#include "fx3d.h"

#include "FXAboutBox.h"




#define HORZ_PAD 30
#define VERT_PAD 2

#define MBOX_BUTTON_MASK   (MBOX_OK|MBOX_OK_CANCEL|MBOX_YES_NO|MBOX_YES_NO_CANCEL|MBOX_QUIT_CANCEL|MBOX_QUIT_SAVE_CANCEL)


FXDEFMAP(FXAboutBox) FXAboutBoxMap[]={
  FXMAPFUNC(SEL_COMMAND,FXAboutBox::ID_CANCEL,FXAboutBox::onCmdCancel),
  FXMAPFUNCS(SEL_COMMAND,FXAboutBox::ID_CLICKED_YES,FXAboutBox::ID_CLICKED_SAVE,FXAboutBox::onCmdClicked),
  };



// Object implementation
FXIMPLEMENT(FXAboutBox,FXDialogBox,FXAboutBoxMap,ARRAYNUMBER(FXAboutBoxMap))



// Create About box
FXAboutBox::FXAboutBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* ic,FXuint opts,FXint x,FXint y):
  FXDialogBox(owner,caption,opts|DECOR_TITLE|DECOR_BORDER,x,y,0,0, 0,0,0,0, 400,400){
  FXuint whichbuttons=opts&MBOX_BUTTON_MASK;
  FXVerticalFrame* content=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXHorizontalFrame* info=new FXHorizontalFrame(content,LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);


//  new FXLabel(info,NULL,ic,ICON_BEFORE_TEXT|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  new FXLabel(info,"Waves v1.0 ",NULL,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXHorizontalSeparator(content,SEPARATOR_GROOVE|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X);
  new FXLabel(info,"Wave equation solver.",NULL,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXHorizontalFrame* buttons=new FXHorizontalFrame(content,LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,10,10,10,10);

  new FXButton(buttons,"&OK",NULL,this,ID_CLICKED_OK,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_CENTER_X,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);



  }


long FXAboutBox::onCmdClicked(FXObject*,FXSelector sel,void*){
  getApp()->stopModal(this,MBOX_CLICKED_YES+(SELID(sel)-ID_CLICKED_YES));
  hide();
  return 1;
  }

long FXAboutBox::onCmdCancel(FXObject* sender,FXSelector sel,void* ptr){
  return FXAboutBox::onCmdClicked(sender,MKUINT(ID_CLICKED_CANCEL,SEL_COMMAND),ptr);
  }

FXuint FXAboutBox::information(FXWindow* owner,FXuint opts,const char* caption,const char* About,...){
  va_list arguments;
  va_start(arguments,About);
  FXAboutBox box(owner,caption,FXStringVFormat(About,arguments),0,opts|DECOR_TITLE|DECOR_BORDER);



  va_end(arguments);
  return box.execute();
  }

