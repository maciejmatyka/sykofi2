

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>


//#include <GL/GL.h>


#include "fx.h"
#include "fx3d.h"


#include "fox.h"

#include "icons.h"

#include "credits.h"

int ROTATEONOFF=1;
int SOURCESONOFF=1;
int FOGONOFF=1;
int BOXONOFF=0;
int FLATONOFF=1;
int LINE3DONOFF=0;
int GLVECTORS=0;





int VIEWMODE=1;



FXDEFMAP(WavesWindow) WavesWindowMap[]={

  //________Message_Type_________ID_____________________Message_Handler_______
  FXMAPFUNC(SEL_PAINT,         WavesWindow::ID_CANVAS,   WavesWindow::onExpose),
  FXMAPFUNC(SEL_CONFIGURE,     WavesWindow::ID_CANVAS,   WavesWindow::onConfigure),
 
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_SPIN,     WavesWindow::onCmdSpin),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_SPIN,     WavesWindow::onUpdSpin),
 
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_SPINFAST, WavesWindow::onCmdSpinFast),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_SPINFAST, WavesWindow::onUpdSpinFast),
 
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_STOP,     WavesWindow::onCmdStop),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_STOP,     WavesWindow::onUpdStop),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_ABOUT,     WavesWindow::onCmdAbout),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_ABOUT,     WavesWindow::onUpdAbout),

  FXMAPFUNC(SEL_TIMEOUT,       WavesWindow::ID_TIMEOUT,  WavesWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,         WavesWindow::ID_CHORE,    WavesWindow::onChore),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_RESET,     WavesWindow::onCmdReset),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_RESET,     WavesWindow::onUpdReset),


  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_CROTATE,     WavesWindow::onCmdRotate),
  FXMAPFUNC(SEL_UPDATE,        WavesWindow::ID_CROTATE,     WavesWindow::onUpdRotate),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_CSOURCES,     WavesWindow::onCmdSources),
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_VIEW_3D,     WavesWindow::onCmdView1),
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_VIEW_2DLINES,     WavesWindow::onCmdView2),
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_VIEW_2D,     WavesWindow::onCmdView3),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_FOG,     WavesWindow::onCmdFog),
  FXMAPFUNC(SEL_UPDATE,       WavesWindow::ID_FOG,     WavesWindow::onUpdFog),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_BOX,     WavesWindow::onCmdBox),
  FXMAPFUNC(SEL_UPDATE,       WavesWindow::ID_BOX,     WavesWindow::onUpdBox),

  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_GLVECTORS,     WavesWindow::onCmdVectors),
  FXMAPFUNC(SEL_UPDATE,       WavesWindow::ID_GLVECTORS,     WavesWindow::onUpdVectors),



  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_FLAT,     WavesWindow::onCmdFlat),
  FXMAPFUNC(SEL_COMMAND,       WavesWindow::ID_LINE3D,     WavesWindow::onCmdLine3D),





  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   WavesWindow::ID_CANVAS2, WavesWindow::onMouseRightDown),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, WavesWindow::ID_CANVAS2, WavesWindow::onMouseRightUp),

  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   WavesWindow::ID_CANVAS2, WavesWindow::onMouseDown),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE, WavesWindow::ID_CANVAS2, WavesWindow::onMouseUp),
  FXMAPFUNC(SEL_MOTION,            WavesWindow::ID_CANVAS2, WavesWindow::onMouseMove),

  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   WavesWindow::ID_CANVAS, WavesWindow::onMouseGRightDown),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, WavesWindow::ID_CANVAS, WavesWindow::onMouseGRightUp),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   WavesWindow::ID_CANVAS, WavesWindow::onMouseGDown),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE, WavesWindow::ID_CANVAS, WavesWindow::onMouseGUp),

  FXMAPFUNC(SEL_MOTION,            WavesWindow::ID_CANVAS, WavesWindow::onMouseGMove),

  FXMAPFUNC(SEL_COMMAND,     FXWindow::ID_QUERY_MENU,	  WavesWindow::onQueryMenu),

  FXMAPFUNC(SEL_CHANGED,     WavesWindow::ID_DIAL_ZOOM,	  WavesWindow::onMotionZoom),
  FXMAPFUNC(SEL_CHANGED,     WavesWindow::ID_DIAL_ROTY,	  WavesWindow::onMotionRotY),
  FXMAPFUNC(SEL_CHANGED,     WavesWindow::ID_DIAL_ROTZ,	  WavesWindow::onMotionRotZ),
  
  FXMAPFUNC(SEL_CHANGED,     WavesWindow::ID_DIAL_ROTX,	  WavesWindow::onMotionRotX),



 };







