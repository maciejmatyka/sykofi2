
#ifndef AboutWINDOW_H
#define AboutWINDOW_H

// wziete z przykladow Fox'a
  
class FXText;


class FXAPI AboutWindow : public FXDialogBox {
  FXDECLARE(AboutWindow)
protected:

  FXVerticalFrame       *textCanvas;
  FXHorizontalSeparator *hSeparator;
  FXComposite           *textPanel;
  FXHorizontalFrame     *frame;
  FXText                *text;
  FXTimer               *timer;
  FXChore               *chore;
  FXFont                *normalFont;
  FXint                  numberDevelopers;
  FXint                  lastPosition;
  FXString               lineSpacer;
  FXint                  yIncrement;
  int					kolor,mouse;
  FXGLCanvas      *glcanvas;
  FXGLVisual      *glvisual;       
  FXVerticalFrame *glcanvasFrame;


  FXText *Abouttext;     
private:
  AboutWindow(){}
  AboutWindow(const AboutWindow&);
public:

  enum
  {
    ID_CANVAS=FXDialogBox::ID_LAST,
    ID_TIMEOUT,
    ID_CHORE,
    ID_LAST,
	ID_CANVASGL,
    };

	long onMouseMove(FXObject*,FXSelector,void*);
	long onMouseUp(FXObject*,FXSelector,void*);
	long onMouseDown(FXObject*,FXSelector,void*);

	long onExpose(FXObject*,FXSelector,void*);
	long onChore(FXObject*,FXSelector,void*);
	long drawScene(void);
 	long drawGLScene(void);
	long onTimeout(FXObject*,FXSelector,void*);

  AboutWindow(FXWindow *owner,const FXString& title);
  void setAbout(const FXString& About);
  FXString getAbout() const;
  virtual ~AboutWindow();
  };

#endif
