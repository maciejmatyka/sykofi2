
#include <string.h>
#include <math.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <stdlib.h>

#include "fx.h"
#include "fx3d.h"

  

/* AVI
*/

#define  STRICT
#define  INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <memory.h>
#include <mmsystem.h>
#include <vfw.h>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
// Our movie is 160x120 and 15 frames long
//

int aviinit=FALSE;
float eps=0;			// fade klatki 1	
	
int avirate=50;

int posdemotxt=20;

#define pocztime 3*25
#define fadetime 3*25



#define BITMAP_X	320
#define BITMAP_Y	200
#define N_FRAMES	15

#define TEXT_HEIGHT	20

#define AVIIF_KEYFRAME	0x00000010L // this frame is a key frame.

#define BUFSIZE 260

#define LPLPBI	LPBITMAPINFOHEADER *

	LPBITMAPINFOHEADER alpbi[N_FRAMES];
	int i;
	AVISTREAMINFO strhdr;
	PAVIFILE pfile = NULL;
	PAVISTREAM ps = NULL, psCompressed = NULL, psText = NULL;
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	HRESULT hr;
	DWORD dwTextFormat;
	WORD wVer;
	char szTitle[BUFSIZE];
	char szMessage[BUFSIZE];
	char szText[BUFSIZE];
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;



	char avinamebuf[100];

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
static void FreeFrame(LPLPBI) ;
static void MakeFrame(LPLPBI, UINT, UINT, UINT) ;
static void MakeFramePocz(LPLPBI, UINT, UINT, UINT,int) ;
static void MakeFrameFade(LPLPBI alpbi, UINT bits, UINT wXSize,UINT wYSize,float eps );
static HANDLE MakeDib(HBITMAP, UINT);




//#define demoversion
//#define demoversionDT
//#define demoversionFRAME

//#define demoversion

int	CELLSONOFF=0;
int	VELOCITYONOFF=0;
int	PARTICLESONOFF=1;
int	PRESSUREONOFF=1;


int j;
int fframe = 0;

extern "C"
{

#include "misc.h"
#include "bndflags.h"
#include "parse.h"
#include "dimalloc.h"
#include "bndinit.h"

#include "particle.h"
#include "visual.h"
#include "tscells.h"
#include "pressure.h"
#include "velocity.h"
#include "minmax.h"
#include "fluid.h"
#include "view.h"


}


#define CI_RED AUX_RED 
#define CI_ANTI_ALIAS_GREEN 16 
#define CI_ANTI_ALIAS_YELLOW 32 
#define CI_ANTI_ALIAS_RED 48  



// global datas



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

  FXList* particleslist;



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
	FXVerticalFrame* m_pFXTabItem3Frame;
	FXTabItem* m_pFXTabItem4;
	FXVerticalFrame* m_pFXTabItem4Frame;
	FXTabItem* m_pFXTabItem5;
	FXVerticalFrame* m_pFXTabItem5Frame;




	FXButton* B_START;
	FXButton* B_STOP;

	FXButton* B_REMOVEALL;
	FXButton* B_REMOVEIT;

	FXCheckButton* B_WRITEONOFF;
    FXTextField*   T_AVINAME;
 
	FXTextField* T_AVIRATE;

	FXImageView      *imageview;


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
	ID_RELOAD,
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

	ID_PARTREMOVE,
	ID_PARTCONFIG,

	ID_REMOVE,
	ID_REMOVEIT,

	ID_AVINAME,
	ID_AVIRATE,
	ID_SAVEIT,
	
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
  long onUpdReLoad(FXObject*,FXSelector,void*);
  long onCmdReLoad(FXObject*,FXSelector,void*);
  long onUpdQuit(FXObject*,FXSelector,void*);
  long onCmdQuit(FXObject*,FXSelector,void*);
  long onUpdAbout(FXObject*,FXSelector,void*);

  long onCmdAbout(FXObject*,FXSelector,void*);
  long onCmdDemo(FXObject*,FXSelector,void*);
  long onCmdDemo2(FXObject*,FXSelector,void*);
  long onCmdModified(FXObject*,FXSelector,void*);

  
  long onCmdAVINAME(FXObject*,FXSelector,void*);
  long onUpdAVINAME(FXObject*,FXSelector,void*);
  long onCmdAVIRATE(FXObject*,FXSelector,void*);
 
 
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

  long onCmdRemoveParticles(FXObject*,FXSelector,void*);
  long onCmdPartConfig(FXObject*,FXSelector,void*);
 
  long onCmdRemoveParticleObject(FXObject*,FXSelector,void*);

  long onCmdSaveIt(FXObject*,FXSelector,void*);



  


  int timestep(double dx, double dy, double gx, double gy, double vis,
	     double t, double dt, int nx, int ny, int imax, int jmax,
	     int width, int height, int **Flag, double **Un1, double **Vn1,
	     double **U, double **V, double **P, double **D, double **R, double sta);

	void Render(void);

 struct particle *AllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
 struct particle *AllocParticleBall(int x0, int y0, int r, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
 void   NegAllocParticleBall(int x0, int y0, int r, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
 void   NegAllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);


struct particle *ParticleFile(int argc, char *argv[], double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V,int verbose);





 


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
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_RELOAD,     GLTestWindow::onCmdReLoad),


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
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_RELOAD,     GLTestWindow::onCmdReLoad),


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

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_PARTREMOVE, GLTestWindow::onCmdRemoveParticles),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_PARTCONFIG, GLTestWindow::onCmdPartConfig),
 
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_PARTCONFIG, GLTestWindow::onCmdPartConfig),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_REMOVE, GLTestWindow::onCmdRemoveParticleObject),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_AVINAME,     GLTestWindow::onCmdAVINAME),
  FXMAPFUNC(SEL_UPDATE,       GLTestWindow::ID_AVINAME,     GLTestWindow::onUpdAVINAME),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_AVIRATE,     GLTestWindow::onCmdAVIRATE),
 
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SAVEIT,     GLTestWindow::onCmdSaveIt),
 
 



  };