FXIMPLEMENT(WavesWindow,FXMainWindow,WavesWindowMap,ARRAYNUMBER(WavesWindowMap))


const FXuint TIMER_INTERVAL = 100;

WavesWindow::WavesWindow(FXApp* a):FXMainWindow(a,"Shroedinger 2.0",NULL,NULL,DECOR_ALL,0,0,495,370){
  FXVerticalFrame *glcanvasFrame;
  FXVerticalFrame *glcanvas2Frame;
//  FXVerticalFrame *glcanvas3Frame;
  FXVerticalFrame *buttonFrame;
  FXVerticalFrame *button2Frame;
  FXVerticalFrame *button3Frame;
//  FXVerticalFrame *button4Frame;
  FXVerticalFrame *podtabFrame;
  FXComposite *glpanel;
  FXHorizontalFrame *frame;
  FXGroupBox *group1;
  FXGroupBox *group0;

  FXGIFIcon *smallwavesicon;

  smallwavesicon=new FXGIFIcon(getApp(),wavesgif);
  setMiniIcon(smallwavesicon);  




  rotateCursor=getApp()->getDefaultCursor(DEF_ROTATE_CURSOR);
  zoomCursor=getApp()->getDefaultCursor(DEF_DRAGH_CURSOR);
  moveCursor=getApp()->getDefaultCursor(DEF_MOVE_CURSOR);
  normalCursor=getApp()->getDefaultCursor(DEF_ARROW_CURSOR);



// make icons

	fileopenicon=new FXGIFIcon(getApp(),fileopen);
	filesaveicon=new FXGIFIcon(getApp(),filesave);

// target



// MENU
/*
  menubar=new FXMenubar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  controlmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Control",NULL,controlmenu);

  new FXMenuCommand(controlmenu,"&Open...\tCtl-O\tOpen config file.",fileopenicon,this,ID_ABOUT);
  new FXMenuCommand(controlmenu,"&Save...\tCtl-S\tSave config file.",filesaveicon,this,ID_ABOUT);
  new FXMenuSeparator(controlmenu);  
  new FXMenuCommand(controlmenu,"&About...\tCtl-A\tAbout info.",filesaveicon,this,ID_ABOUT);
  new FXMenuCommand(controlmenu,"&Quit...\tCtl-Q\tExit now.",filesaveicon,getApp(),FXApp::ID_QUIT);

  viewmenu=new FXMenuPane(this);

  new FXMenuCommand(viewmenu,"&Model 3d\tCtl-M\tModel 3d.",NULL,this,ID_VIEW_3D,MENU_AUTOGRAY);
  new FXMenuCommand(viewmenu,"&Lines 2d\tCtl-L\tLines 2d, fast view mode.",NULL,this,ID_VIEW_2DLINES,MENU_AUTOGRAY);
  new FXMenuCommand(viewmenu,"S&imple 2d\tCtl-I\tSimple, fast 2d view mode.",NULL,this,ID_VIEW_2D,MENU_AUTOGRAY);
  new FXMenuSeparator(viewmenu);  
  new FXMenuCommand(viewmenu,"&Fog\tCtl-F\tSimple, fast 2d view mode.",NULL,this,ID_FOG);

  new FXMenuTitle(menubar,"&View",NULL,viewmenu);


  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Help",NULL,helpmenu);
  new FXMenuCommand(helpmenu,"&Help me...\tCtl-H\tHelp text. Show me.",NULL,this,ID_ABOUT);
*/







// ENDMENU


// statusbar

  statusbar=new FXStatusbar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);



  frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  


  //frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  
 glcanvasFrame=new FXVerticalFrame(frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);





 new FXLabel(glcanvasFrame,"Shroedinger 2.0 Output View.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
 new FXHorizontalSeparator(glcanvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);
 glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
 glcanvas=new FXGLCanvas(glpanel,glvisual,this,ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

 
 
podtabFrame=new FXVerticalFrame(frame,LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH,0,0,160,0,1,1,1,1);



 m_pFXTabBook1 = new FXTabBook(podtabFrame, 0, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y|TABBOOK_TOPTABS,0,0,0,0, 0,0,0,0);//248
 m_pFXTabItem1 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 4,4,69,23, 2,2,2,2);
 m_pFXTabItem1Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
 m_pFXTabItem1->setText("Control");
 m_pFXTabItem2 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 72,6,66,21, 2,2,2,2);
 m_pFXTabItem2Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
 m_pFXTabItem2->setText("View");
 m_pFXTabItem3 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 72,6,66,21, 2,2,2,2);
 m_pFXTabItem3Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
 m_pFXTabItem3->setText("Config");
