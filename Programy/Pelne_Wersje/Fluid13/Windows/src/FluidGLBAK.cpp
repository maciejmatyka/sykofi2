
#include <string.h>
#include <math.h>
#include <gl/glaux.h>
#include <stdio.h>
#include <stdlib.h>

#include "fx.h"
#include "fx3d.h"

  

//#define demoversion
//#define demoversionDT
//#define demoversionFRAME

//#define demoversion

int	CELLSONOFF=0;
int	VELOCITYONOFF=0;
int	PARTICLESONOFF=1;
int	PRESSUREONOFF=1;


#include "Vfw.h"
#pragma comment( lib, "Vfw32.lib" )



int fframe = 0;


extern "C"
{

#include "misc.h"
#include "bndflags.h"
#include "parse.h"
#include "dimalloc.h"
#include "bndinit.h"
#include "view.h"
#include "particle.h"
#include "visual.h"
#include "tscells.h"
#include "pressure.h"
#include "velocity.h"
#include "minmax.h"
#include "fluid.h"

}


#define CI_RED AUX_RED 
#define CI_ANTI_ALIAS_GREEN 16 
#define CI_ANTI_ALIAS_YELLOW 32 
#define CI_ANTI_ALIAS_RED 48  



// global datas



char blah[100];
float point[3] = { 0.0, 0.0, 0.0 };

float point2[3] = { 0.0, 0.0, 0.0 };

int write = 0;	 // write frames to disk
int dzialaj=1;
long how_much_particles=0;

GLint size; 
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };	
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLenum rgb, doubleBuffer, windType; 
GLenum mode; 



int loaded = 0;


// c functions
void glinit(void);

// Timer setting (in milliseconds)
const FXuint TIMER_INTERVAL = 100;


// Event Handler Object
class GLTestWindow : public FXMainWindow {
  FXDECLARE(GLTestWindow)

private:

  FXGLCanvas      *glcanvas;                  // GL Canvas to draw into
  FXTimer         *timer;                     // Timer for spinning box
  FXChore         *chore;                     // Timer for spinning box
  int              spinning;                  // Is box spinning
  double           angle;                     // Rotation angle of box
  FXGLVisual      *glvisual;                  // OpenGL visual
  
protected:
  GLTestWindow(){}
//  FXTextField*		framenr;
  FXLabel*		framenr;


  FXLabel*			whatimdoing;

  FXTextField*		T_WIDTH;
  FXTextField*		T_HEIGHT;
  FXTextField*		T_NX;
  FXTextField*		T_NY;
  FXTextField*		T_DT;
  FXTextField*		T_STA;
  FXTextField*		T_GX;
  FXTextField*		T_GY;
  FXTextField*		T_VIS;


  FXLabel*		L_WIDTH;
  FXLabel*		L_HEIGHT;
  FXLabel*		L_NX;
  FXLabel*		L_NY;
  FXLabel*		L_DT;
  FXLabel*		L_STA;
  FXLabel*		L_GX;
  FXLabel*		L_GY;
  FXLabel*		L_VIS;
  FXLabel*		L_PARTICLES;

  FXLabel*      L_Render;
  
  FXLabel*		infoglcanvas;

  FXList* configlist;


//  FXProgressBar*	pbar2;
  FXProgressBar*	pbar;
  FXText*			text;

  FXCheckButton* B_PRESSUREONOFF;
  FXCheckButton* B_CELLSONOFF;
  FXCheckButton* B_VELOCITYONOFF;
  FXCheckButton* B_PARTICLESONOFF;

	FXTabBook* m_pFXTabBook1;
	FXTabItem* m_pFXTabItem1;
	FXHorizontalFrame* m_pFXTabItem1Frame;
	FXTabItem* m_pFXTabItem2;
	FXHorizontalFrame* m_pFXTabItem2Frame;
	FXTabItem* m_pFXTabItem3;
	FXHorizontalFrame* m_pFXTabItem3Frame;

	FXButton* B_START;
	FXButton* B_STOP;
	FXCheckButton* B_WRITEONOFF;




public:

  // We define additional ID's, starting from the last one used by the base class+1.
  // This way, we know the ID's are all unique for this particular target.
  enum{
    ID_CANVAS=FXMainWindow::ID_LAST,
    ID_SPIN,
    ID_SPINFAST,
    ID_STOP,
    ID_TIMEOUT,
    ID_CHORE,
	ID_WRITE,
	ID_LOAD,
	ID_QUIT,
	ID_ABOUT,
	ID_DEMO,

	ID_WIDTH,
	ID_HEIGHT,
	ID_NX,
	ID_NY,
	ID_DT,
	ID_STA,
	ID_GX,
	ID_GY,
	ID_VIS,

	ID_PRESSUREONOFF,
	ID_CELLSONOFF,
	ID_VELOCITYONOFF,
	ID_PARTICLESONOFF,

	ID_CONFIG,

  	};

    
  // Message handlers
  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
  long onMouseMove(FXObject*,FXSelector,void*);
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
  long onUpdWrite(FXObject*,FXSelector,void*);
  long onCmdWrite(FXObject*,FXSelector,void*);
  long onUpdLoad(FXObject*,FXSelector,void*);
  long onCmdLoad(FXObject*,FXSelector,void*);
  long onUpdQuit(FXObject*,FXSelector,void*);
  long onCmdQuit(FXObject*,FXSelector,void*);
  long onUpdAbout(FXObject*,FXSelector,void*);

  long onCmdAbout(FXObject*,FXSelector,void*);
  long onCmdDemo(FXObject*,FXSelector,void*);
  long onCmdDemo2(FXObject*,FXSelector,void*);
  long onCmdModified(FXObject*,FXSelector,void*);

  
  long onCmdWIDTH(FXObject*,FXSelector,void*);
  long onCmdHEIGHT(FXObject*,FXSelector,void*);
  long onCmdNX(FXObject*,FXSelector,void*);
  long onCmdNY(FXObject*,FXSelector,void*);
  long onCmdDT(FXObject*,FXSelector,void*);
  long onCmdSTA(FXObject*,FXSelector,void*);
  long onCmdGX(FXObject*,FXSelector,void*);
  long onCmdGY(FXObject*,FXSelector,void*);
  long onCmdVIS(FXObject*,FXSelector,void*);

  long onCmdPRESSUREONOFF(FXObject*,FXSelector,void*);
  long onCmdCELLSONOFF(FXObject*,FXSelector,void*);
  long onCmdVELOCITYONOFF(FXObject*,FXSelector,void*);
  long onCmdPARTICLESONOFF(FXObject*,FXSelector,void*);
    
  long onCmdConfig(FXObject*,FXSelector,void*);
  long onUpdConfig(FXObject*,FXSelector,void*);



