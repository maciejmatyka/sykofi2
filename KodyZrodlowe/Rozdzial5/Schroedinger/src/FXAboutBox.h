
#ifndef FXAboutBOX_H
#define FXAboutBOX_H



/// About box
class FXAPI FXAboutBox : public FXDialogBox {
  FXDECLARE(FXAboutBox)
protected:
  FXAboutBox(){}
private:
  FXAboutBox(const FXAboutBox&);
  FXAboutBox &operator=(const FXAboutBox&);
public:
  long onCmdClicked(FXObject*,FXSelector,void*);
  long onCmdCancel(FXObject*,FXSelector,void*);
public:
  enum{
    ID_CLICKED_YES=FXDialogBox::ID_LAST,
    ID_CLICKED_NO,
    ID_CLICKED_OK,
    ID_CLICKED_CANCEL,
    ID_CLICKED_QUIT,
    ID_CLICKED_SAVE,
    ID_LAST
    };
public:

  /// Construct About box with given caption, icon, and About text
  FXAboutBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* ic=NULL,FXuint opts=0,FXint x=0,FXint y=0);

  /// Show a modal information dialog
  static FXuint information(FXWindow* owner,FXuint opts,const char* caption,const char* About,...) FX_PRINTF(4,5) ;

  };

  

#endif