// m_pFXTabItem4 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 72,6,66,21, 2,2,2,2);
// m_pFXTabItem4Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
// m_pFXTabItem4->setText("About");




  new FXTooltip(getApp());

 buttonFrame=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_Y,0,0,0,0,1,1,1,1);
 //button2Frame=new FXVerticalFrame(m_pFXTabItem2Frame,LAYOUT_FIX_HEIGHT|LAYOUT_FILL_X,0,0,0,180,1,1,1,1);
 button2Frame=new FXVerticalFrame(m_pFXTabItem2Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,1,1,1,1);




 group0=new FXGroupBox(buttonFrame,"General Options",LAYOUT_FIX_HEIGHT|FRAME_RIDGE,10,10,10,160);

// new FXLabel(group0,"Realize your dreams...",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
// new FXHorizontalSeparator(group0,SEPARATOR_RIDGE|LAYOUT_FILL_X);
 
B_CONTINUE = new FXButton(group0,"Continue.\t\tContinue simulation.",NULL,this,ID_SPINFAST,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
B_PAUSE = new FXButton(group0,"&Pause\t\tPause simulation.",NULL,this,ID_STOP,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
B_EXIT = new FXButton(group0,"&Quit\t\tExit program.",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);


 //new FXLabel(buttonFrame,"Select world properties.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
 new FXHorizontalSeparator(group0,SEPARATOR_RIDGE|LAYOUT_FILL_X);

B_ABOUT = new FXButton(group0,"&About\t\tAbout wave program.",NULL,this,ID_ABOUT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);


// new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);







  group1=new FXGroupBox(button2Frame,"View mode",LAYOUT_FIX_HEIGHT,10,10,10,320);
  R_3D=new FXRadioButton(group1,"3d model\t\t 3D model projection mode.",this,ID_VIEW_3D);
  C_FLAT=new FXRadioButton(group1,"Flat 3d \t\tEasiest way for 3D wave showing.",this,ID_FLAT);
  C_LINE3D=new FXRadioButton(group1,"Lines 3d \t\t\t\tVisual 3D model projection option.",this,ID_LINE3D);
  new FXRadioButton(group1,"2d lines\t\t 2D lines mode. Try to zoom it.",this,ID_VIEW_2DLINES);
  new FXRadioButton(group1,"Simple 2d \t\tEasiest way for 2D wave showing.",this,ID_VIEW_2D);


// C_LINE3D = new FXCheckButton(button2Frame,"Line 3D\t\tVisual 3D model projection option.",this,ID_LINE3D,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
// C_LINE3D->onUncheck(0,0,0);



// C_FLAT = new FXCheckButton(button2Frame,"Flat\t\tVisual 3D model projection option.",this,ID_FLAT,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
// C_FLAT->onUncheck(0,0,0);



  R_3D->onCheck(0,0,0);

new FXHorizontalSeparator(group1,SEPARATOR_GROOVE|LAYOUT_FILL_X);


 m_pFXDial1=new FXDial(group1,this,ID_DIAL_ROTX,DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|FRAME_RAISED|FRAME_THICK,0,0,60,0);
 m_pFXDial1->setTipText("Rotate X");
 m_pFXDial1->setRange(0,360);
 new FXLabel(group1,"Rotate X.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

 m_pFXDial2=new FXDial(group1,this,ID_DIAL_ROTY,DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|FRAME_RAISED|FRAME_THICK,0,0,60,0);
 m_pFXDial2->setTipText("Rotate Y");
 m_pFXDial2->setRange(0,360);
 new FXLabel(group1,"Rotate Y.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

 m_pFXDial3=new FXDial(group1,this,ID_DIAL_ROTZ,DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|FRAME_RAISED|FRAME_THICK,0,0,60,0);
 m_pFXDial3->setTipText("Rotate Z");
 m_pFXDial3->setRange(0,360);
 new FXLabel(group1,"Rotate Z.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

 m_pFXDial4=new FXDial(group1,this,ID_DIAL_ZOOM,DIAL_CYCLIC|DIAL_HORIZONTAL|LAYOUT_FIX_WIDTH|FRAME_RAISED|FRAME_THICK,0,0,60,0);
 m_pFXDial4->setTipText("Zoom me please...");
 m_pFXDial4->setRange(-50,660);
 new FXLabel(group1,"Zoom.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);






new FXHorizontalSeparator(button2Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 C_ROTATE = new FXCheckButton(button2Frame,"Rotate model\t\tSwitch rotating model mode (3d).",this,ID_CROTATE,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 C_ROTATE->onCheck(0,0,0);
 C_FOG = new FXCheckButton(button2Frame,"Fog\t\tVisual 3D model projection option.",this,ID_FOG,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 C_FOG->onUncheck(0,0,0);

 C_BOX = new FXCheckButton(button2Frame,"Box\t\tAdd 3d model box-room.",this,ID_BOX,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 C_BOX->onUncheck(0,0,0);

 C_GLVECTORS = new FXCheckButton(button2Frame,"Vectors\t\tShow Cartesian Orientation Vectors.",this,ID_GLVECTORS,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 C_GLVECTORS->onCheck(0,0,0);





 button3Frame=new FXVerticalFrame(m_pFXTabItem3Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,1,1,1,1);

 glcanvas2Frame=new FXVerticalFrame(button3Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,140,140,0,0,0,0);
 glvisual2=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
 glcanvas2=new FXGLCanvas(glcanvas2Frame,glvisual2,this,ID_CANVAS2,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);



//B_ADDSRC = new FXButton(button3Frame,"A&dd Source\t\tAdd source cells.",NULL,this,ID_RESET,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

B_RESET = new FXButton(button3Frame,"R&eset\t\tResed field.",NULL,this,ID_RESET,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
 C_SOURCES = new FXCheckButton(button3Frame,"Wave sources\t\t Switch wave sources on, off.",this,ID_CSOURCES,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 C_SOURCES->onCheck(0,0,0);



// button4Frame=new FXVerticalFrame(m_pFXTabItem4Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,1,1,1,1);
// glcanvas3Frame=new FXVerticalFrame(button4Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,50,50,0,0,0,0);
// glvisual3=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
// glcanvas3=new FXGLCanvas(glcanvas3Frame,glvisual3,this,ID_CANVAS3,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);


// new FXLabel(button4Frame,"Waves 1.0.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
// new FXLabel(button4Frame,"2d Wave Equation Solver",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
// new FXHorizontalSeparator(button4Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);


 angle=0;
 spinning=0;
 timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
 chore=getApp()->addChore(this,ID_CHORE);


 key=mousedown=0;
 startx=starty=endx=endy=0.0f;
 dial_zoom=1;


  }
    

WavesWindow::~WavesWindow(){
  if(timer) getApp()->removeTimeout(timer);

delete helpmenu;
delete viewmenu;
delete controlmenu;
  }

 

void WavesWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}




long WavesWindow::onConfigure(FXObject*,FXSelector,void*){
  if(glcanvas->makeCurrent()){
    glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());
    glcanvas->makeNonCurrent();
    }
  return 1;
  }



long WavesWindow::onExpose(FXObject*,FXSelector,void*){
  drawScene();
  return 1;
  }


long WavesWindow::onTimeout(FXObject*,FXSelector,void*){
//  angle += 2.;
//  if(angle > 360.) angle -= 360.;
  drawScene();
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }


long WavesWindow::onCmdSpin(FXObject*,FXSelector,void*){
  spinning=1;
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }

long WavesWindow::onUpdSpin(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }


long WavesWindow::onCmdSpinFast(FXObject*,FXSelector,void*){
  spinning=1;
  chore=getApp()->addChore(this,ID_CHORE);
  return 1;
  }


long WavesWindow::onUpdSpinFast(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }


long WavesWindow::onCmdStop(FXObject*,FXSelector,void*){
  spinning=0;
  if(timer){
    getApp()->removeTimeout(timer);
    timer=NULL;
    }
  if(chore){
    getApp()->removeChore(chore);
    chore=NULL;
    }
  return 1;
  }


long WavesWindow::onUpdStop(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->enable() : button->disable(); 
  return 1;
  }






long WavesWindow::onCmdRotate(FXObject* a,FXSelector b,void* c){
	(ROTATEONOFF) ? ROTATEONOFF = 0 : ROTATEONOFF = 1;

 (ROTATEONOFF) ? C_ROTATE->onCheck(0,0,0) : C_ROTATE->onUncheck(0,0,0);
										  
   return 1;
}

long WavesWindow::onUpdRotate(FXObject* a,FXSelector b,void* c){

 (ROTATEONOFF) ? C_ROTATE->onCheck(0,0,0) : C_ROTATE->onUncheck(0,0,0);

	if(VIEWMODE != 1) // no 3d model?
		C_ROTATE->disable();
	else
		C_ROTATE->enable();
		
										  
   return 1;
}



long WavesWindow::onCmdView1(FXObject* a,FXSelector b,void* c)
{
   VIEWMODE = 1;
	if(spinning==0)
	drawScene();
   return 1;
}

long WavesWindow::onCmdView2(FXObject* a,FXSelector b,void* c)
{
   VIEWMODE = 2;
	if(spinning==0)
	drawScene();
   return 1;
}

long WavesWindow::onCmdView3(FXObject* a,FXSelector b,void* c)
{
   VIEWMODE = 3;
	if(spinning==0)
	drawScene();
   return 1;
}


long WavesWindow::onCmdFog(FXObject* a,FXSelector b,void* c){
	(FOGONOFF) ? FOGONOFF = 0 : FOGONOFF = 1;

 (FOGONOFF) ? C_FOG->onCheck(0,0,0) : C_FOG->onUncheck(0,0,0);



	if(spinning==0)
	drawScene();
										  
   return 1;
}

long WavesWindow::onUpdFog(FXObject* a,FXSelector b,void* c){
	if(VIEWMODE != 1)
		C_FOG->disable();
	else
		C_FOG->enable();
										  
   return 1;
}


long WavesWindow::onCmdBox(FXObject* a,FXSelector b,void* c){
	(BOXONOFF) ? BOXONOFF = 0 : BOXONOFF = 1;

 (BOXONOFF) ? C_BOX->onCheck(0,0,0) : C_BOX->onUncheck(0,0,0);



	if(spinning==0)
	drawScene();
										  
   return 1;
}

long WavesWindow::onUpdBox(FXObject* a,FXSelector b,void* c){
	if(VIEWMODE != 1)
		C_BOX->disable();
	else
		C_BOX->enable();
										  
   return 1;
}



long WavesWindow::onCmdVectors(FXObject* a,FXSelector b,void* c){
	(GLVECTORS) ? GLVECTORS = 0 : GLVECTORS = 1;

 (GLVECTORS) ? C_GLVECTORS->onCheck(0,0,0) : C_GLVECTORS->onUncheck(0,0,0);



	if(spinning==0)
	drawScene();
										  
   return 1;
}

long WavesWindow::onUpdVectors(FXObject* a,FXSelector b,void* c){
	if(GLVECTORS != 1)
		C_GLVECTORS->disable();
	else
		C_GLVECTORS->enable();
										  
   return 1;
}




long WavesWindow::onCmdFlat(FXObject* a,FXSelector b,void* c){
	(FLATONOFF) ? FLATONOFF = 0 : FLATONOFF = 1;

 (FLATONOFF) ? C_FLAT->onCheck(0,0,0) : C_FLAT->onUncheck(0,0,0);

   VIEWMODE = 1; // 3d model


	if(spinning==0)
	drawScene();
										  
   return 1;
}

long WavesWindow::onCmdLine3D(FXObject* a,FXSelector b,void* c){
	(LINE3DONOFF) ? LINE3DONOFF = 0 : LINE3DONOFF = 1;

 (LINE3DONOFF) ? C_LINE3D->onCheck(0,0,0) : C_LINE3D->onUncheck(0,0,0);

   VIEWMODE = 1; // 3d model


	if(spinning==0)
	drawScene();
										  
   return 1;
}




long WavesWindow::onCmdAbout(FXObject*,FXSelector,void*){
const FXString& help="Waves v1.0\nGeneral information\n";

  	onCmdStop(0,0,0);


  AboutWindow aboutwindow(this,"About...");
  //aboutwindow.setAbout(help);
  //aboutwindow.setX(getX()+80);
  //aboutwindow.setY(getY()+80);
  aboutwindow.execute(PLACEMENT_DEFAULT);

  	onCmdSpin(this,0,0);
  	onCmdSpinFast(this,0,0);

  return 1;
  }

long WavesWindow::onUpdAbout(FXObject* sender,FXSelector,void*){
  

  return 1;
  }

long WavesWindow::onQueryMenu(FXObject* sender,FXSelector,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;
  FXMenuPane pane(this);

  new FXMenuCommand(&pane,"View 3D &Model\t\tSwitch to 3d projection.",NULL,this,ID_VIEW_3D);
  new FXMenuCommand(&pane,"View 2D &Lines\t\tSwitch to 2d lines projection.",NULL,this,ID_VIEW_2DLINES);
  new FXMenuCommand(&pane,"View S&imple 2D \t\tSwitch to simple 2d projection.",NULL,this,ID_VIEW_2D);
  new FXMenuSeparator(&pane);
  new FXMenuCommand(&pane,"&Fog\t\tSwitch FOG in 3D model.",NULL,this,ID_FOG);
  new FXMenuCommand(&pane,"&Flat\t\tSwitch FLAT in 3D model.",NULL,this,ID_FLAT);
  new FXMenuCommand(&pane,"&Line 3D\t\tSwitch Line 3D in 3D model.",NULL,this,ID_LINE3D);
  pane.create();
  pane.popup(NULL,event->root_x,event->root_y);
  getApp()->runModalWhileShown(&pane);
  return 1;
}