  int timestep(double dx, double dy, double gx, double gy, double vis,
	     double t, double dt, int nx, int ny, int imax, int jmax,
	     int width, int height, int **Flag, double **Un1, double **Vn1,
	     double **U, double **V, double **P, double **D, double **R, double sta);

	void Render(void);







 


public:

  // GLTestWindow constructor
  GLTestWindow(FXApp* a);
  
  // Initialize
  void create();

  
  // Draw scene
  void drawScene();
  
  // GLTestWindow destructor
  virtual ~GLTestWindow();



  };



// Message Map for the Scribble App class
FXDEFMAP(GLTestWindow) GLTestWindowMap[]={

  //________Message_Type_________ID_____________________Message_Handler_______
  FXMAPFUNC(SEL_PAINT,         GLTestWindow::ID_CANVAS,   GLTestWindow::onExpose),
  FXMAPFUNC(SEL_CONFIGURE,     GLTestWindow::ID_CANVAS,   GLTestWindow::onConfigure),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPIN,     GLTestWindow::onCmdSpin),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_SPIN,     GLTestWindow::onUpdSpin),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPINFAST, GLTestWindow::onCmdSpinFast),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_SPINFAST, GLTestWindow::onUpdSpinFast),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_STOP,     GLTestWindow::onCmdStop),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_STOP,     GLTestWindow::onUpdStop),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_WRITE,     GLTestWindow::onCmdWrite),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_WRITE,     GLTestWindow::onUpdWrite),

  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_LOAD,     GLTestWindow::onUpdLoad),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_QUIT,     GLTestWindow::onCmdQuit),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_QUIT,     GLTestWindow::onUpdQuit),


  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_ABOUT,     GLTestWindow::onCmdAbout),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_ABOUT,     GLTestWindow::onUpdAbout),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_DEMO,     GLTestWindow::onCmdDemo),


#ifdef demoversion
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_LOAD,     GLTestWindow::onCmdDemo),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_WIDTH,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_HEIGHT,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_NX,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_NY,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_DT,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_STA,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GX,     GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GY,       GLTestWindow::onCmdDemo),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_VIS,      GLTestWindow::onCmdDemo),
#else
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_LOAD,     GLTestWindow::onCmdLoad),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_WIDTH,     GLTestWindow::onCmdWIDTH),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_HEIGHT,     GLTestWindow::onCmdHEIGHT),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_NX,     GLTestWindow::onCmdNX),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_NY,     GLTestWindow::onCmdNY),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_DT,     GLTestWindow::onCmdDT),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_STA,     GLTestWindow::onCmdSTA),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GX,     GLTestWindow::onCmdGX),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GY,       GLTestWindow::onCmdGY),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_VIS,      GLTestWindow::onCmdVIS),
#endif

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_PRESSUREONOFF,     GLTestWindow::onCmdPRESSUREONOFF),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_CELLSONOFF,     GLTestWindow::onCmdCELLSONOFF),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_VELOCITYONOFF,       GLTestWindow::onCmdVELOCITYONOFF),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_PARTICLESONOFF,      GLTestWindow::onCmdPARTICLESONOFF),


  FXMAPFUNC(SEL_TIMEOUT,       GLTestWindow::ID_TIMEOUT,  GLTestWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,         GLTestWindow::ID_CHORE,    GLTestWindow::onChore),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_CONFIG, GLTestWindow::onCmdConfig),
  FXMAPFUNC(SEL_UPDATE,        GLTestWindow::ID_CONFIG, GLTestWindow::onUpdConfig),
 


  };



// Macro for the GLTestApp class hierarchy implementation
FXIMPLEMENT(GLTestWindow,FXMainWindow,GLTestWindowMap,ARRAYNUMBER(GLTestWindowMap))




// Construct a GLTestApp
GLTestWindow::GLTestWindow(FXApp* a):FXMainWindow(a,"Fluid version 1.2 - 2D Navier-Stokes based, incompressible, viscous, Free Surface fluid flow solver.",NULL,NULL,DECOR_ALL,0,0,680,370){
  FXVerticalFrame *glcanvasFrame;
  FXVerticalFrame *buttonFrame;
  FXVerticalFrame *buttonFrame2;
  FXVerticalFrame *buttonFrameRight;
  FXVerticalFrame *buttonFrameRight2;
  FXComposite *glpanel;
  FXHorizontalFrame *frame;
	 int argc = 2;
	 char *argv[3];


  

	m_pFXTabBook1 = new FXTabBook(this, 0, 0, LAYOUT_RIGHT|LAYOUT_CENTER_X|LAYOUT_BOTTOM|LAYOUT_CENTER_Y|LAYOUT_FIX_X|LAYOUT_FIX_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|TABBOOK_TOPTABS,0,0,240,370, 0,0,0,0);
  frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 240,0,0,0);

	m_pFXTabItem1 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 4,4,69,23, 2,2,2,2);
	m_pFXTabItem1Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem1->setText("Output");
	m_pFXTabItem2 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 72,6,66,21, 2,2,2,2);
	m_pFXTabItem2Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem2->setText("Configuration of Fluid");

  
  // RIGHT pane for the buttons						 //FRAME_RIDGE|LAYOUT_FILL_Y
//  buttonFrame=new FXVerticalFrame(frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);

//  buttonFrame=new FXVerticalFrame(m_pFXTabItem1Frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  buttonFrame=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);




  //	new FXButton(buttonFrame,"&Load Configuration.",NULL,this,ID_LOAD,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);

 B_WRITEONOFF = new FXCheckButton(buttonFrame,"    Write frames.",this,ID_WRITE,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);


	B_START = new FXButton(buttonFrame,"&Start\tStart rendering.",NULL,this,ID_SPIN,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);
	B_STOP = new FXButton(buttonFrame,"&Stop\tStop rendering.",NULL,this,ID_STOP,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);

    B_START->addHotKey(MKUINT('s',0));



//  new FXLabel(buttonFrame,"Frame:  ",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

 // framenr=new FXTextField(buttonFrame,10,NULL,0,FRAME_SUNKEN|FRAME_THICK|JUSTIFY_RIGHT|LAYOUT_FILL_X, 0,0,0,0, 14,14,1,1);
//  framenr=new FXTextField(buttonFrame,7,NULL,0,JUSTIFY_RIGHT|LAYOUT_FILL_X, 0,0,0,0, 14,14,1,1);

//   pbar2 = new FXProgressBar(buttonFrame,NULL,0,LAYOUT_FILL_X|PROGRESSBAR_PERCENTAGE);
 
 
//	new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);
 