// Macro for the GLTestApp class hierarchy implementation
FXIMPLEMENT(GLTestWindow,FXMainWindow,GLTestWindowMap,ARRAYNUMBER(GLTestWindowMap))




// Construct a GLTestApp
GLTestWindow::GLTestWindow(FXApp* a):FXMainWindow(a,"Fluid version 1.3 - 2D Navier-Stokes based, incompressible, viscous, Free Surface fluid flow solver.",NULL,NULL,DECOR_ALL,0,0,680,370){
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
	m_pFXTabItem1->setText("Control");
	m_pFXTabItem2 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 72,6,66,21, 2,2,2,2);
	m_pFXTabItem2Frame = new FXHorizontalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem2->setText("Physic");

	
	m_pFXTabItem3 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 130,6,66,21, 2,2,2,2);
	m_pFXTabItem3Frame = new FXVerticalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem3->setText("Particles");



/*
  Make particle configuration list */

  particleslist=new FXList(m_pFXTabItem3Frame,9,this,ID_PARTCONFIG,LAYOUT_TOP|FRAME_RIDGE|LIST_SINGLESELECT|LAYOUT_FILL_X);
  particleslist->appendItem("          PARTICLE CONFIGURATION       ");
  particleslist->appendItem(" ");
  particleslist->selectItem(0);
	B_REMOVEIT = new FXButton(m_pFXTabItem3Frame,"&Remove\tRemove particle object.",NULL,this,ID_REMOVE,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);
	B_REMOVEALL = new FXButton(m_pFXTabItem3Frame,"&Remove All\tRemove all objects.",NULL,this,ID_PARTREMOVE,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);

	
	m_pFXTabItem5 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 130,6,66,21, 2,2,2,2);
	m_pFXTabItem5Frame = new FXVerticalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem5->setText("Avi options");

 new FXHorizontalSeparator(m_pFXTabItem5Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem5Frame," Avi animation options. ",NULL,LAYOUT_FILL_X);
 new FXHorizontalSeparator(m_pFXTabItem5Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 
 
 new FXLabel(m_pFXTabItem5Frame,"Frame rate (fps)",NULL,LAYOUT_FILL_X);
 T_AVIRATE=new FXTextField(m_pFXTabItem5Frame,5,this,ID_AVIRATE,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,9,0,0);
 sprintf(blah,"%d",avirate);
 T_AVIRATE->setText(blah);	
 
 

	m_pFXTabItem4 = new FXTabItem(m_pFXTabBook1, 0, 0, TAB_TOP, 190,6,66,21, 2,2,2,2);
	m_pFXTabItem4Frame = new FXVerticalFrame(m_pFXTabBook1,FRAME_THICK|FRAME_RAISED);
	m_pFXTabItem4->setText("About");


 new FXHorizontalSeparator(m_pFXTabItem4Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame,"Fluid v1.3b",NULL,LAYOUT_FILL_X);
 new FXHorizontalSeparator(m_pFXTabItem4Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);



/* new FXLabel(m_pFXTabItem4Frame," ",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame,"    It's demo version of incompressible, viscous,",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," free surface, based on MAC (marker and cell)    ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," 2d solver.",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
*/

new FXLabel(m_pFXTabItem4Frame," Incompressible Navier-Stokes solver.",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," If you think my work is worth something",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," please register fluid, becouse it's demo",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," version. It cost only",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," 6$ (US)",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," Author: Maciej Matyka",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," Adress: ul.Kielczowska 137/10",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," 51-315 Wroclaw",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," email: maq@panoramix.ift.uni.wroc.pl",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," url: http://panoramix.ift.uni.wroc.pl/~maq",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," Any suggestions? Write to me!",NULL,LAYOUT_FILL_X);
               
/*"DEMO OF \n
 FLUID V1.21b
 \n\n
 The MAC Method      \n
 Navier-Stokes, Incompressible,\n
 Viscous, Free Surface solver,\n\n
 Copyright (C) 2001, Maciej Matyka\n 
 (maq@panoramix.ift.uni.wroc.pl)\n\n
 If you wanna register program 6$\n \n full source codes 10$ (US):\n- source code for linux console version (pure ansi c)\n- source code for windows with GUI created with FOX library (c++) \n\nMaciej Matyka (BSK SA) SWIFT: SKAT PL PK  10501575 – 2245341900 \n\n or better by postal order to my home adress: \n\nMaciej Matyka\nul. Kielczowska 137/10\n51-315 Wroclaw\nPoland\n\n When i receive money - you will get fluid .zip package with program or source code \n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
*/   
 
 
 
 /* new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," bablabalbablbalbaa ",NULL,LAYOUT_FILL_X);
 new FXLabel(m_pFXTabItem4Frame," ",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
*/ 
 
//  imageview=new FXImageView(m_pFXTabItem4Frame,NULL,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);

//",FXStringFormat("DEMO OF \n                       FLUID V1.0                      \n\n       The MAC Method      \nNavier-Stokes, Incompressible,\nViscous, Free Surface solver,\n\nCopyright (C) 2001, Maciej Matyka\n (maq@panoramix.ift.uni.wroc.pl)\n\nIf you wanna register program 6$\n \n full source codes 10$ (US):\n- source code for linux console version (pure ansi c)\n- source code for windows with GUI created with FOX library (c++) \n\nMaciej Matyka (BSK SA) SWIFT: SKAT PL PK  10501575 – 2245341900 \n\n or better by postal order to my home adress: \n\nMaciej Matyka\nul. Kielczowska 137/10\n51-315 Wroclaw\nPoland\n\n When i receive money - you will get fluid .zip package with program or source code \n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
 

  
  // RIGHT pane for the buttons	
//  buttonFrame=new FXVerticalFrame(frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);

//  buttonFrame=new FXVerticalFrame(m_pFXTabItem1Frame,FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  buttonFrame=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);




  //	new FXButton(buttonFrame,"&Load Configuration.",NULL,this,ID_LOAD,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);


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
//	new FXButton(buttonFrame,"&About\tAbout program.",NULL,this,ID_ABOUT,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);

 new FXLabel(buttonFrame,"Configuration",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 new FXHorizontalSeparator(buttonFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);

  new FXButton(buttonFrame,"&Load",NULL,this,ID_LOAD,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);

#ifndef demoversion  
  new FXButton(buttonFrame,"&Reload",NULL,this,ID_RELOAD,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);
#endif


  
  
 new FXLabel(buttonFrame,"Output file",NULL,JUSTIFY_LEFT|LAYOUT_FILL_X);
 
 new FXHorizontalSeparator(m_pFXTabItem5Frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
 
 B_WRITEONOFF = new FXCheckButton(buttonFrame,"    Write .avi file.",this,ID_WRITE,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
 T_AVINAME=new FXTextField(buttonFrame,7,this,ID_AVINAME,JUSTIFY_LEFT|FRAME_SUNKEN|FRAME_THICK, 0,0,0,0,  4,11,0,0);

 sprintf(avinamebuf,"anim.avi");
 T_AVINAME->setText(avinamebuf);	
  
	if(write)
		T_AVINAME->enable();
	else
		T_AVINAME->disable();


  new FXButton(buttonFrame,"&Save .PPM Now",NULL,this,ID_SAVEIT,FRAME_THICK|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X,0,0,0,0,10,10,5,5);



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





  // LEFT pane to contain the glcanvas


  glcanvasFrame=new FXVerticalFrame(frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
//  glcanvasFrame=new FXVerticalFrame(m_pFXTabItem1Frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
   
  infoglcanvas = new FXLabel(glcanvasFrame,"Output View.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

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
 
	/*
#ifdef demoversion


  configlist=new FXList(buttonFrameRight2,6,this,ID_CONFIG,FRAME_RAISED|LIST_SINGLESELECT|LAYOUT_FILL_X);
  configlist->appendItem("Splash");
  configlist->appendItem("Complicated");
  configlist->appendItem("Rownaj");
  configlist->appendItem("Sluza");
  
 configlist->selectItem(1);
#endif
*/
	


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



/***************************************************************8
   Allocating particles functions
*****************************************************************/



struct particle *GLTestWindow::AllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;

	sprintf(blah,"RECT %d,%d,%d,%d,%.2f,%.3f,%.3f", x0, y0, x1, y1, partstep, Uk, Vk);
	particleslist->appendItem(blah);
	particleobjects+=1;

/*	if(x0<2) x0=2;*/
/*	if(x1<2) x1=2;*/
/*	if(x0>width-2) x0=width-2;*/
/*	if(x1>width-2) x1=width-2;*/
/*	if(y0<2) y0=2;*/
/*	if(y1<2) y1=2;*/
/*	if(y0>height-2) y0=height-2;*/
/*	if(y1>height-2) y1=height-2;*/


	for(y=y0;y<y1;y+=partstep)
	{
		for(x=x0;x<x1;x+=partstep)
		{

			wrkpart=PlaceParticle(x,y,wrkpart,Uk,Vk,dx,dy,nx,ny,width,height,F,U,V);

 	  }
  }

	return(wrkpart);
}



void GLTestWindow::NegAllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;

	struct particle *workpart;


	sprintf(blah,"!RECT %d,%d,%d,%d,%.2f,%.3f,%.3f", x0, y0, x1, y1, partstep, Uk, Vk);
	particleslist->appendItem(blah);
	particleobjects+=1;


	
	for(workpart=wrkpart;workpart!=NULL;workpart=workpart->next)
	{

/*		printf("!\n");*/
			x=workpart->x;
			y=workpart->y;

/*			printf("%f,%f\n",x,y);*/

			if( x > min(x1,x0) && x< max(x1,x0) && y > min(y1,y0) && y < max(y1,y0))
			{
					workpart->i = 0;
  	      workpart->j = 0;
    	    workpart->flag=P_AVAIL;
      	  workpart->x = 0;
        	workpart->y = 0;

			}


  }

}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: struct particle *AllocParticleBall(int x0, int y0, int r, double partstep, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F)
	  it:       Alocate ball of particles.
		args:     see...
		return:   struct particle *
    todo:     grrr..
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

struct particle *GLTestWindow::AllocParticleBall(int x0, int y0, int r, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{

	double x,y;
 
	
	sprintf(blah,"BALL %d,%d,%d,%.2f,%.3f,%.3f", x0, y0, r, partstep, Uk, Vk);
	particleslist->appendItem(blah);
	particleobjects+=1;


 

/*	if(r>width || r>height) r=min(width,height);*/
	if(r<0) r=0;
/*	if(x0-r<2) r=2;*/
/*	if(y0-r<2) r=2;*/
/*	if(x0+r>width) r=width-x0;*/
/*	if(y0+r>height) r=height-y0;*/
                        /* r=0 - cancel ball creating */

	/* make ball */
  /* x,y,r */

	for(x=x0-r;x<x0+r;x+=partstep)
	{
		for(y=y0-r;y<y0+r;y+=partstep)
		{
			if( sqrt( (double) abs(x0-x)*abs(x0-x) + (double) abs(y0-y)*abs(y0-y)) < r )
			{
				wrkpart=PlaceParticle(x,y,wrkpart,Uk,Vk,dx,dy,nx,ny,width,height,F,U,V);
			}
		}
  }

	return(wrkpart);
}

void GLTestWindow::NegAllocParticleBall(int x0, int y0, int r, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;
	struct particle *workpart;


	sprintf(blah,"!BALL %d,%d,%d,%.2f,%.3f,%.3f", x0, y0, r, partstep, Uk, Vk);
	particleslist->appendItem(blah);
	particleobjects+=1;



/*	if(r>width || r>height) r=min(width,height);*/
	if(r<0) r=0;
/*	if(x0-r<2) r=2;*/
/*	if(y0-r<2) r=2;*/
/*	if(x0+r>width) r=width-x0;*/
/*	if(y0+r>height) r=height-y0;*/
                        /* r=0 - cancel ball creating */

	/* make ball */
  /* x,y,r */

	for(workpart=wrkpart;workpart!=NULL;workpart=workpart->next)
	{

/*		printf("!\n");*/
			x=workpart->x;
			y=workpart->y;

/*			printf("%f,%f\n",x,y);*/

			if( sqrt( (double) abs(x0-x)*abs(x0-x) + (double) abs(y0-y)*abs(y0-y)) < r )
			{
				workpart->i = 0;
        workpart->j = 0;
        workpart->flag=P_AVAIL;
        workpart->x = 0;
        workpart->y = 0;
			}


  }

}


struct particle *GLTestWindow::ParticleFile(int argc, char *argv[], double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V,int verbose)
{
	FILE *fp;
	char workbuf[MAXWORD];

	int			x1,y1,x2,y2;
	double	step;
	double	uk,vk;

	struct particle *wrkpart;
	struct particle *partptr;



	workbuf[0]=0;

  if(argc==1)
  {
		printf("Option file not specified.\n");
		printf("USAGE: fluid <file.opt>\n");
		exit(0);
  }

  if(argc==2)
	{
	  if(strcmp(argv[1],"?")==0 || strcmp(argv[1],"-?")==0 || strcmp(argv[1],"h")==0 || strcmp(argv[1],"-h")==0 || strcmp(argv[1],"help")==0 || strcmp(argv[1],"-help")==0)
		{
			printf("USAGE: fluid <file.opt>\n");
			exit(0);
	  }

	  if(!(fp=fopen(argv[1],"r")))
		{
			printf("Unable to open %s file.\n",argv[1]);
			exit(1);
	  }
	}

  if(argc!=2 && argc!=1)
   {
		printf("Too many parametrs.\n");
		exit(1);
   }



  MoveNextWord(fp,"PARTICLES:");

	wrkpart=AllocOneParticle(0.0,0.0,P_AVAIL,0,0,0.0,0.0);
  partptr=wrkpart;

	x1=x2=y1=y2=step=0;
	uk=vk=0.0;

	printf("\n");

	while(strcmp(workbuf,"ENDPAR."))
	{
		mfscanf(fp,"%s",workbuf);

		if(!(strcmp(workbuf,"BOX")))
		{
			if(verbose)
			printf("BOX  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			ISetValue(fp,&y2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%d,%f,%f,%f\n",x1,y1,x2,y2,step,uk,vk);

			wrkpart=AllocParticleBox(x1 , y1 , x2 ,y2 ,step , uk,vk ,wrkpart,dx,dy,nx,ny,width,height,F,U,V);

    }


		if(!(strcmp(workbuf,"BALL")))
		{
			if(verbose)
			printf("BALL  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%f,%f,%f\n",x1,y1,x2,step,uk,vk);

			wrkpart=AllocParticleBall(x1 , y1 , x2 ,step , uk,vk ,wrkpart,dx,dy,nx,ny,width,height,F,U,V);

		}


		if(!(strcmp(workbuf,"!BALL")))
		{
			if(verbose)
			printf("!BALL  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%f,%f,%f\n",x1,y1,x2,step,uk,vk);

			NegAllocParticleBall(x1 , y1 , x2 ,step , uk,vk ,partptr,dx,dy,nx,ny,width,height,F,U,V);
		}


		if(!(strcmp(workbuf,"!BOX")))
		{
			if(verbose)
			printf("!BOX  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			ISetValue(fp,&y2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%d,%f,%f,%f\n",x1,y1,x2,y2,step,uk,vk);

			NegAllocParticleBox(x1 , y1 , x2 , y2,step , uk,vk ,partptr,dx,dy,nx,ny,width,height,F,U,V);
		}

  }



	fclose(fp);

	return(partptr);
}




int p=0;
int fi=0;

// Start simulation when START button on
long GLTestWindow::onChore(FXObject* a,FXSelector b,void* c){
	GLdouble windW = glcanvas->getWidth();
	GLdouble windH = glcanvas->getHeight();


	
		
//	do{

		
		
//		for (i = 0; i < ((int) ((dtmax / dt))); i++) {


if(fi<((int) ((dtmax / dt))))
{
	fi++;
 
 	pbar->setProgress(((fi+1)/(dtmax/dt))*100);
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
		    
			fi=0;
			dt /= 2.0;	  // one more time please...
			

			chore=getApp()->addChore(this,ID_CHORE);
			return 1;
		}
}
else // time step ende
{
	fi=0;
	dt *= 2.0;
	if (dt > dtmax)
	    dt = dtmax;
	frame+=1;
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
	  
	if(frame!=0)
	{
		sprintf(blah,"Frame:        %d",frame);
		framenr->setText(blah);	
	}

	Render();
	if(write) if(aviinit)
//		V_Write(frame);
	{
		posdemotxt-=1;
		if(posdemotxt<-20) posdemotxt=-20;


		MakeFrame(alpbi, 24, BITMAP_X, BITMAP_Y);

		hr = AVIStreamWrite(psCompressed,	// stream pointer
			(frame+pocztime+pocztime/2+fadetime),				// time of this frame
			1,				// number to write
			(LPBYTE) alpbi[0] +		// pointer to data
				alpbi[0]->biSize +
				alpbi[0]->biClrUsed * sizeof(RGBQUAD),
				alpbi[0]->biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
	 

	}
  drawScene();




}




  chore=getApp()->addChore(this,ID_CHORE);

  return 1;
  }


  long GLTestWindow::onCmdSaveIt(FXObject*,FXSelector,void*)
{
		getApp()->beginWaitCursor();

	if(write)
		V_Write(frame);

		getApp()->endWaitCursor();

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


// AVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVI

if(write)
{
	/* first let's make sure we are running on 1.1 */
	wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a){
		 /* oops, we are too old, blow out of here */
		MessageBox(NULL, "Video For Windows Version 1.1 or more required", "no avi output becousse...", MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	alpbi[0] = NULL;

	//
	// Set up the bitmaps for the file in an array
	//
	AVIFileInit();
	//
	// Open the movie file for writing....
	//
  //	LoadString(hInstance, IDS_FILENAME, szTitle, BUFSIZE );
	


	hr = AVIFileOpen(&pfile,		    // returned file pointer
		    //   "blah.avi",		            // file name
		    
			avinamebuf,
			
			OF_WRITE | OF_CREATE,	    // mode to open file with
		       NULL);			    // use handler determined
						    // from file extension....
	if (hr != AVIERR_OK)
		goto error;

	// Fill in the header for the video stream....

	// The video stream will run in 15ths of a second....


		MakeFrame(alpbi, 24, BITMAP_X, BITMAP_Y);
//goto t;



	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType                = streamtypeVIDEO;// stream type
	strhdr.fccHandler             = 0;
	strhdr.dwScale                = 1;
	strhdr.dwRate                 = avirate;		    // 15 fps
	strhdr.dwSuggestedBufferSize  = alpbi[0]->biSizeImage;
	SetRect(&strhdr.rcFrame, 0, 0,		    // rectangle for stream
	    (int) alpbi[0]->biWidth,
	    (int) alpbi[0]->biHeight);



	// And create the stream;
	hr = AVIFileCreateStream(pfile,		    // file pointer
			         &ps,		    // returned stream pointer
			         &strhdr);	    // stream header
	if (hr != AVIERR_OK) {
		goto error;
	}

	_fmemset(&opts, 0, sizeof(opts));

	if (!AVISaveOptions(NULL, 0, 1, &ps, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
	    goto error;

	hr = AVIMakeCompressedStream(&psCompressed, ps, &opts, NULL);
	if (hr != AVIERR_OK) {
		goto error;
	}

	hr = AVIStreamSetFormat(psCompressed, 0,
			       alpbi[0],	    // stream format
			       alpbi[0]->biSize +   // format size
			       alpbi[0]->biClrUsed * sizeof(RGBQUAD));
	if (hr != AVIERR_OK) {
	goto error;
	}

	// Fill in the stream header for the text stream....

	// The text stream is in 60ths of a second....

	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType                = streamtypeTEXT;
	strhdr.fccHandler             = mmioFOURCC('D', 'R', 'A', 'W');
	strhdr.dwScale                = 1;
	strhdr.dwRate                 = 60;
	strhdr.dwSuggestedBufferSize  = sizeof(szText);
	SetRect(&strhdr.rcFrame, 0, (int) alpbi[0]->biHeight,
	    (int) alpbi[0]->biWidth, (int) alpbi[0]->biHeight + TEXT_HEIGHT);

	// ....and create the stream.
	hr = AVIFileCreateStream(pfile, &psText, &strhdr);
	if (hr != AVIERR_OK) {
		goto error;
	}

	dwTextFormat = sizeof(dwTextFormat);
	hr = AVIStreamSetFormat(psText, 0, &dwTextFormat, sizeof(dwTextFormat));
	if (hr != AVIERR_OK) {
		goto error;
	}


	getApp()->beginWaitCursor();


// tu walimy poczateczek

	 for(i=1;i<39;i++)
	 {

		 MakeFramePocz(alpbi, 24, BITMAP_X, BITMAP_Y,i);

		hr = AVIStreamWrite(psCompressed,	// stream pointer
			i*(pocztime/(39)),				// time of this frame
			1,				// number to write
			(LPBYTE) alpbi[0] +		// pointer to data
				alpbi[0]->biSize +
				alpbi[0]->biClrUsed * sizeof(RGBQUAD),
				alpbi[0]->biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
	 }

 		 MakeFramePocz(alpbi, 24, BITMAP_X, BITMAP_Y,39);

		hr = AVIStreamWrite(psCompressed,	// stream pointer
			(pocztime/2),				// time of this frame
			1,				// number to write
			(LPBYTE) alpbi[0] +		// pointer to data
				alpbi[0]->biSize +
				alpbi[0]->biClrUsed * sizeof(RGBQUAD),
				alpbi[0]->biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);


// fade 1 klatki

		eps=0;

	for(i=1;i<39;i++)
	 {
		 eps=(float)i/(float)39;
 		MakeFrameFade(alpbi, 24, BITMAP_X, BITMAP_Y, eps);

		hr = AVIStreamWrite(psCompressed,	// stream pointer
			(pocztime+pocztime/2)+i*(fadetime/39),				// time of this frame
			1,				// number to write
			(LPBYTE) alpbi[0] +		// pointer to data
				alpbi[0]->biSize +
				alpbi[0]->biClrUsed * sizeof(RGBQUAD),
				alpbi[0]->biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
	 }
		



	getApp()->endWaitCursor();



}

		aviinit=TRUE;
goto t;
error:
		MessageBox(NULL, "Avi init Failed!", "f", MB_OK|MB_ICONSTOP);
		aviinit=FALSE;
t:
if(!write)
	aviinit=FALSE;





// AVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVIAVI

	sprintf(blah,"Please Wait A Little..");
	framenr->setText(blah);	

   return 1;
  }

//
// Fill an array of LPBI's with the frames for this movie
//



static void MakeFrame(LPLPBI alpbi, UINT bits, UINT wXSize,UINT wYSize )
{
	HDC         hdc ;
	HDC         hdcMem ;
	HBITMAP     hbitmap,hbitmapOld ;
	HPEN        hpen3,hpen1,hpenwhite,hpenOld ;
	HFONT       hfont,hfontOld ;
	HBRUSH      hbrush,hbrushOld ;
	RECT        rc ;
	RECT        rcFrameNo ;
	int         cxPixInch ;
	int         cyPixInch ;
	int         cxPixels ;
	int         cyPixels ;
	int         radius ;
	int         x0,y0,x1,y1 ;
	int         i=0,j ;
	char        szNumber[123] ;

	//
	// Make sure our resources are freed
	//
	FreeFrame(alpbi);

	hdc = GetDC(NULL) ;
	hdcMem = CreateCompatibleDC(NULL) ;


	hbitmap = CreateCompatibleBitmap(hdc,wXSize,wYSize) ;

	SetRect( &rcFrameNo,0,0,wXSize,wYSize);


//	for ( i=0; i<N_FRAMES; i++ ) {
		hbitmapOld = SelectBitmap(hdcMem, hbitmap);
	
	  
	 	for(y1=0;y1<200;y1++)
		 	for(x1=0;x1<320;x1++)
			   SetPixel(hdcMem,x1,y1,RGB(V_Red(y1,x1),V_Green(y1,x1),V_Blue(y1,x1)));


	  wsprintf(szNumber,"Demo version.") ;

		SetBkColor(hdcMem,RGB(128,128,128)) ;
		SetTextColor(hdcMem,RGB(255,0,0)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left+40,
			rcFrameNo.bottom+posdemotxt,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			13,
			NULL);


	wsprintf(szNumber,"Frame: %d     ",frame);
																		   
	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(225,215,215)) ;
	ExtTextOut(
			hdcMem,
			-posdemotxt*2,
			4,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			11,
			NULL);


/*
	
		wsprintf(szNumber,"http://panoramix.ift.uni.wroc.pl/~maq");

		SetBkColor(hdcMem,RGB(128,128,128)) ;
		SetTextColor(hdcMem,RGB(255,255,255)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left+10,
			rcFrameNo.bottom-20,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			37,
			NULL);
*/



		SelectBitmap(hdcMem, hbitmapOld);

		alpbi[0] = (LPBITMAPINFOHEADER)GlobalLock(MakeDib(hbitmap, bits));

	  //	if (alpbi[i] == NULL && i )
	   //		alpbi[i] = alpbi[i-1] ;
//	}

	DeleteBitmap(hbitmap) ;
	DeleteObject(hdcMem) ;
	ReleaseDC(NULL,hdc) ;
}


static void MakeFrameFade(LPLPBI alpbi, UINT bits, UINT wXSize,UINT wYSize,float eps )
{
	HDC         hdc ;
	HDC         hdcMem ;
	HBITMAP     hbitmap,hbitmapOld ;
	HPEN        hpen3,hpen1,hpenwhite,hpenOld ;
	HFONT       hfont,hfontOld ;
	HBRUSH      hbrush,hbrushOld ;
	RECT        rc ;
	RECT        rcFrameNo ;
	int         cxPixInch ;
	int         cyPixInch ;
	int         cxPixels ;
	int         cyPixels ;
	int         radius ;
	int         x0,y0,x1,y1 ;
	int         i=0,j ;
	char        szNumber[123] ;

	//
	// Make sure our resources are freed
	//
	FreeFrame(alpbi);

	hdc = GetDC(NULL) ;
	hdcMem = CreateCompatibleDC(NULL) ;


	hbitmap = CreateCompatibleBitmap(hdc,wXSize,wYSize) ;

	SetRect( &rcFrameNo,0,0,wXSize,wYSize);


//	for ( i=0; i<N_FRAMES; i++ ) {
		hbitmapOld = SelectBitmap(hdcMem, hbitmap);
	
	  
	 	for(y1=0;y1<200;y1++)
		 	for(x1=0;x1<320;x1++)
			   SetPixel(hdcMem,x1,y1,RGB(V_Red(y1,x1)*eps,V_Green(y1,x1)*eps,V_Blue(y1,x1)*eps));



			/*
	  wsprintf(szNumber,"Demo version.") ;

		SetBkColor(hdcMem,RGB(128*eps,128*eps,128*eps)) ;
		SetTextColor(hdcMem,RGB(255*eps,0,0)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left+40,
			rcFrameNo.bottom-20,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			13,
			NULL);


	wsprintf(szNumber,"Frame: %d     ",frame);
																		   
	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(225*eps,215*eps,215*eps)) ;
	ExtTextOut(
			hdcMem,
			0,
			0,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			11,
			NULL);
*/

/*
	
		wsprintf(szNumber,"http://panoramix.ift.uni.wroc.pl/~maq");

		SetBkColor(hdcMem,RGB(128,128,128)) ;
		SetTextColor(hdcMem,RGB(255,255,255)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left+10,
			rcFrameNo.bottom-20,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			37,
			NULL);
*/



		SelectBitmap(hdcMem, hbitmapOld);

		alpbi[0] = (LPBITMAPINFOHEADER)GlobalLock(MakeDib(hbitmap, bits));

	  //	if (alpbi[i] == NULL && i )
	   //		alpbi[i] = alpbi[i-1] ;
//	}

	DeleteBitmap(hbitmap) ;
	DeleteObject(hdcMem) ;
	ReleaseDC(NULL,hdc) ;
}



static void MakeFramePocz(LPLPBI alpbi, UINT bits, UINT wXSize,UINT wYSize,int how)
{
	HDC         hdc ;
	HDC         hdcMem ;
	HBITMAP     hbitmap,hbitmapOld ;
	HPEN        hpen3,hpen1,hpenwhite,hpenOld ;
	HFONT       hfont,hfontOld ;
	HBRUSH      hbrush,hbrushOld ;
	RECT        rc ;
	RECT        rcFrameNo ;
	int         cxPixInch ;
	int         cyPixInch ;
	int         cxPixels ;
	int         cyPixels ;
	int         radius ;
	int         x0,y0,x1,y1 ;
	int         i=0,j ;
	char        szNumber[123] ;


	//
	// Make sure our resources are freed
	//
	FreeFrame(alpbi);

	hdc = GetDC(NULL) ;
	hdcMem = CreateCompatibleDC(NULL) ;


	hbitmap = CreateCompatibleBitmap(hdc,wXSize,wYSize) ;

	SetRect( &rcFrameNo,0,0,wXSize,wYSize);


//	for ( i=0; i<N_FRAMES; i++ ) {
		hbitmapOld = SelectBitmap(hdcMem, hbitmap);
	
		SetRect(&rc,0,0,wXSize,wYSize) ;
		FillRect(hdcMem,&rc,GetStockBrush(BLACK_BRUSH));	  


  wsprintf(szNumber,  "Anim created with Fluid v1.3.                                          ");
													 
	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(255,255,255)) ;
	ExtTextOut(
			hdcMem,
			fwidth/8,
			fheight/4,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			how,
			NULL);

  wsprintf(szNumber,  "'Fluid v1.3'available at:                                              ");

	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(255,255,255)) ;
	ExtTextOut(
			hdcMem,
			fwidth/8,
			fheight/4+40,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber+(38-how),
			how,
			NULL);

	wsprintf(szNumber," http://panoramix.ift.uni.wroc.pl/~maq                                 ");

	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(225,215,215)) ;
	ExtTextOut(
			hdcMem,
			fwidth/8,
			fheight/4+60,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber+(38-how),
			how,
			NULL);

	wsprintf(szNumber,"author: maq@panoramix.ift.uni.wroc.pl                                  ");
																		   
	SetBkColor(hdcMem,RGB(0,0,0)) ;
	SetTextColor(hdcMem,RGB(225,215,215)) ;
	ExtTextOut(
			hdcMem,
			fwidth/8,
			fheight/4+80,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber+(38-how),
			how,
			NULL);



/*
	
		wsprintf(szNumber,"http://panoramix.ift.uni.wroc.pl/~maq");

		SetBkColor(hdcMem,RGB(128,128,128)) ;
		SetTextColor(hdcMem,RGB(255,255,255)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left+10,
			rcFrameNo.bottom-20,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			37,
			NULL);
*/



		SelectBitmap(hdcMem, hbitmapOld);

		alpbi[0] = (LPBITMAPINFOHEADER)GlobalLock(MakeDib(hbitmap, bits));

	  //	if (alpbi[i] == NULL && i )
	   //		alpbi[i] = alpbi[i-1] ;
//	}

	DeleteBitmap(hbitmap) ;
	DeleteObject(hdcMem) ;
	ReleaseDC(NULL,hdc) ;
}



//
// Walk through our array of LPBI's and free them
//
static void FreeFrame(LPLPBI alpbi)
{
	UINT        w ;

	if (!alpbi[0])
		return ;
	GlobalFreePtr(alpbi[0]);
}

/*
** MakeDib(hbitmap)
**
** Take the given bitmap and transform it into a DIB with parameters:
**
** BitsPerPixel:    8
** Colors:          palette
**
*/
static HANDLE  MakeDib( HBITMAP hbitmap, UINT bits )
{
	HANDLE              hdib ;
	HDC                 hdc ;
	BITMAP              bitmap ;
	UINT                wLineLen ;
	DWORD               dwSize ;
	DWORD               wColSize ;
	LPBITMAPINFOHEADER  lpbi ;
	LPBYTE              lpBits ;
	
	GetObject(hbitmap,sizeof(BITMAP),&bitmap) ;

	//
	// DWORD align the width of the DIB
	// Figure out the size of the colour table
	// Calculate the size of the DIB
	//
	wLineLen = (bitmap.bmWidth*bits+31)/32 * 4;
	wColSize = sizeof(RGBQUAD)*((bits <= 8) ? 1<<bits : 0);
	dwSize = sizeof(BITMAPINFOHEADER) + wColSize +
		(DWORD)(UINT)wLineLen*(DWORD)(UINT)bitmap.bmHeight;

	//
	// Allocate room for a DIB and set the LPBI fields
	//
	hdib = GlobalAlloc(GHND,dwSize);
	if (!hdib)
		return hdib ;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib) ;

	lpbi->biSize = sizeof(BITMAPINFOHEADER) ;
	lpbi->biWidth = bitmap.bmWidth ;
	lpbi->biHeight = bitmap.bmHeight ;
	lpbi->biPlanes = 1 ;
	lpbi->biBitCount = (WORD) bits ;
	lpbi->biCompression = BI_RGB ;
	lpbi->biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize ;
	lpbi->biXPelsPerMeter = 0 ;
	lpbi->biYPelsPerMeter = 0 ;
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;
	lpbi->biClrImportant = 0 ;

	//
	// Get the bits from the bitmap and stuff them after the LPBI
	//
	lpBits = (LPBYTE)(lpbi+1)+wColSize ;

	hdc = CreateCompatibleDC(NULL) ;

	GetDIBits(hdc,hbitmap,0,bitmap.bmHeight,lpBits,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	// Fix this if GetDIBits messed it up....
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;

	DeleteDC(hdc) ;
	GlobalUnlock(hdib);

	return hdib ;
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

	if(write)
		T_AVINAME->enable();
	else
		T_AVINAME->disable();



return 1;
  }


long GLTestWindow::onCmdAbout(FXObject*,FXSelector,void*){

FXMessageBox about(this,"Fluid Navier-Stokes Solver.",FXStringFormat("DEMO OF \n FLUID V1.3 \n\n The MAC Method      \n Navier-Stokes, Incompressible,\n Viscous, Free Surface solver,\n\n Copyright (C) 2001, Maciej Matyka\n (maq@panoramix.ift.uni.wroc.pl)\n\n If you wanna register program 6$\n \n full source codes 10$ (US):\n- source code for linux console version (pure ansi c)\n- source code for windows with GUI created with FOX library (c++) \n\nMaciej Matyka (BSK SA) SWIFT: SKAT PL PK  10501575 – 2245341900 \n\n or better by postal order to my home adress: \n\nMaciej Matyka\nul. Kielczowska 137/10\n51-315 Wroclaw\nPoland\n\n When i receive money - you will get fluid .zip package with program or source code \n\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL),0,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
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

	


long GLTestWindow::onCmdAVINAME(FXObject* a,FXSelector b,void* c){

	//	sprintf(avinamebuf,"%s",(T_AVINAME->getText()).text);

	FXString filename;

	filename=T_AVINAME->getText();


	for(i=0;i<99;i++)
		avinamebuf[i]=0;

	for(i=0;i<filename.length();i++)
	{
		avinamebuf[i]=filename[i];
	}



	return 1;
}
long GLTestWindow::onUpdAVINAME(FXObject* a,FXSelector b,void* c){

	//	sprintf(avinamebuf,"%s",(T_AVINAME->getText()).text);

	
	return 1;
}



long GLTestWindow::onCmdAVIRATE(FXObject* a,FXSelector b,void* c){
	double myrate;


  myrate=FXDoubleVal(T_AVIRATE->getText());	

  if((int)myrate>0 && (int)myrate<100)
	  avirate=(int)myrate;
  else
  {
	 sprintf(blah,"%d",avirate);
	 T_AVIRATE->setText(blah); 
  }	



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
  	dtstart=dtmax=dt;


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



long GLTestWindow::onCmdPartConfig(FXObject*,FXSelector,void*)
{

//	wrkpart->object=particleobjects;

	Render();
	PlotParticlesColorObject(particle1,fwidth,fheight,nx,ny,234,234,243,(particleslist->getCurrentItem())-1);

	
	drawScene();
 	
	return 1;
}



long GLTestWindow::onCmdRemoveParticleObject(FXObject*,FXSelector,void*)
{
	struct particle *workpart;

	getApp()->beginWaitCursor();

	for(workpart=particle1;workpart!=NULL;workpart=workpart->next)
	{
		if((particleslist->getCurrentItem())-1==workpart->object)
		{
			workpart->flag=P_AVAIL;
			workpart->x=0;
			workpart->y=0;
			workpart->i=0;
			workpart->j=0;
			workpart->object=0;
	 	}
	}

	CopyParticles(particle1,_particle1);

		for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			if(!( (Flag[i][j] & C_BND) || (Flag[i][j] & C_EMPBND) ))
					ReflagTo(Flag,i,j,C_EMP);

//    ReflagCells(Vn1, Flag, P, U, V, nx, ny, particle1);
    timestep(dx, dy, gx, gy, vis, t, dt, nx, ny, imax,
			     jmax, width, height, Flag, Un1, Vn1, U, V, P,
			     D, R, sta);


	Render();
	drawScene();

	getApp()->endWaitCursor();

	return 1;
}



long GLTestWindow::onCmdRemoveParticles(FXObject*,FXSelector,void*)
{

	for(i=0;i<particleobjects;i++)
	particleslist->removeItem(2);
	particleobjects=0;

	return 1;
}
 




FXString file;
 



long GLTestWindow::onCmdReLoad(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  int argc = 2;
  char *argv[3];
  
  
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


	for(i=0;i<particleobjects;i++)
	particleslist->removeItem(2);
	particleobjects=0;

	particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);
	_particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);
	
	particleobjects/=2;

	for(i=0;i<particleobjects;i++)
	particleslist->removeItem(2);

	

   how_much_particles=0;

 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);
   
   for(wrkpartile = particle1; wrkpartile != NULL; wrkpartile = wrkpartile->next) 
    how_much_particles+=1; 
	
 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);

		for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			if(!( (Flag[i][j] & C_BND) || (Flag[i][j] & C_EMPBND) ))
					ReflagTo(Flag,i,j,C_EMP);

  CalculateBNDConditions(Un1, Vn1, Flag, U, P, V, D, nx, ny, gx, gy, vis, dx, dy);

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




return 1;
  }


long GLTestWindow::onCmdLoad(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
 FXString pattern = "*.opt";
  FXFileDialog opendialog(this,"Open .opt file.");
  opendialog.setSelectMode(SELECTFILE_EXISTING);
	FXString directory = "opt/";
	int argc = 2;
  char *argv[3];
  
  
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

	getApp()->beginWaitCursor();



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


	for(i=0;i<particleobjects;i++)
	particleslist->removeItem(2);
	particleobjects=0;

	particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);
	_particle1=ParticleFile(argc,argv,dx,dy,nx,ny,fwidth,fheight,Flag,U,V,1);


	particleobjects/=2;
	for(i=0;i<particleobjects;i++)
	particleslist->removeItem(2);

	

   how_much_particles=0;

 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);
   
   for(wrkpartile = particle1; wrkpartile != NULL; wrkpartile = wrkpartile->next) 
    how_much_particles+=1; 
	
 sprintf(blah,"Particles: %d",how_much_particles);
 L_PARTICLES->setText(blah);

		for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			if(!( (Flag[i][j] & C_BND) || (Flag[i][j] & C_EMPBND) ))
					ReflagTo(Flag,i,j,C_EMP);

  CalculateBNDConditions(Un1, Vn1, Flag, U, P, V, D, nx, ny, gx, gy, vis, dx, dy);

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



getApp()->endWaitCursor();


  }

return 1;
  }

long GLTestWindow::onUpdLoad(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
   spinning ? button->disable() : button->enable(); 
   return 1;
  }

long GLTestWindow::onUpdReLoad(FXObject* sender,FXSelector,void*){
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

	aviinit=FALSE;

	if (ps)
		AVIStreamClose(ps);

	if (psCompressed)
		AVIStreamClose(psCompressed);

	if (psText)
		AVIStreamClose(psText);

	if (pfile)
		AVIFileClose(pfile);

	AVIFileExit();
	FreeFrame(alpbi);

	
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


