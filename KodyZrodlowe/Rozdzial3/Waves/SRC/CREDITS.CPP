


#include <windows.h>
#include <GL/GL.h>

#include <fx.h>
#include <fx3d.h>

#include "credits.h"



const FXuint TIMER_INTERVAL = 10;
const FXchar* developers[]= {
                            "                            ",
                             "                            ", 
                             "\t\tWaves v1.0                  ",  
                             "\t2d wave equation solver     ",
                             "\t-----------------------     ",
                             "                            ",
                             "\tprogramming:                ",
                             "\t\t\tMaciej Matyka               ",
                             "                            ",
                             "  contact:                  ",
                             "                            ",
                             "    Maciej Matyka                        ",
                             "    ul. Kielczowska 137/10  ",
                             "    51-315 Wroclaw          ",
                             "    Poland                  ",
                             "                            ",
                             "mailto:maq@panoramix.ift.uni.wroc.pl ",
                             "http://panoramix.ift.uni.wroc.pl/~maq",
                             "                            ",
                             "                             ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "\t...scroll restart...        ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                            "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                            "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
                             "                            ",
};                                                

  
FXDEFMAP(AboutWindow) AboutWindowMap[]=
{
  FXMAPFUNC(SEL_PAINT,         AboutWindow::ID_CANVAS,   AboutWindow::onExpose),
  FXMAPFUNC(SEL_TIMEOUT,       AboutWindow::ID_TIMEOUT,  AboutWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,         AboutWindow::ID_CHORE,    AboutWindow::onChore),
 

  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   AboutWindow::ID_CANVASGL, AboutWindow::onMouseUp),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE, AboutWindow::ID_CANVASGL, AboutWindow::onMouseDown),

  FXMAPFUNC(SEL_MOTION,            AboutWindow::ID_CANVASGL, AboutWindow::onMouseMove),

 
};


FXIMPLEMENT(AboutWindow,FXDialogBox,AboutWindowMap,ARRAYNUMBER(AboutWindowMap))


AboutWindow::AboutWindow(FXWindow *owner,const FXString& title):
  FXDialogBox(owner,title,DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE,300,200,250,400, 6,6,6,6, 4,4){
  

  glcanvasFrame=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,50,50,0,0,0,0);

  glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
  glcanvas=new FXGLCanvas(glcanvasFrame,glvisual,this,ID_CANVASGL,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);


  frame      = new FXHorizontalFrame (this, LAYOUT_SIDE_TOP|
                          LAYOUT_FILL_X|LAYOUT_FILL_Y);



  textCanvas = new FXVerticalFrame (frame, LAYOUT_FILL_X|
                           LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

  hSeparator = new FXHorizontalSeparator (textCanvas, SEPARATOR_GROOVE|
                              LAYOUT_FILL_X);
  textPanel  = new FXVerticalFrame (textCanvas, FRAME_SUNKEN|
                        FRAME_THICK|LAYOUT_FILL_X|
                        LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);
  text  = new FXText (textPanel, NULL, 0,
                LAYOUT_FILL_Y|LAYOUT_FILL_X|
                VSCROLLER_NEVER|HSCROLLER_NEVER|
                TEXT_READONLY|TEXT_OVERSTRIKE);
  text->setTextColor (FXRGB (0,0,0));
  
  timer = NULL;
  timer = getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);


  yIncrement = 0;
  lineSpacer = "\n";
  lastPosition = 0;
  numberDevelopers = sizeof (developers) / sizeof (developers[0]);


  text->setText (developers[0]);
  for (int b = 0; b < numberDevelopers; b++)
  {
    FXString paddedString (developers[b]);
    paddedString += lineSpacer;
    text->appendText (paddedString.text(), strlen (paddedString.text()));
  }

	kolor=0;
	mouse=0;

}



long AboutWindow::onExpose(FXObject* tg,FXSelector sel,void* ptr)
{
  drawScene();
  return 1;
}


void AboutWindow::setAbout(const FXString& About){
  Abouttext->setText(About);
  }


FXString AboutWindow::getAbout() const {
  return Abouttext->getText();
  }


AboutWindow::~AboutWindow(){
  Abouttext=(FXText*)-1;
  if (timer) getApp()->removeTimeout(timer);
  }


long AboutWindow::onTimeout(FXObject* tg,FXSelector sel,void* ptr)
{
if(kolor>255)
 {
  drawScene();
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
 } else
 {
  getApp()->forceRefresh();
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  kolor=kolor+1;  
  }

  drawGLScene();

  return 1;
}

long AboutWindow::onChore(FXObject* tg,FXSelector sel,void* ptr)
{
 if(kolor>255)
 {
  drawScene();
  chore=getApp()->addChore(this,ID_CHORE);
 } else
 {
  chore=getApp()->addChore(this,ID_CHORE);
  kolor=kolor+1;  
  }

  drawGLScene();


 return 1;
}



double t,dt=0.01,x=0,y=0.4,dx=0,dy=0,vx=1,vy=1,dvx=0,dvy=0,m=1;
int angleab=0;
double g=-9.8;





long AboutWindow::drawGLScene()
{
 float w,h;

 angleab+=1;

if(mouse==0)
{
	dvy=g;
	dvy*=dt;
	vy += dvy;
	dy  = vy*dt;
	y  = y + dy;

    
	dvx*=dt;
	vx += dvx;
	dx  = vx*dt;
	x  = x + dx;



	// Bouncy

	if( x<-1)
	{
		x=-1;
		vx = -vx;
	}
	if( y<-1)
	{
		y=-1;
		vy = -vy;
	}
	if( x>1)
	{
		x=1;
		vx = -vx;
	}
	if( y>1)
	{
		y=1;
		vy = -vy;
	}

}
  glcanvas->makeCurrent();

  w=glcanvas->getWidth();
  h=glcanvas->getHeight();

  glViewport(0,0,w,h);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_DITHER);


  glPointSize(5);     


glColor3f((1.5+y)/2,(1.5+y)/2,(1.5+y)/2);

  glBegin(GL_POINTS);
   glVertex3f(x,y,0);
   glVertex3f(x,y,0);
  glEnd();


  if(glvisual->isDoubleBuffer())
  {
     glcanvas->swapBuffers();
  }
  glcanvas->makeNonCurrent();


  return 1;

}

long AboutWindow::drawScene()
{


  return 1;
}




long AboutWindow::onMouseMove(FXObject* tg,FXSelector sel,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
	
if(mouse==1)
{


 x=((double)(ev->win_x)/(double)glcanvas->getWidth()-0.5)*2;
 y=(-1*(double)(ev->win_y)/(double)glcanvas->getHeight()+0.5)*2;

 dt=0.01;
 dx=0;
 dy=0;
 vx=1;
 vy=1;
 dvx=0;
 dvy=0;

 drawGLScene();

}


 return 1;
}
long AboutWindow::onMouseUp(FXObject* tg,FXSelector sel,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;


 mouse=1;
 onMouseMove(NULL,NULL,ptr);

 return 1;
}
long AboutWindow::onMouseDown(FXObject* tg,FXSelector sel,void* ptr)
{
 mouse=0;
 return 1;
}
