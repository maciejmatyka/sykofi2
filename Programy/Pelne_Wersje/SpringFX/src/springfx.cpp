
#include <string.h>
#include <math.h>
#include <gl/glaux.h>
#include "fx.h"
#include "fx3d.h"
  
#define PX 0
#define PY 1
#define PZ 2

#define C 0.3

#define K1 0.4
#define K2 0.4
#define K3 0.4
#define K4 0.4

#define ballsize 90

#define CI_RED AUX_RED 
#define CI_ANTI_ALIAS_GREEN 16 
#define CI_ANTI_ALIAS_YELLOW 32 
#define CI_ANTI_ALIAS_RED 48  


// global datas

char blah[100];
float point[3] = { 0.0, 0.0, 0.0 };   
float t,dt,x,y,dx,dy,vx,vy,dvx,dvy,m;
float g=-9.8;
int k1=FALSE,k2=FALSE,k3=FALSE,k4=FALSE,c=FALSE,gravity=FALSE; // c -opor
GLint size; 
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };	
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLenum rgb, doubleBuffer, windType; 
GLenum mode; 


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
  FXTextField* poptextx;
  FXTextField* poptexty;
  FXTextField* poptextvx;
  FXTextField* poptextvy;
  FXTextField* gravitytext;


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
	ID_SPRING1,
	ID_SPRING2,
	ID_SPRING3,
	ID_SPRING4,
	ID_GRAVITY,
	ID_FORCEV,
	ID_ARROWUP,
	ID_ARROWDOWN,
	ID_ARROWLEFT,
	ID_ARROWRIGHT,
	ID_GRAVITYSET

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

  long onSpring1(FXObject*,FXSelector,void*);
  long onSpring2(FXObject*,FXSelector,void*);
  long onSpring3(FXObject*,FXSelector,void*);
  long onSpring4(FXObject*,FXSelector,void*);

  long onGravity(FXObject*,FXSelector,void*);
  long onForcev(FXObject*,FXSelector,void*);

  long onArrowup(FXObject*,FXSelector,void*);
  long onArrowdown(FXObject*,FXSelector,void*);
  long onArrowleft(FXObject*,FXSelector,void*);
  long onArrowright(FXObject*,FXSelector,void*);
  long onGravitySet(FXObject*,FXSelector,void*);
  
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

  FXMAPFUNC(SEL_TIMEOUT,       GLTestWindow::ID_TIMEOUT,  GLTestWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,         GLTestWindow::ID_CHORE,    GLTestWindow::onChore),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPRING1,  GLTestWindow::onSpring1),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPRING2,  GLTestWindow::onSpring2),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPRING3,  GLTestWindow::onSpring3),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_SPRING4,  GLTestWindow::onSpring4),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GRAVITY,  GLTestWindow::onGravity),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_FORCEV,   GLTestWindow::onForcev),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_ARROWUP,  GLTestWindow::onArrowup),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_ARROWDOWN,  GLTestWindow::onArrowdown),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_ARROWLEFT,  GLTestWindow::onArrowleft),
  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_ARROWRIGHT,  GLTestWindow::onArrowright),

  FXMAPFUNC(SEL_COMMAND,       GLTestWindow::ID_GRAVITYSET,  GLTestWindow::onGravitySet),




  };



// Macro for the GLTestApp class hierarchy implementation
FXIMPLEMENT(GLTestWindow,FXMainWindow,GLTestWindowMap,ARRAYNUMBER(GLTestWindowMap))





