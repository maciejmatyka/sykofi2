

class WavesWindow : public FXMainWindow {
  FXDECLARE(WavesWindow)

private:

  FXGLCanvas      *glcanvas;
  FXGLCanvas     *glcanvas2;
  FXGLCanvas     *glcanvas3;
  FXTimer         *timer;   
  FXChore         *chore;   
  int              spinning;       
  double           angle;          
  FXGLVisual      *glvisual;       
  FXGLVisual      *glvisual2;       
  FXGLVisual      *glvisual3;       


  
protected:
  WavesWindow(){}
  FXTextField* poptextx;
  FXTextField* poptexty;
  FXTextField* poptextvx;
  FXTextField* poptextvy;
  FXTextField* gravitytext;

  FXCheckButton* C_ROTATE;
  
  FXCheckButton* C_SOURCES;
  FXCheckButton* C_FOG;
  FXCheckButton* C_BOX;
  FXCheckButton* C_GLVECTORS;




  FXRadioButton* C_FLAT;
  FXRadioButton* C_LINE3D;





  FXTabBook* m_pFXTabBook1;
  FXTabItem* m_pFXTabItem1;
  FXHorizontalFrame* m_pFXTabItem1Frame;
  FXTabItem* m_pFXTabItem2;
  FXHorizontalFrame* m_pFXTabItem2Frame;
  FXTabItem* m_pFXTabItem3;
  FXHorizontalFrame* m_pFXTabItem3Frame;
  FXTabItem* m_pFXTabItem4;
  FXHorizontalFrame* m_pFXTabItem4Frame;



  FXButton* B_PAUSE;
  FXButton* B_RESET;
  FXButton* B_CONTINUE;
  FXButton* B_EXIT;

  FXButton* B_ADDSRC;


  FXButton* B_ABOUT;

  FXRadioButton* R_3D;

  FXMenubar *menubar;              


  FXMenuPane        *controlmenu;           
  FXMenuPane        *viewmenu;           
  FXMenuPane        *helpmenu;         


  FXIcon            *fileopenicon;    
  FXIcon            *filesaveicon;  

  FXStatusbar       *statusbar;

  FXDial *m_pFXDial1;
  FXDial *m_pFXDial2;
  FXDial *m_pFXDial3;

  FXDial *m_pFXDial4; // zoom


  FXCursor* rotateCursor;
  FXCursor* normalCursor;
  FXCursor* zoomCursor;
  FXCursor* moveCursor;





int key;
int dial_zoom;
int mousedown;
float startx,starty,endx,endy;



public:

 enum{
    ID_CANVAS=FXMainWindow::ID_LAST,
    ID_CANVAS2,
    ID_CANVAS3,
    ID_SPIN,
    ID_SPINFAST,
    ID_STOP,
    ID_ABOUT,
    ID_TIMEOUT,
    ID_CHORE,
	ID_RESET,
	ID_CROTATE,
	ID_CSOURCES,
	ID_VIEW_2DLINES,
	ID_VIEW_2D,
	ID_VIEW_3D,
	ID_FOG,
	ID_BOX,
	ID_FLAT,
	ID_LINE3D,
	ID_QUERY_MENU,
	ID_DIAL_ZOOM,
	ID_DIAL_ROTX,
	ID_DIAL_ROTY,
	ID_DIAL_ROTZ,
	ID_GLVECTORS,
	ID_ADDSRC,
  	};
    
  long onExpose(FXObject*,FXSelector,void*);
  long onConfigure(FXObject*,FXSelector,void*);
  long onCmdSpin(FXObject*,FXSelector,void*);
  long onUpdSpin(FXObject*,FXSelector,void*);
  long onCmdStop(FXObject*,FXSelector,void*);
  long onUpdStop(FXObject*,FXSelector,void*);
  long onTimeout(FXObject*,FXSelector,void*);
  long onChore(FXObject*,FXSelector,void*);
  long onCmdSpinFast(FXObject*,FXSelector,void*);
  long onUpdSpinFast(FXObject*,FXSelector,void*);
  long onCmdReset(FXObject*,FXSelector,void*);
  long onUpdReset(FXObject*,FXSelector,void*);
  long onCmdRotate(FXObject*,FXSelector,void*);
  long onUpdRotate(FXObject*,FXSelector,void*);
  long onCmdSources(FXObject*,FXSelector,void*);
  long onCmdFog(FXObject*,FXSelector,void*);
  long onCmdBox(FXObject*,FXSelector,void*);
  long onUpdBox(FXObject*,FXSelector,void*);
  long onCmdVectors(FXObject*,FXSelector,void*);
  long onUpdVectors(FXObject*,FXSelector,void*);
  long onUpdFog(FXObject*,FXSelector,void*);
  long onCmdFlat(FXObject*,FXSelector,void*);
  long onCmdLine3D(FXObject*,FXSelector,void*);



  long onCmdAbout(FXObject*,FXSelector,void*);
  long onUpdAbout(FXObject*,FXSelector,void*);
 
  long onCmdView1(FXObject*,FXSelector,void*);
  long onCmdView2(FXObject*,FXSelector,void*);
  long onCmdView3(FXObject*,FXSelector,void*);

  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
  long onMouseMove(FXObject*,FXSelector,void*);

  long onMouseGDown(FXObject*,FXSelector,void*);
  long onMouseGUp(FXObject*,FXSelector,void*);
  long onMouseGMove(FXObject*,FXSelector,void*);

  long onMouseGRightDown(FXObject*,FXSelector,void*);
  long onMouseGRightUp(FXObject*,FXSelector,void*);

  long onMouseRightDown(FXObject*,FXSelector,void*);
  long onMouseRightUp(FXObject*,FXSelector,void*);


  long onQueryMenu(FXObject*,FXSelector,void*);

  long onMotionZoom(FXObject*,FXSelector,void*);
  
  long onMotionRotX(FXObject*,FXSelector,void*);
   
  long onMotionRotY(FXObject*,FXSelector,void*);
  long onMotionRotZ(FXObject*,FXSelector,void*);



//public:

 WavesWindow(FXApp* a);
  
 void create();

  
 void drawScene();
  
 virtual ~WavesWindow();
  };