//	new FXButton(buttonFrame,"&Exit\tExit program.",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);
	new FXButton(buttonFrame,"&Exit\tExit program.",NULL,this,ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);
	new FXButton(buttonFrame,"&About\tAbout program.",NULL,this,ID_ABOUT,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);


  buttonFrame2=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);

 new FXLabel(buttonFrame2,"Render View",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXHorizontalSeparator(buttonFrame2,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 B_CELLSONOFF = new FXCheckButton(buttonFrame2,"Cells",this,ID_CELLSONOFF,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 B_VELOCITYONOFF = new FXCheckButton(buttonFrame2,"Velocity",this,ID_VELOCITYONOFF,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 B_PARTICLESONOFF = new FXCheckButton(buttonFrame2,"Particles",this,ID_PARTICLESONOFF,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 B_PRESSUREONOFF = new FXCheckButton(buttonFrame2,"Pressure Field",this,ID_PRESSUREONOFF,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 
 PRESSUREONOFF ? B_PRESSUREONOFF->onCheck(0,0,0) : B_PRESSUREONOFF->onUncheck(0,0,0);
 CELLSONOFF ? B_CELLSONOFF->onCheck(0,0,0) : B_CELLSONOFF->onUncheck(0,0,0);
 VELOCITYONOFF ? B_VELOCITYONOFF->onCheck(0,0,0) : B_VELOCITYONOFF->onUncheck(0,0,0);
 PARTICLESONOFF ? B_PARTICLESONOFF->onCheck(0,0,0) : B_PARTICLESONOFF->onUncheck(0,0,0);

 B_PRESSUREONOFF->disable();
 B_CELLSONOFF->disable();
 B_VELOCITYONOFF->disable();
 B_PARTICLESONOFF->disable();

  new FXButton(buttonFrame2,"&Load",NULL,this,ID_LOAD,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);




  // LEFT pane to contain the glcanvas


  glcanvasFrame=new FXVerticalFrame(frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
//  glcanvasFrame=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
   
  infoglcanvas = new FXLabel(glcanvasFrame,"Fluid v1.0 Output View.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

  new FXHorizontalSeparator(glcanvasFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  
  // Drawing glcanvas
//  glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_RIDGE|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);
  glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);



//  whatimdoing=new FXTextField(glcanvasFrame,10,NULL,0,FRAME_SUNKEN|FRAME_THICK|JUSTIFY_RIGHT|LAYOUT_FILL_X, 0,0,0,0, 14,14,1,1);
//  whatimdoing=new FXLabel(glcanvasFrame,"Load or modify fluid configuration.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
   
  // A Visual to drag OpenGL

  glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
   
  // Drawing glcanvas
  glcanvas=new FXGLCanvas(glpanel,glvisual,this,ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);
 L_PARTICLES=new FXLabel(glpanel,"Particles: 0",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

 framenr=new FXLabel(glpanel,"Please Load Config.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 pbar = new FXProgressBar(glpanel,NULL,0,LAYOUT_FILL_X|PROGRESSBAR_PERCENTAGE);

// framenr=new FXLabel(buttonFrame,"Please Load Config.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
// pbar = new FXProgressBar(buttonFrame,NULL,0,LAYOUT_FILL_X|PROGRESSBAR_PERCENTAGE);


 //

// buttonFrameRight2=new FXVerticalFrame(frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,0,10,10,10);
// buttonFrameRight2=new FXVerticalFrame(m_pFXTabItem2Frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,0,10,10,10);
 buttonFrameRight2=new FXVerticalFrame(m_pFXTabItem2Frame,LAYOUT_FILL_Y,0,0,0,0,0,10,10,10);




	new FXHorizontalSeparator(buttonFrameRight2,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 L_WIDTH=new FXLabel(buttonFrameRight2,"Width",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_HEIGHT=new FXLabel(buttonFrameRight2,"Height",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_NX=new FXLabel(buttonFrameRight2,"NX Cells",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_NY=new FXLabel(buttonFrameRight2,"NY Cells",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

	new FXHorizontalSeparator(buttonFrameRight2,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 L_GX=new FXLabel(buttonFrameRight2,"Force X",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_GY=new FXLabel(buttonFrameRight2,"Force Y (Gravity)",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);

	new FXHorizontalSeparator(buttonFrameRight2,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 
 L_STA=new FXLabel(buttonFrameRight2,"Stability Condition",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_DT=new FXLabel(buttonFrameRight2,"DT Increment",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 L_VIS=new FXLabel(buttonFrameRight2,"Viscosity of Fluid",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);


	new FXHorizontalSeparator(buttonFrameRight2,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 
 configlist=new FXList(buttonFrameRight2,6,this,ID_CONFIG,FRAME_RAISED|LIST_SINGLESELECT|LAYOUT_FILL_X);
  configlist->appendItem("Splash");
  configlist->appendItem("Complicated");
  configlist->appendItem("Rownaj");
  configlist->appendItem("Sluza");
  
  configlist->selectItem(1);

//  buttonFrameRight=new FXVerticalFrame(frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
//  buttonFrameRight=new FXVerticalFrame(m_pFXTabItem2Frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  buttonFrameRight=new FXVerticalFrame(m_pFXTabItem2Frame,LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);


	new FXHorizontalSeparator(buttonFrameRight,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 T_WIDTH=new FXTextField(buttonFrameRight,5,this,ID_WIDTH,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 T_HEIGHT=new FXTextField(buttonFrameRight,5,this,ID_HEIGHT,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0, 4,9,0,0);
 T_NX=new FXTextField(buttonFrameRight,5,this,ID_NX,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 T_NY=new FXTextField(buttonFrameRight,5,this,ID_NY,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0, 4,9,0,0);

	new FXHorizontalSeparator(buttonFrameRight,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 T_GX=new FXTextField(buttonFrameRight,5,this,ID_GX,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 T_GY=new FXTextField(buttonFrameRight,5,this,ID_GY,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0, 4,9,0,0);

	new FXHorizontalSeparator(buttonFrameRight,SEPARATOR_GROOVE|LAYOUT_FILL_X);

 
 T_STA=new FXTextField(buttonFrameRight,5,this,ID_STA,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 T_DT=new FXTextField(buttonFrameRight,5,this,ID_DT,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 T_VIS=new FXTextField(buttonFrameRight,5,this,ID_VIS,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0, 4,9,0,0);

 
	new FXHorizontalSeparator(buttonFrameRight,SEPARATOR_GROOVE|LAYOUT_FILL_X);


 sprintf(blah,"0");
 T_WIDTH->setText(blah);	
 T_HEIGHT->setText(blah);	
 T_NX->setText(blah);	
 T_NY->setText(blah);	
 T_GX->setText(blah);	
 T_GY->setText(blah);	
 T_STA->setText(blah);	
 T_DT->setText(blah);	
 T_VIS->setText(blah);

   
 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);
 







	// Initialize private variables


//	E[NX/2]=-1;
/*	spinning=1;
	timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
	chore=getApp()->addChore(this,ID_CHORE);

*/  

/////// Start Fluid Configuration

//ParseFile(argc,argv,&nx,&ny,&fwidth,&fheight,&gx,&gy,&dt,&vis,&sta,&gmode,&BNum,&BCells,&BPoints);


}
// Destructor
GLTestWindow::~GLTestWindow(){
  if(timer) getApp()->removeTimeout(timer);
  }

 



// Create and initialize 
void GLTestWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}






int GLTestWindow::timestep(double dx, double dy, double gx, double gy, double vis,
	     double t, double dt, int nx, int ny, int imax, int jmax,
	     int width, int height, int **Flag, double **Un1, double **Vn1,
	     double **U, double **V, double **P, double **D, double **R, double sta)
{
//		pbar->setProgress(10);
    ReflagCells(Vn1, Flag, P, U, V, nx, ny, particle1);
//		pbar->setProgress(20);
    CalculateDij(Flag, D, U, V, dx, dy, nx, ny);
//		pbar->setProgress(30);
    PressureSource(Flag, U, V, D, R, nx, ny, dx, dy, vis, dt);
//	pbar->setProgress(40);
    if (CalculatePressure(Flag, R, P, nx, ny, dx, dy, gx, gy, vis, U, V) !=
	0) return (1);
//		pbar->setProgress(50);
    CalculateVelocity(Un1, Vn1, Flag, R, P, nx, ny, dx, dy, gx, gy, vis, U,
		      V, dt);
//		pbar->setProgress(60);
    CalculateSurface(Un1, Vn1, Flag, P, D, U, V, nx, ny, gx, gy, dt, dx,
		     dy);
//		pbar->setProgress(70);
	CalculateBNDConditions(Un1, Vn1, Flag, U, P, V, D, nx, ny, gx, gy, vis,
			   dx, dy);
    if (teststability(U, V, nx, ny, dx, dy, dt, vis, sta) != 0)
	return (1);
//		pbar->setProgress(80);
    ParticleMovement(particle1, dx, dy, nx, ny, width, height, Flag, U, V,
		     dt);
//		pbar->setProgress(100);
    return (0);
}

// Widget has been resized
long GLTestWindow::onConfigure(FXObject*,FXSelector,void*){
  if(glcanvas->makeCurrent()){
    glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());
    glcanvas->makeNonCurrent();
    }
  return 1;
  }



// Widget needs repainting
long GLTestWindow::onExpose(FXObject*,FXSelector,void*){
  drawScene();
  return 1;
  }


//  Rotate the boxes when a timer message is received
long GLTestWindow::onTimeout(FXObject*,FXSelector,void*){
  angle += 2.;
  if(angle > 360.) angle -= 360.;
  drawScene();
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }


int p=0;
// Start simulation when START button on
long GLTestWindow::onChore(FXObject* a,FXSelector b,void* c){

		int i;
  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();


	
  if(frame!=0)
	sprintf(blah,"Frame:        %d",frame);
	framenr->setText(blah);	

		
	do{
	    for (i = 0; i < ((int) ((dtmax / dt))); i++) {


		pbar->setProgress(((i+1)/(dtmax/dt))*100);


//		printf("i=%d(%d)\n",i,(int)(dtmax/dt)-1);

		stability=0;

		stability +=
		    timestep(dx, dy, gx, gy, vis, t, dt, nx, ny, imax,
			     jmax, width, height, Flag, Un1, Vn1, U, V, P,
			     D, R, sta);


		stability += teststability(U, V, nx, ny, dx, dy, dt, vis, sta);
		if (stability != 0) 
		{
			if (frame > 1) 
		    {
			CopyParticles(_particle1,particle1);
			CopyTableI(_Flag, Flag, nx, ny);
			CopyTable(_U, U, nx, ny);
			CopyTable(_V, V, nx, ny);
			CopyTable(_Vn1, Vn1, nx, ny);
			CopyTable(_Un1, Un1, nx, ny);
			CopyTable(_P, P, nx, ny);
			CopyTable(_D, D, nx, ny);
			CopyTable(_R, R, nx, ny);
		    }
		    dt /= 2.0;
		    break;
		}
	}

	} while (stability != 0);



	dt *= 2.0;
	if (dt > dtmax)
	    dt = dtmax;
	frame+=1;

#ifdef demoversionFRAME
	if(frame>10)
	{
	  spinning=0;
	  if(timer){
	    getApp()->removeTimeout(timer);
	    timer=NULL;
	    }
	  if(chore){
	    getApp()->removeChore(chore);
	    chore=NULL;
	    }
			onCmdDemo2(a,b,c);
			frame=1;
			return 1;
	}
#endif	

//	printf("[%d] \n ", frame);

	if (frame > 1) 
	{
		CopyParticles(particle1,_particle1);
		CopyTableI(Flag, _Flag, nx, ny);
		CopyTable(U, _U, nx, ny);
		CopyTable(V, _V, nx, ny);
		CopyTable(Vn1, _Vn1, nx, ny);
		CopyTable(Un1, _Un1, nx, ny);
		CopyTable(P, _P, nx, ny);
		CopyTable(D, _D, nx, ny);
		CopyTable(R, _R, nx, ny);
	}


	Render();

	if(write)
		V_Write(frame);


  drawScene();
  chore=getApp()->addChore(this,ID_CHORE);

  return 1;
  }


void GLTestWindow::Render(void){

 	V_ClearRect(0,0,fwidth-1,fheight-1);

	if(CELLSONOFF)
		DrawMesh(fwidth,fheight,nx,ny,dx,dy);

	if(PARTICLESONOFF)
	{
		if(!PRESSUREONOFF)
		{
			for (j = 1; j <= ny; j++)
			for (i = 1; i <= nx; i++)
				Vn1[i][j] = 0;
		
			PlotParticles(_particle1,fwidth,fheight,Vn1,nx,ny);
		}
		else
			PlotParticles(_particle1,fwidth,fheight,P,nx,ny);
	}


    if(VELOCITYONOFF)
		PlotVelocities(fwidth,fheight,nx,ny,dx,dy,U,V,Flag);
	
	
	DrawBndCells(fwidth,fheight,nx,ny,dx,dy,Flag);
	
}


// Start the boxes spinning
long GLTestWindow::onCmdSpin(FXObject* sender,FXSelector b,void* c){
	FXButton* button=(FXButton*)sender;
	spinning=1;
    spinning ? B_START->disable() : B_START->enable(); 
    spinning ? B_STOP->enable() : B_STOP->disable(); 
	spinning ? B_WRITEONOFF->disable() : B_WRITEONOFF->enable();
	
	timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
	chore=getApp()->addChore(this,ID_CHORE);

	sprintf(blah,"Please Wait A Little..");
	framenr->setText(blah);	

   return 1;
  }



// Enable or disable the spin button
long GLTestWindow::onUpdSpin(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
     
    loaded ? (spinning ? B_START->disable() : B_START->enable()) : B_START->disable(); 
	spinning ? B_WRITEONOFF->disable() : B_WRITEONOFF->enable();
  	
 
	return 1;
}


// Start the boxes spinning
long GLTestWindow::onCmdSpinFast(FXObject*,FXSelector,void*){
//  spinning=1;
//  chore=getApp()->addChore(this,ID_CHORE);
  return 1;
  }

long GLTestWindow::onCmdWrite(FXObject*,FXSelector,void*){

	write=!write;


return 1;
  }


long GLTestWindow::onCmdAbout(FXObject*,FXSelector,void*){

FXMessageBox about(this,"Fluid Navier-Stokes Solver.",FXStringFormat("DEMO OF \n                       FLUID V1.0                      \n\n       The MAC Method      \nNavier-Stokes, Incompressible,\nViscous, Free Surface solver,\n\nCopyright (C) 2001, Maciej Matyka\n (maq@panoramix.ift.uni.wroc.pl)\n\nIf you wanna register program 6$\n \n full source codes 10$ (US):\n- source code for linux console version (pure ansi c)\n- source code for windows with GUI created with FOX library (c++) \n\nMaciej Matyka (BSK SA) SWIFT: SKAT PL PK  10501575 – 2245341900 \n\n or better by postal order to my home adress: \n\nMaciej Matyka\nul. Kielczowska 137/10\n51-315 Wroclaw\nPoland\n\n When i receive money - you will get fluid .zip package with program or source code \n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  about.execute();

return 1;
  }

long GLTestWindow::onUpdAbout(FXObject*,FXSelector,void*){


return 1;
  }


long GLTestWindow::onCmdPRESSUREONOFF(FXObject* a,FXSelector b,void* c){
	(PRESSUREONOFF) ? PRESSUREONOFF = 0 : PRESSUREONOFF = 1;
	Render();
    drawScene();
   return 1;
}

long GLTestWindow::onCmdCELLSONOFF(FXObject* a,FXSelector b,void* c){
	(CELLSONOFF) ? CELLSONOFF = 0 : CELLSONOFF = 1;
	Render();
    drawScene();
   return 1;
}

long GLTestWindow::onCmdVELOCITYONOFF(FXObject* a,FXSelector b,void* c){
	(VELOCITYONOFF)	? VELOCITYONOFF = 0 : VELOCITYONOFF = 1;
	Render();
    drawScene();
   return 1;
}

long GLTestWindow::onCmdPARTICLESONOFF(FXObject* a,FXSelector b,void* c){
	(PARTICLESONOFF) ? PARTICLESONOFF = 0 : PARTICLESONOFF = 1;
	
	PARTICLESONOFF ? B_PRESSUREONOFF->enable() : B_PRESSUREONOFF->disable();


	Render();
    drawScene();
   return 1;
}


long GLTestWindow::onCmdModified(FXObject*,FXSelector,void*)
{
 loaded=0;
  return 1;
}

	

long GLTestWindow::onUpdConfig(FXObject* sender,FXSelector,void*){
//	loaded ? configlist->disable() : configlist->enable();
   return 1;
  }


long GLTestWindow::onCmdConfig(FXObject* a,FXSelector b ,void* c)
{
	 FXint item;
	 int i;
	 int argc = 2;
	 char *argv[3];

	 

 	sprintf(blah,"Please Wait A Little..");
	framenr->setText(blah);	


	item=configlist->getCurrentItem();

         switch (item){
          case 0:
			  {
				  argv[1]="opt/splash.opt";
		  } break;

          case 1:
			  { 
				  argv[1]="opt/brokendamn.opt";
		          
			  } break;

          case 2:
			  { 
				  argv[1]="opt/rownaj.opt";
		          
			  } break;

	      case 3:
			  { 
				  argv[1]="opt/sluza.opt";
		          
			  } break;
		  



		  default:break;

	 }

	ParseFile(argc,argv,&nx,&ny,&fwidth,&fheight,&gx,&gy,&dt,&vis,&sta,&gmode,&BNum,&BCells,&BPoints);



if(particle)
    FreeAllParticles(particle1);
if(_particle1)
    FreeAllParticles(_particle1);


if(_Flag)
    Free2TableI(_Flag, imax, jmax);
if(_R)
    Free2TableD(_R, imax, jmax);
if(_D)
    Free2TableD(_D, imax, jmax);
if(_P)
    Free2TableD(_P, imax, jmax);
if(_U)
    Free2TableD(_U, imax, jmax);
if(_V)
    Free2TableD(_V, imax, jmax);
if(_Un1)
    Free2TableD(_Un1, imax, jmax);
if(_Vn1)
    Free2TableD(_Vn1, imax, jmax);

/*    Free2TableD(R, imax, jmax);
    Free2TableD(D, imax, jmax);
    Free2TableD(P, imax, jmax);
    Free2TableD(U, imax, jmax);
    Free2TableD(V, imax, jmax);
    Free2TableD(Un1, imax, jmax);
    Free2TableD(Vn1, imax, jmax);
    Free2TableI(Flag, imax, jmax);
    Free1TableI(BPoints);
    Free1TableI(BCells);
}
*/

	dtstart=dtmax=dt;

    imax = nx + 2;
    jmax = ny + 2;
    dx = fwidth / nx;
    dy = fheight / ny;


	AllocTables();



    BundaryCellsToPoints(BCells, BPoints, BNum, fwidth, fheight, dx, dy);
    CellSetup15(Flag, BPoints, BNum, fwidth, fheight, nx, ny, dx, dy);


	if(item==1)
	{
	
		for(i=3;i<nx-2;i++)
{
	Flag[i][ny-1] &= ~C_EMPBND;
	Flag[i][ny-1] &= ~C_BND;
	Flag[i][ny-1] |= C_OB;
	Flag[i][ny-1] |= C_URON;
	Flag[i][ny] |= C_BND;

}

	
	
    Flag[17][10] &= ~C_BND;
    Flag[18][10] &= ~C_BND;
	Flag[15][10] &= ~C_BND;
	Flag[16][10] &= ~C_BND;
	Flag[15][10] |= C_OB;
	Flag[18][10] |= C_OB;
	
	Flag[15][10] &= ~C_URON;
	Flag[16][10] &= ~C_URON;
	Flag[17][10] &= ~C_URON;
	Flag[18][10] &= ~C_URON;
	Flag[15][9] &= ~C_URON;
	Flag[16][9] &= ~C_URON;
	Flag[17][9] &= ~C_URON;
 	Flag[18][9] &= ~C_URON;
	

	Flag[32][10] &= ~C_URON;
 	Flag[32][11] &= ~C_URON;
	Flag[31][10] &= ~C_OB;
	Flag[32][10] &= ~C_OB;
	Flag[32][11] &= ~C_OB;
	Flag[32][11] &= ~C_COR;

	}


	particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);
	_particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);

   how_much_particles=0;

sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);
   
   for (wrkpartile = particle1; wrkpartile != NULL; wrkpartile = wrkpartile->next) 
    how_much_particles+=1; 

   

		FXMessageBox demo(this,"Demo",FXStringFormat("       You wanted to use modified .opt file! \n\n\n  It's demo version of Fluid solver. \n\nIf you want to solve your own fluid problems\n please register program.\n\nMore info - see ABOUT or http://panoramix.ift.uni.wroc.pl/~maq\n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
#ifdef demoversion		
         switch (item){
          case 0:
			  {
				if(how_much_particles != 34588 || vis!=0.7 || fwidth!=320
					|| fheight !=200 || nx!=64 || ny!=40 || dt != 8)
				{
					demo.execute();
					loaded=0;
					return 1;
				
				}
			  } break;

          case 1:
			  { 
				if(how_much_particles != 51001)
				{
					demo.execute();
					loaded=0;
					return 1;
				
				}
	
		          
			  } break;

          case 2:
			  { 
				if(how_much_particles != 29851 || vis!=0.6|| fwidth!=320
					|| fheight !=200 || nx!=64 || ny!=40 || dt != 0.5)
				{
					demo.execute();
					loaded=0;
					return 1;
				
				}
	

	          
			  } break;

	      case 3:
			  { 
				if(how_much_particles != 19101 || vis!=0.6 || fwidth!=320
					|| fheight !=200 || nx!=64 || ny!=40 || dt != 0.3)
				{
					demo.execute();
					loaded=0;
					return 1;
				
				}
	


			  } break;
		  



		  default:break;

	 }
#endif


 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);

		for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			if(!( (Flag[i][j] & C_BND) || (Flag[i][j] & C_EMPBND) ))
					ReflagTo(Flag,i,j,C_EMP);

  CalculateBNDConditions(Un1, Vn1, Flag, U, P, V, D, nx, ny, gx, gy, vis,
			   dx, dy);

  V_OpenView(fwidth,fheight);


/*
  V_ClearRect(0,0,fwidth-1,fheight-1);
  
  DrawMesh(fwidth,fheight,nx,ny,dx,dy);
  DrawBndCells(fwidth,fheight,nx,ny,dx,dy,Flag);
  PlotParticles(_particle1,fwidth,fheight,P,nx,ny);
*/ 

  //PlotParticles(particle1,width,height,P,nx,ny);

	frame=0;	
	loaded=1;

 sprintf(blah,"%d",fwidth);
 T_WIDTH->setText(blah);	

 sprintf(blah,"%d",fheight);
  T_HEIGHT->setText(blah);	

  sprintf(blah,"%d",nx);
 T_NX->setText(blah);	

 sprintf(blah,"%d",ny);
 T_NY->setText(blah);	

 sprintf(blah,"%.3f",gx);
 T_GX->setText(blah);	

 sprintf(blah,"%.3f",gy);
 T_GY->setText(blah);	

 sprintf(blah,"%.3f",sta);
 T_STA->setText(blah);	

#ifdef demoversionDT
 dt=0.05;
#endif
 sprintf(blah,"%.3f",dt);
 T_DT->setText(blah);	

 sprintf(blah,"%.3f",vis);
 T_VIS->setText(blah);


	if(loaded!=0)
	{
     Render();
     drawScene();
	}

 
		do{
	    for (i = 0; i < ((int) ((dtmax / dt))); i++) {


		pbar->setProgress(((i+1)/(dtmax/dt))*100);


//		printf("i=%d(%d)\n",i,(int)(dtmax/dt)-1);

		stability=0;

		stability +=
		    timestep(dx, dy, gx, gy, vis, t, dt, nx, ny, imax,
			     jmax, width, height, Flag, Un1, Vn1, U, V, P,
			     D, R, sta);


		stability += teststability(U, V, nx, ny, dx, dy, dt, vis, sta);
		if (stability != 0) 
		{
			if (frame > 1) 
		    {
			CopyParticles(_particle1,particle1);
			CopyTableI(_Flag, Flag, nx, ny);
			CopyTable(_U, U, nx, ny);
			CopyTable(_V, V, nx, ny);
			CopyTable(_Vn1, Vn1, nx, ny);
			CopyTable(_Un1, Un1, nx, ny);
			CopyTable(_P, P, nx, ny);
			CopyTable(_D, D, nx, ny);
			CopyTable(_R, R, nx, ny);
		    }
		    dt /= 2.0;
		    break;
		}
	}

	} while (stability != 0);



	dt *= 2.0;
	if (dt > dtmax)
	    dt = dtmax;
	frame+=1;

//	printf("[%d] \n ", frame);

	if (frame > 1) 
	{
		CopyParticles(particle1,_particle1);
		CopyTableI(Flag, _Flag, nx, ny);
		CopyTable(U, _U, nx, ny);
		CopyTable(V, _V, nx, ny);
		CopyTable(Vn1, _Vn1, nx, ny);
		CopyTable(Un1, _Un1, nx, ny);
		CopyTable(P, _P, nx, ny);
		CopyTable(D, _D, nx, ny);
		CopyTable(R, _R, nx, ny);
	}




  Render();
  drawScene();
 
  sprintf(blah,"Press Start Button.");
  framenr->setText(blah);	

 B_PRESSUREONOFF->enable();
 B_CELLSONOFF->enable();
 B_VELOCITYONOFF->enable();
 B_PARTICLESONOFF->enable();





	return 1;
}






long GLTestWindow::onCmdWIDTH(FXObject* a,FXSelector b,void* c){
  double mywidth;

  mywidth=FXDoubleVal(T_WIDTH->getText());	


  if(mywidth<0)
  {
		FXMessageBox demo(this,"WIDTH",FXStringFormat("Width of output must be > 0.",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",fwidth);
		T_WIDTH->setText(blah);	
		return 1;
  }

  fwidth=mywidth;

  Render();
  drawScene();

  
  return 1;
}
long GLTestWindow::onCmdHEIGHT(FXObject* a,FXSelector b,void* c){
  double myheight;

  myheight=FXDoubleVal(T_HEIGHT->getText());	

  if(myheight<0)
  {
		FXMessageBox demo(this,"HEIGHT",FXStringFormat("Height of output must be > 0.",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",fheight);
		T_HEIGHT->setText(blah);	
		return 1;
  }

  fheight=myheight;

  Render();
  drawScene();


  return 1;
}

long GLTestWindow::onCmdNX(FXObject* a,FXSelector b,void* c){
  double mynx;


  mynx=FXDoubleVal(T_NX->getText());	

  if(mynx==0) mynx=1;
   

  if(mynx<0)
  {
		FXMessageBox demo(this,"NX",FXStringFormat("NX of cells must be > 0.",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",nx);
		T_NX->setText(blah);	
		return 1;
  }



  nx=mynx;

  Render();
  drawScene();


  return 1;
}

long GLTestWindow::onCmdNY(FXObject* a,FXSelector b,void* c){
  double myny;


  myny=FXDoubleVal(T_NY->getText());	

  if(myny<0)
  {
		FXMessageBox demo(this,"NY",FXStringFormat("NY of cells must be > 0.",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",ny);
		T_NY->setText(blah);	
		return 1;
  }



  ny=myny;

  Render();
  drawScene();

  return 1;
}

long GLTestWindow::onCmdGX(FXObject* a,FXSelector b,void* c){
	double mygx;

  mygx=FXDoubleVal(T_GX->getText());	
  
  gx=mygx;


  return 1;
}

long GLTestWindow::onCmdGY(FXObject* a,FXSelector b,void* c){
	double mygy;

  mygy=FXDoubleVal(T_GY->getText());	
  
  gy=mygy;



  return 1;
}


long GLTestWindow::onCmdSTA(FXObject* a,FXSelector b,void* c){
  double mysta;


  mysta=FXDoubleVal(T_STA->getText());	
  
  if(mysta<0)
  {
		FXMessageBox demo(this,"Stability Condition",FXStringFormat("Stability must be > 0.\nRemember than\n smaller stability condition value = more time to render, and better accuracy\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",sta);
		T_STA->setText(blah);	
		return 1;
  }



  sta=mysta;



  return 1;
}

long GLTestWindow::onCmdVIS(FXObject* a,FXSelector b,void* c){
  double myvis;


  myvis=FXDoubleVal(T_VIS->getText());	
  
  if(myvis<0)
  {
		FXMessageBox demo(this,"Viscosity",FXStringFormat("Fluid with viscosity < 0? Interesting... Can you email me? :)",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",vis);
		T_VIS->setText(blah);	
		return 1;
  }



  vis=myvis;



  return 1;
}




long GLTestWindow::onCmdDT(FXObject* a,FXSelector b,void* c){

#ifdef demoversionDT
  FXMessageBox demo(this,"DT",FXStringFormat("Sorry but it's demo version. \nIf you want to change DT of calculations\nRegister program\nMore info - see about or http://panoramix.ift.uni.wroc.pl/~maq\n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  demo.execute();

  dt=0.05;
  sprintf(blah,"%.3f",dt);
  T_DT->setText(blah);	
#else
  double mydt;

  mydt=FXDoubleVal(T_DT->getText());	
  
  if(mydt<0)
  {
		FXMessageBox demo(this,"DT",FXStringFormat("DT < 0? Email me pleeeeaase :)",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
		demo.execute();
		sprintf(blah,"%d",dt);
		T_DT->setText(blah);	
		return 1;
  }



  dt=mydt;

#endif


  return 1;
}



long GLTestWindow::onCmdDemo2(FXObject* a,FXSelector b,void* c){

FXMessageBox demo(this,"Demo",FXStringFormat("Sorry but it's demo version. \nIf you want render more frames than 10\n please register program.\n\nMore info - see about or http://panoramix.ift.uni.wroc.pl/~maq\n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  demo.execute();
  loaded=0;



  return 1;
}

long GLTestWindow::onCmdDemo(FXObject*,FXSelector,void*){

FXMessageBox demo(this,"Demo",FXStringFormat("Sorry but it's demo version. \nIf you want load & make your own fluid configurations\nplease register program.\n\nMore info - see about or http://panoramix.ift.uni.wroc.pl/~maq\n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  demo.execute();

sprintf(blah,"%d",fwidth);
 T_WIDTH->setText(blah);	

 sprintf(blah,"%d",fheight);
  T_HEIGHT->setText(blah);	

  sprintf(blah,"%d",nx);
 T_NX->setText(blah);	

 sprintf(blah,"%d",ny);
 T_NY->setText(blah);	

 sprintf(blah,"%.3f",gx);
 T_GX->setText(blah);	

 sprintf(blah,"%.3f",gy);
 T_GY->setText(blah);	

 sprintf(blah,"%.3f",sta);
 T_STA->setText(blah);	

 sprintf(blah,"%.3f",dt);
 T_DT->setText(blah);	

 sprintf(blah,"%.3f",vis);
 T_VIS->setText(blah);



return 1;
  }



long GLTestWindow::onUpdWrite(FXObject* sender,FXSelector,void*){
	spinning ? B_WRITEONOFF->disable() : B_WRITEONOFF->enable();
   return 1;
  }

long GLTestWindow::onCmdQuit(FXObject*,FXSelector,void*){

	FXuint answer;

    
  if(chore){
    getApp()->removeTimeout(timer);
    timer=NULL;
    getApp()->removeChore(chore);
    chore=NULL;
	answer=FXMessageBox::question(this,MBOX_YES_NO,"Please confirm.","Sure want to exit?");
    if(answer==MBOX_CLICKED_YES)
		getApp()->exit(0);

	chore=getApp()->addChore(this,ID_CHORE);
	timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
 	}
   else
   {
	answer=FXMessageBox::question(this,MBOX_QUIT_CANCEL,"Please confirm.","Sure want to exit?");
    if(answer==MBOX_CLICKED_QUIT)
		getApp()->exit(0);
   }

	return 1;
  }

long GLTestWindow::onUpdQuit(FXObject* sender,FXSelector,void*){
   return 1;
  }


 FXString directory = "opt/";
 
long GLTestWindow::onCmdLoad(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  FXString file;
  FXString pattern = "*.opt";
  FXFileDialog opendialog(this,"Open .opt file.");
  int argc = 2;
  char *argv[3];

  opendialog.setSelectMode(SELECTFILE_EXISTING);

  
//void setPattern(const FXString& ptrn)  
  opendialog.setPattern(pattern);
  opendialog.setDirectory(directory);


  if(opendialog.execute())
  {
    file=opendialog.getFilename();
	directory=opendialog.getDirectory();


//    insertFile(file);

	argv[0]="Fluid";
	argv[1]=(char *)file.text();
	
	
	
	ParseFile(argc,argv,&nx,&ny,&fwidth,&fheight,&gx,&gy,&dt,&vis,&sta,&gmode,&BNum,&BCells,&BPoints);



if(particle)
    FreeAllParticles(particle1);
if(_particle1)
    FreeAllParticles(_particle1);


if(_Flag)
    Free2TableI(_Flag, imax, jmax);
if(_R)
    Free2TableD(_R, imax, jmax);
if(_D)
    Free2TableD(_D, imax, jmax);
if(_P)
    Free2TableD(_P, imax, jmax);
if(_U)
    Free2TableD(_U, imax, jmax);
if(_V)
    Free2TableD(_V, imax, jmax);
if(_Un1)
    Free2TableD(_Un1, imax, jmax);
if(_Vn1)
    Free2TableD(_Vn1, imax, jmax);

/*    Free2TableD(R, imax, jmax);
    Free2TableD(D, imax, jmax);
    Free2TableD(P, imax, jmax);
    Free2TableD(U, imax, jmax);
    Free2TableD(V, imax, jmax);
    Free2TableD(Un1, imax, jmax);
    Free2TableD(Vn1, imax, jmax);
    Free2TableI(Flag, imax, jmax);
    Free1TableI(BPoints);
    Free1TableI(BCells);
}
*/

	dtstart=dtmax=dt;

    imax = nx + 2;
    jmax = ny + 2;
    dx = fwidth / nx;
    dy = fheight / ny;


	AllocTables();

//    printf("tables...\n");


//    printf("...done\n");

//    printf("\n");		/* for progress */


    BundaryCellsToPoints(BCells, BPoints, BNum, fwidth, fheight, dx, dy);
    CellSetup15(Flag, BPoints, BNum, fwidth, fheight, nx, ny, dx, dy);


	particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);
	_particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);

   how_much_particles=0;

sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);
   
   for (wrkpartile = particle1; wrkpartile != NULL; wrkpartile = wrkpartile->next) 
    how_much_particles+=1; 
	
 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);

		for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			if(!( (Flag[i][j] & C_BND) || (Flag[i][j] & C_EMPBND) ))
					ReflagTo(Flag,i,j,C_EMP);

  CalculateBNDConditions(Un1, Vn1, Flag, U, P, V, D, nx, ny, gx, gy, vis,
			   dx, dy);

  V_OpenView(fwidth,fheight);


/*
  V_ClearRect(0,0,fwidth-1,fheight-1);
  
  DrawMesh(fwidth,fheight,nx,ny,dx,dy);
  DrawBndCells(fwidth,fheight,nx,ny,dx,dy,Flag);
  PlotParticles(_particle1,fwidth,fheight,P,nx,ny);
*/ 

  //PlotParticles(particle1,width,height,P,nx,ny);

	frame=0;	
	loaded=1;
    spinning ? button->disable() : button->enable();

 sprintf(blah,"%d",fwidth);
 T_WIDTH->setText(blah);	

 sprintf(blah,"%d",fheight);
  T_HEIGHT->setText(blah);	

  sprintf(blah,"%d",nx);
 T_NX->setText(blah);	

 sprintf(blah,"%d",ny);
 T_NY->setText(blah);	

 sprintf(blah,"%.3f",gx);
 T_GX->setText(blah);	

 sprintf(blah,"%.3f",gy);
 T_GY->setText(blah);	

 sprintf(blah,"%.3f",sta);
 T_STA->setText(blah);	

#ifdef demoversionDT
 dt=0.05;
#endif
 sprintf(blah,"%.3f",dt);
 T_DT->setText(blah);	

 sprintf(blah,"%.3f",vis);
 T_VIS->setText(blah);


 	sprintf(blah,"Please Wait A Little..");
	framenr->setText(blah);	

 
		do{
	    for (i = 0; i < ((int) ((dtmax / dt))); i++) {


		pbar->setProgress(((i+1)/(dtmax/dt))*100);


//		printf("i=%d(%d)\n",i,(int)(dtmax/dt)-1);

		stability=0;

		stability +=
		    timestep(dx, dy, gx, gy, vis, t, dt, nx, ny, imax,
			     jmax, width, height, Flag, Un1, Vn1, U, V, P,
			     D, R, sta);


		stability += teststability(U, V, nx, ny, dx, dy, dt, vis, sta);
		if (stability != 0) 
		{
			if (frame > 1) 
		    {
			CopyParticles(_particle1,particle1);
			CopyTableI(_Flag, Flag, nx, ny);
			CopyTable(_U, U, nx, ny);
			CopyTable(_V, V, nx, ny);
			CopyTable(_Vn1, Vn1, nx, ny);
			CopyTable(_Un1, Un1, nx, ny);
			CopyTable(_P, P, nx, ny);
			CopyTable(_D, D, nx, ny);
			CopyTable(_R, R, nx, ny);
		    }
		    dt /= 2.0;
		    break;
		}
	}

	} while (stability != 0);



	dt *= 2.0;
	if (dt > dtmax)
	    dt = dtmax;
	frame+=1;

//	printf("[%d] \n ", frame);

	if (frame > 1) 
	{
		CopyParticles(particle1,_particle1);
		CopyTableI(Flag, _Flag, nx, ny);
		CopyTable(U, _U, nx, ny);
		CopyTable(V, _V, nx, ny);
		CopyTable(Vn1, _Vn1, nx, ny);
		CopyTable(Un1, _Un1, nx, ny);
		CopyTable(P, _P, nx, ny);
		CopyTable(D, _D, nx, ny);
		CopyTable(R, _R, nx, ny);
	}


	Render();

  drawScene();
 
	sprintf(blah,"Press Start Button.");
	framenr->setText(blah);	

 B_PRESSUREONOFF->enable();
 B_CELLSONOFF->enable();
 B_VELOCITYONOFF->enable();
 B_PARTICLESONOFF->enable();




  }

return 1;
  }

long GLTestWindow::onUpdLoad(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
   spinning ? button->disable() : button->enable(); 
   return 1;
  }






long GLTestWindow::onUpdSpinFast(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
   return 1;
  }

long GLTestWindow::onCmdStop(FXObject*,FXSelector,void*){
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


// Enable or disable the stop button
long GLTestWindow::onUpdStop(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
   spinning ? B_STOP->enable() : B_STOP->disable(); 
  return 1;
  }






// Draw the GL scene
void GLTestWindow::drawScene(void){
  const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
  const GLfloat lightDiffuse[]={.4f,.9f,.4f,1.};
   const GLfloat blueMaterial[]={1.0,0.4,1.0,1.0};


//int ix,iy;


   int i,x,y;

  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();


  glcanvas->makeCurrent();
  glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());
//  glViewport(0,0,fwidth,fheight);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

//	AUX_SETCOLOR(windType, AUX_WHITE);     


	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(1);     





	OutputFrameGL();

 
  // Swap if it is double-buffered
  if(glvisual->isDoubleBuffer())
  {
    glcanvas->swapBuffers();
  }
	

}






 void glinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);      
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);      

	
	mode = GL_FALSE;     
	//size = 5; 

 	//dt=(float)0.1;
   
}




// Here we begin

 
int main(int argc,char *argv[]){

	
  FXApp application("GLTest","FoxTest");
  application.init(argc,argv);
  new GLTestWindow(&application);
  application.create();
  glinit();
 
 
  


  
  
  return application.run();
  }