// Construct a GLTestApp
GLTestWindow::GLTestWindow(FXApp* a):FXMainWindow(a,"Spring 1.0",NULL,NULL,DECOR_ALL,0,0,800,600){
  FXVerticalFrame *glcanvasFrame;
  FXVerticalFrame *buttonFrame;
  FXComposite *glpanel;
  FXHorizontalFrame *frame;
  
  frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  
  // LEFT pane to contain the glcanvas
  glcanvasFrame=new FXVerticalFrame(frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
  
  // Label above the glcanvas               
  new FXLabel(glcanvasFrame,"Spring1.0 Output View.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  
  // Horizontal divider line
  new FXHorizontalSeparator(glcanvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  
  // Drawing glcanvas
  glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);
  
  // A Visual to drag OpenGL
  glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
  
  // Drawing glcanvas
  glcanvas=new FXGLCanvas(glpanel,glvisual,this,ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

  // RIGHT pane for the buttons
  buttonFrame=new FXVerticalFrame(frame,LAYOUT_FILL_Y|FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);

  // Label above the buttons  
  new FXLabel(buttonFrame,"Realize your dreams...",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  
  // Horizontal divider line
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);
  
  // Button to print
  //new FXButton(buttonFrame,"Spin &Timer\tSpin using interval timers\nNote the app blocks until the interal has elapsed...",NULL,this,ID_SPIN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  new FXButton(buttonFrame,"Start &simulation.\tStart simulation.\n",NULL,this,ID_SPINFAST,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  
  // Button to print
  new FXButton(buttonFrame,"&Pause\tPause simulation.",NULL,this,ID_STOP,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  
  // Exit button
  new FXButton(buttonFrame,"&Quit\tExit Spring.",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // options

  new FXLabel(buttonFrame,"Select world properties.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  
  new FXLabel(buttonFrame,"Springs.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  new FXCheckButton(buttonFrame,"Spring 1",this,ID_SPRING1,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
  new FXCheckButton(buttonFrame,"Spring 2",this,ID_SPRING2,ICON_BEFORE_TEXT|LAYOUT_SIDE_RIGHT);
  new FXCheckButton(buttonFrame,"Spring 3",this,ID_SPRING3,ICON_BEFORE_TEXT|LAYOUT_LEFT);
  new FXCheckButton(buttonFrame,"Spring 4",this,ID_SPRING4,ICON_BEFORE_TEXT|LAYOUT_LEFT);


  new FXLabel(buttonFrame,"Forces.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  new FXCheckButton(buttonFrame,"Gravity",this,ID_GRAVITY,ICON_BEFORE_TEXT|LAYOUT_LEFT);
  new FXCheckButton(buttonFrame,"Force ~v",this,ID_FORCEV,ICON_BEFORE_TEXT|LAYOUT_LEFT);

  new FXLabel(buttonFrame,"Force ball.",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);


  new FXArrowButton(buttonFrame,this,ID_ARROWUP,JUSTIFY_CENTER_X|ARROW_UP|FRAME_RAISED);
  new FXArrowButton(buttonFrame,this,ID_ARROWDOWN,JUSTIFY_CENTER_X|ARROW_DOWN|FRAME_RAISED);
  new FXArrowButton(buttonFrame,this,ID_ARROWLEFT,JUSTIFY_CENTER_X|ARROW_LEFT|FRAME_RAISED);
  new FXArrowButton(buttonFrame,this,ID_ARROWRIGHT,JUSTIFY_CENTER_X|ARROW_RIGHT|FRAME_RAISED);


	poptextx=new FXTextField(buttonFrame,7,NULL,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP,0,0,0,0);
	poptexty=new FXTextField(buttonFrame,7,NULL,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP,0,0,0,0);
	poptextvx=new FXTextField(buttonFrame,7,NULL,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP,0,0,0,0);
	poptextvy=new FXTextField(buttonFrame,7,NULL,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP,0,0,0,0);

  	poptextx->setText("0");	
	poptexty->setText("0");	
	poptextvx->setText("0");	
	poptextvy->setText("0");	


	new FXLabel(buttonFrame,"Gravity",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

	gravitytext=new FXTextField(buttonFrame,7,this,ID_GRAVITYSET,FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP,0,0,0,0);
	sprintf(blah,"%4.2f",g);
	gravitytext->setText(blah);	
 
// Make a tooltip
//  new FXTooltip(getApp());
  
  // Initialize private variables
  spinning=0;    
  timer = NULL;
  angle = 0.;
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


long GLTestWindow::onSpring1(FXObject*,FXSelector,void*){
	k1=!k1;
	GLTestWindow::drawScene();
	return 1;
  }

long GLTestWindow::onSpring2(FXObject*,FXSelector,void*){
  k2=!k2;
  GLTestWindow::drawScene();
  return 1;
  }


long GLTestWindow::onSpring3(FXObject*,FXSelector,void*){
  k3=!k3;
  GLTestWindow::drawScene();
  return 1;
  }


long GLTestWindow::onSpring4(FXObject*,FXSelector,void*){
  k4=!k4;
  GLTestWindow::drawScene();
  return 1;
  }


long GLTestWindow::onGravity(FXObject*,FXSelector,void*){

  gravity=!gravity;
  GLTestWindow::drawScene();
  return 1;
  }


long GLTestWindow::onForcev(FXObject*,FXSelector,void*){
  c=!c;
  GLTestWindow::drawScene();
  return 1;
  }



long GLTestWindow::onArrowup(FXObject*,FXSelector,void*){
  vy=20; 	
  
  return 1;
  }

long GLTestWindow::onArrowdown(FXObject*,FXSelector,void*){
  vy=-20; 	
  return 1;
  }

long GLTestWindow::onArrowright(FXObject*,FXSelector,void*){
  vx=20; 	
  return 1;
  }

long GLTestWindow::onArrowleft(FXObject*,FXSelector,void*){
  vx=-20; 	
  return 1;
  }



long GLTestWindow::onGravitySet(FXObject*,FXSelector,void*){

  g=FXDoubleVal(gravitytext->getText());	

 	
  return 1;
  }

 



// Start simulation when START button on
long GLTestWindow::onChore(FXObject*,FXSelector,void*){

  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();


  if(gravity)
		dvy=g;
  else
		dvy=0;
  
 
  if(c)
		dvy-=C/m*vy;
		
	if(k1)
		dvy-=(K1/m)*(y-windH/2);
	
	if(k2)
		dvy-=(K2/m)*(y+windH/2);
	
	dvy*=dt;

	vy += dvy;
	dy  = vy*dt;
	y  = y + dy;


	if(c)
		dvx=(-C/m*vx);
	else
		dvx=0;

	if(k3)
		dvx-=(K3/m)*(x-windW/2);
	
	if(k4)
		dvx-=(K4/m)*(x+windW/2);
	
	dvx*=dt;

	vx += dvx;
	dx  = vx*dt;
	x  = x + dx;

	// Bouncy

	if( x< (-windW/2+ballsize))
	{
		x=(-windW/2+ballsize);
		vx = -vx;
	}


	 
	if( x> (windW/2-ballsize))
	{
		x=(windW/2-ballsize);
		vx = -vx;
	}




	if( y< (-windH/2+ballsize))
	{
		y=(-windH/2+ballsize);
		vy = -vy;
	}


	 
	if( y> (windH/2-ballsize))
	{
		y=(windH/2-ballsize);
		vy = -vy;
	}





 	point[PY]=y;
	point[PX]=x;

	light_position[PX]=-point[PX]/126;
	light_position[PY]=-point[PY]/126;

	sprintf(blah,"vx: %4.2f",vx);
	poptextvx->setText(blah);	

	sprintf(blah,"vy: %4.2f",vy);
	poptextvy->setText(blah);	

	sprintf(blah,"x:  %4.2f",x);
	poptextx->setText(blah);	

	sprintf(blah,"y:  %4.2f",y);
	poptexty->setText(blah);	



  drawScene();
  chore=getApp()->addChore(this,ID_CHORE);

  return 1;
  }


// Start the boxes spinning
long GLTestWindow::onCmdSpin(FXObject*,FXSelector,void*){
  spinning=1;
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }



// Enable or disable the spin button
long GLTestWindow::onUpdSpin(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }


// Start the boxes spinning
long GLTestWindow::onCmdSpinFast(FXObject*,FXSelector,void*){
  spinning=1;
  chore=getApp()->addChore(this,ID_CHORE);
  return 1;
  }


// Enable or disable the spin button
long GLTestWindow::onUpdSpinFast(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }


// If boxes are spinning, stop them
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
  spinning ? button->enable() : button->disable(); 
  return 1;
  }






// Draw the GL scene
void GLTestWindow::drawScene(){
  const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
  const GLfloat lightDiffuse[]={.4f,.9f,.4f,1.};
   const GLfloat blueMaterial[]={1.0,0.4,1.0,1.0};


//int ix,iy;

  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();

  

  glcanvas->makeCurrent();
  glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

	AUX_SETCOLOR(windType, AUX_WHITE);     


	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(size);     

   	glEnable(GL_BLEND);     
   	glEnable(GL_POINT_SMOOTH);     

  //	(rgb) ? glColor3f(0.5, 1.0, 0.5) : glIndexf(CI_ANTI_ALIAS_RED);     

	glMatrixMode(GL_PROJECTION);     
	glLoadIdentity();     
	gluOrtho2D(-windW/2, windW/2, -windH/2, windH/2);     
	glMatrixMode(GL_MODELVIEW); 
	
	glDisable(GL_LIGHTING); 
	glDisable(GL_DEPTH_TEST);

	glLineWidth(4.0);
 
  if(k1)
  {
	glBegin(GL_LINE_STRIP);     
		glVertex2f(0, windH/2);     
		glVertex2f(point[PX], point[PY]);     
	glEnd();     
  }

  if(k2)
  {
	  glBegin(GL_LINE_STRIP);     
		glVertex2f(0, -windH/2);     
		glVertex2f(point[PX], point[PY]);     
	glEnd();     
  }


  if(k3)
  {
	glBegin(GL_LINE_STRIP);     
		glVertex2f(windW/2, 0);     
		glVertex2f(point[PX], point[PY]);     
	glEnd();     
  }

  if(k4)
  {
	glBegin(GL_LINE_STRIP);     
		glVertex2f(-windW/2, 0);     
		glVertex2f(point[PX], point[PY]);     
	glEnd();     
  }

	glEnable(GL_LIGHTING); 
	glEnable(GL_DEPTH_TEST);

  	glMatrixMode(GL_PROJECTION);   
	glLoadIdentity();   

	if (windW <= windH)    
		glOrtho (-1.5, 1.5, -1.5*(GLfloat)windH/(GLfloat)windW,1.5*(GLfloat)windH/(GLfloat)windW, -10.0, 10.0); 
	else      
		glOrtho (-1.5*(GLfloat)windW/(GLfloat)windH,          1.5*(GLfloat)windW/(GLfloat)windH, -1.5, 1.5, -10.0, 10.0);  


	glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());


	
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);



	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();  
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     

	glPushMatrix();           
	glTranslatef((point[PX]/256)*1.5,
	   (point[PY]/256)*1.5,0.0);
	auxSolidSphere(m/16);	
	glPopMatrix();        

	


 
  // Swap if it is double-buffered
  if(glvisual->isDoubleBuffer()){
    glcanvas->swapBuffers();
    }
  
  // Make context non-current
  glcanvas->makeNonCurrent();
  }



  void glinit(void)
{
     

/*	
	static GLfloat whiteAmbient[] = {0.3, 0.3, 0.3, 1.0}; 
	static GLfloat redAmbient[] = {0.3, 0.1, 0.1, 1.0};  
	static GLfloat greenAmbient[] = {0.1, 0.3, 0.1, 1.0}; 
	static GLfloat blueAmbient[] = {0.1, 0.1, 0.3, 1.0};   
	static GLfloat whiteDiffuse[] = {1.0, 1.0, 1.0, 1.0};  
	static GLfloat redDiffuse[] = {1.0, 0.0, 0.0, 1.0};    
	static GLfloat greenDiffuse[] = {0.0, 1.0, 0.0, 1.0};  
	static GLfloat blueDiffuse[] = {0.4, 0.0, 1.0, 1.0};    
	static GLfloat whiteSpecular[] = {1.0, 1.0, 1.0, 1.0};  
	static GLfloat redSpecular[] = {1.0, 0.0, 0.0, 1.0};    
	static GLfloat greenSpecular[] = {0.0, 1.0, 0.0, 1.0};  
	static GLfloat blueSpecular[] = {0.0, 0.0, 1.0, 1.0};  
 
	
	GLfloat mat_shininess[] = { 50.0 };    
	

	
	windType |= (rgb) ? AUX_RGB : AUX_INDEX;     
	windType |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE; 
	windType |= (AUX_RGB | AUX_DEPTH16);
	auxInitDisplayMode(windType);      
 */


	glClearColor(0.0, 0.0, 0.0, 0.0);      
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);      

/*
	if (!rgb) 
		{    
			for (i = 0; i < 16; i++) 
			{         
				auxSetOneColor(i+CI_ANTI_ALIAS_RED, i/15.0, 0.0, 0.0);         
				auxSetOneColor(i+CI_ANTI_ALIAS_YELLOW, i/15.0, i/15.0, 0.0);         
				auxSetOneColor(i+CI_ANTI_ALIAS_GREEN, 0.0, i/15.0, 0.0);     
			}     
		}      
*/ 		
	
	mode = GL_FALSE;     
	size = 10; 

	t=x=y=dx=dy=vx=vy=dvx=dvy=0.0;
 	dt=(float)0.1;
 
	m=5;
	vx=0;

	light_position[PX]=-point[PX]/126;
	light_position[PY]=-point[PY]/126;

   
}




// Here we begin

 
int main(int argc,char *argv[]){

  // Make application
  FXApp application("GLTest","FoxTest");
  
  // Open the display
  application.init(argc,argv);

  // Make window
  new GLTestWindow(&application);

  // Create the application's windows
  application.create();
  
  // init gl

  glinit();

  // Run the application  
  
  return application.run();
  }


