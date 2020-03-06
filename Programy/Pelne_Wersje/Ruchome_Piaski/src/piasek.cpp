/*


Program:			Ruchome Piaski
Wersja:				1.0
Autor:				Maciej Matyka
System Operacyjny:		Win/Linux (Linux - wymaga rekompilacji)
Wymagane Biblioteki 
do kompilacji:			FOX, Glut

Opis:

  Program sluzy do symulacji ruchu piasku (z przeszkodami) w polu grawitacyjnym. Najprostsze
  mozliwe rozwiazanie tego problemu przy pomocy automatu komorkowego.


*/

#include <string.h>
#include <math.h>
#include <gl/glaux.h>
#include "fx.h"
#include "fx3d.h"


#include "piachicon.h"
  
#define WIDTH 150
#define HEIGHT 150

#define NX WIDTH
#define NY HEIGHT


int Sc[WIDTH][HEIGHT]={0};
int Gc[WIDTH][HEIGHT]={0};

int S[WIDTH][HEIGHT]={0};
int G[WIDTH][HEIGHT]={0};


float SCOLr[WIDTH][HEIGHT]={0};
float SCOLg[WIDTH][HEIGHT]={0};
float SCOLb[WIDTH][HEIGHT]={0};

#define C_EMP 0
#define C_SND 1

int tama=1;
float gbak;


// klawisze myszki
int mousemode=0;

int TOOL_SIZE=4;

int SYPANIEONOFF=1;
int BRZEGIONOFF=0;


// global datas

char blah[100];
float point[3] = { 0.0, 0.0, 0.0 };   
GLint size; 
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };	
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLenum rgb, doubleBuffer, windType; 
GLenum mode; 


// c functions

void glinit(void);





const FXuint TIMER_INTERVAL = 100;


class PiasekWindow : public FXMainWindow {
  FXDECLARE(PiasekWindow)

private:

  FXGLCanvas      *glcanvas;
  FXTimer         *timer;   
  FXChore         *chore;   
  int              spinning;                  
  float           angle;                     
  FXGLVisual      *glvisual;                  

  FXRadioButton   *but_sypanie;  
  FXRadioButton   *but_brzegi;  



protected:
  PiasekWindow(){}
  FXTextField* poptextx;
  FXTextField* poptexty;
  FXTextField* poptextvx;
  FXTextField* poptextvy;
  FXTextField* gravitytext;


public:

  enum{
    ID_CANVAS=FXMainWindow::ID_LAST,
    ID_SPIN,
    ID_SPINFAST,
    ID_STOP,
    ID_TIMEOUT,
    ID_CHORE,

	ID_ZWOLNIJ,
	ID_SYPANIE,
	ID_BRZEGI,
	ID_ZAPISZ,
	ID_WCZYTAJ,

  	};
    
  long onMouseRDown(FXObject*,FXSelector,void*);
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

  long onCmdZwolnij(FXObject*,FXSelector,void*);

  long onCmdSypanie(FXObject*,FXSelector,void*);
  long onUpdSypanie(FXObject*,FXSelector,void*);

  long onCmdBrzegi(FXObject*,FXSelector,void*);
  long onUpdBrzegi(FXObject*,FXSelector,void*);


  long onCmdZapisz(FXObject*,FXSelector,void*);
  long onCmdWczytaj(FXObject*,FXSelector,void*);




  void timestep(void);


  
public:

  // PiasekWindow constructor
  PiasekWindow(FXApp* a);
  
  void create();

  
  void drawScene();
  
  virtual ~PiasekWindow();
  };



FXDEFMAP(PiasekWindow) PiasekWindowMap[]={

  FXMAPFUNC(SEL_PAINT,         PiasekWindow::ID_CANVAS,   PiasekWindow::onExpose),
  FXMAPFUNC(SEL_CONFIGURE,     PiasekWindow::ID_CANVAS,   PiasekWindow::onConfigure),




  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   PiasekWindow::ID_CANVAS, PiasekWindow::onMouseDown),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE, PiasekWindow::ID_CANVAS, PiasekWindow::onMouseUp),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   PiasekWindow::ID_CANVAS, PiasekWindow::onMouseRDown),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, PiasekWindow::ID_CANVAS, PiasekWindow::onMouseUp),
  FXMAPFUNC(SEL_MOTION,            PiasekWindow::ID_CANVAS, PiasekWindow::onMouseMove),




  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_SYPANIE,     PiasekWindow::onCmdSypanie),
  FXMAPFUNC(SEL_UPDATE,        PiasekWindow::ID_SYPANIE,     PiasekWindow::onUpdSypanie),

  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_BRZEGI,     PiasekWindow::onCmdBrzegi),
  FXMAPFUNC(SEL_UPDATE,        PiasekWindow::ID_BRZEGI,     PiasekWindow::onUpdBrzegi),


 
  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_SPIN,     PiasekWindow::onCmdSpin),
  FXMAPFUNC(SEL_UPDATE,        PiasekWindow::ID_SPIN,     PiasekWindow::onUpdSpin),
 
  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_SPINFAST, PiasekWindow::onCmdSpinFast),
  FXMAPFUNC(SEL_UPDATE,        PiasekWindow::ID_SPINFAST, PiasekWindow::onUpdSpinFast),
 
  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_STOP,     PiasekWindow::onCmdStop),
  FXMAPFUNC(SEL_UPDATE,        PiasekWindow::ID_STOP,     PiasekWindow::onUpdStop),

  FXMAPFUNC(SEL_TIMEOUT,       PiasekWindow::ID_TIMEOUT,  PiasekWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,         PiasekWindow::ID_CHORE,    PiasekWindow::onChore),


  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_ZWOLNIJ,     PiasekWindow::onCmdZwolnij),


  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_WCZYTAJ,     PiasekWindow::onCmdWczytaj),
  FXMAPFUNC(SEL_COMMAND,       PiasekWindow::ID_ZAPISZ,     PiasekWindow::onCmdZapisz),


  };

FXIMPLEMENT(PiasekWindow,FXMainWindow,PiasekWindowMap,ARRAYNUMBER(PiasekWindowMap))

PiasekWindow::PiasekWindow(FXApp* a):FXMainWindow(a,"Ruchome Piaski 1.0",NULL,NULL,DECOR_ALL,0,0,450,350){
  FXVerticalFrame *glcanvasFrame;
  FXVerticalFrame *buttonFrame;
  FXComposite *glpanel;
  FXHorizontalFrame *frame;

	int i,j;

  FXGIFIcon *smallpiachicon;

  smallpiachicon=new FXGIFIcon(getApp(),piachgif);
  setMiniIcon(smallpiachicon);  

  frame=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  
  glcanvasFrame=new FXVerticalFrame(frame,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
  new FXLabel(glcanvasFrame,"Ruchome Piaski 1.0",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(glcanvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_TOP|LAYOUT_LEFT,0,0,WIDTH*2,HEIGHT*2, 0,0,0,0);
  glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);
  glcanvas=new FXGLCanvas(glpanel,glvisual,this,ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);
  buttonFrame=new FXVerticalFrame(frame,LAYOUT_FILL_Y|FRAME_RIDGE|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  new FXLabel(buttonFrame,"Symulacja",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);
  new FXButton(buttonFrame,"&Start",NULL,this,ID_SPINFAST,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  new FXButton(buttonFrame,"&Pauza",NULL,this,ID_STOP,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  new FXButton(buttonFrame,"&Wyjœcie.",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  new FXLabel(buttonFrame,"Tryb Edycji",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  but_sypanie=new FXRadioButton(buttonFrame,"Sypanie piaskiem",this,ID_SYPANIE);
  but_brzegi=new FXRadioButton(buttonFrame,"Edycja brzegów",this,ID_BRZEGI);

  SYPANIEONOFF ? but_sypanie->onCheck(0,0,0) : but_sypanie->onUncheck(0,0,0);
  BRZEGIONOFF ? but_brzegi->onCheck(0,0,0) : but_brzegi->onUncheck(0,0,0);

  new FXButton(buttonFrame,"&Czyœæ Wszystko",NULL,this,ID_ZWOLNIJ,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);


  new FXLabel(buttonFrame,"Konfiguracja",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);


  new FXButton(buttonFrame,"&Zapisz",NULL,this,ID_ZAPISZ,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  new FXButton(buttonFrame,"&Wczytaj",NULL,this,ID_WCZYTAJ,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

 spinning=0;    
  timer = NULL;
  angle = 0.;


for(i=0;i<NX;i++)
for(j=0;j<NY;j++)
{
	SCOLr[i][j]=(rand()/(float)RAND_MAX)/4+0.6;
	SCOLg[i][j]=(rand()/(float)RAND_MAX)/6+0.5;
	SCOLb[i][j]=(rand()/(float)RAND_MAX)/8+0.4;
}


/* 	for(i=0;i<40;i++)
	for(j=0;j<70;j++)
		S[NX/2+i][NY/2+j]=C_SND;
*/



for(i=0;i<NX;i++)
for(j=0;j<60;j++)
		S[i][NY/2+j]=C_SND;

//for(j=0;j<2;j++)
	for(i=1;i<NX;i++)
	if(abs(i-NX/2)>3)
 		G[i][NY/2]=1;


	for(i=2;i<80;i++)
	{
		for(j=0;j<NX/2-i*sin((105-i)*PI/180);j++)
		{
 			G[j][NY/2+i-2]=1;
 			S[j][NY/2+i-2]=C_EMP;

 			G[j][NY/2-i+2]=1;
 			S[j][NY/2-i+2]=C_EMP;
		}

		for(j=NX-1;j>NX/2+i*sin((105-i)*PI/180);j--)
		{
			G[j][NY/2+i-2]=1;
 			S[j][NY/2+i-2]=C_EMP;

			G[j][NY/2-i+2]=1;
 			S[j][NY/2-i+2]=C_EMP;
		}
	}

// dolna polka
	for(j=0;j<=10;j++)
	for(i=0;i<NX;i++)
	{
		G[i][NY-j]=1;
		G[i][j]=1;
	}



  

  // init gl

  glinit();

  }
    

PiasekWindow::~PiasekWindow(){
  if(timer) getApp()->removeTimeout(timer);
  }

 

void PiasekWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}




long PiasekWindow::onConfigure(FXObject*,FXSelector,void*){
  if(glcanvas->makeCurrent()){
    glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());
    glcanvas->makeNonCurrent();
    }
  return 1;
  }



long PiasekWindow::onExpose(FXObject*,FXSelector,void*){
  drawScene();
  return 1;
  }


long PiasekWindow::onTimeout(FXObject*,FXSelector,void*){
  angle += 2.;
  if(angle > 360.) angle -= 360.;
  drawScene();
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }



long PiasekWindow::onChore(FXObject*,FXSelector,void*){

  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();


	timestep();
  drawScene();
  chore=getApp()->addChore(this,ID_CHORE);

  return 1;
  }


long PiasekWindow::onCmdSpin(FXObject*,FXSelector,void*){
  spinning=1;
  timer=getApp()->addTimeout(TIMER_INTERVAL,this,ID_TIMEOUT);
  return 1;
  }


long PiasekWindow::onUpdSpin(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }

long PiasekWindow::onCmdSpinFast(FXObject*,FXSelector,void*){
  spinning=1;
  chore=getApp()->addChore(this,ID_CHORE);
  return 1;
  }



long PiasekWindow::onCmdZwolnij(FXObject*,FXSelector,void*){
	int i,j=0;


for(i=0;i<NX;i++)
for(j=0;j<NY;j++)
{

		S[i][j]=0;
		G[i][j]=0;

}


		if(!spinning)
			drawScene();


  return 1;
  }

long PiasekWindow::onUpdSpinFast(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable(); 
  return 1;
  }


long PiasekWindow::onCmdStop(FXObject*,FXSelector,void*){
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

long PiasekWindow::onUpdStop(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->enable() : button->disable(); 
  return 1;
  }



// zapisywanie iodczytywanie konfiguracji na dysku
long PiasekWindow::onCmdZapisz(FXObject* sender,FXSelector,void*){

 FXString pattern = "*.pia";
  FXFileDialog opendialog(this,"Wybierz plik .pia");
  opendialog.setSelectMode(SELECTFILE_ANY);
//	FXString directory = "opt/";
   FXString file;
	
	int i,j;

	FILE *fp;
	char workbuf[100];


 
  opendialog.setPattern(pattern);
//  opendialog.setDirectory(directory);


  if(opendialog.execute())
  {
    file=opendialog.getFilename();
  }


  if(!(fp=fopen((char *)file.text(),"w")))
  {
	sprintf(workbuf,"Nie mo¿na otworzyæ pliku %s do zapisu.\n",file);
//	FXMessageBox::question(this,MBOX_OK,"B³¹d!",workbuf);
  	return(1);
  }


	// procka zapisuj¹ca


	fputs("!PIASEK",fp);

for(i=0;i<NX;i++)
for(j=0;j<NY;j++)
{
	fputc(S[i][j],fp);
	fputc(G[i][j],fp);
}

	// koniec procki zapisuj¹cej
	fclose(fp);


  return 1;
  }


long PiasekWindow::onCmdWczytaj(FXObject* sender,FXSelector,void*){

 FXString pattern = "*.pia";
  FXFileDialog opendialog(this,"Wybierz plik .pia");
  opendialog.setSelectMode(SELECTFILE_EXISTING);
//	FXString directory = "opt/";
   FXString file;
	
	int i,j;

	FILE *fp;
	char workbuf[100];
	char *pocz;

  opendialog.setPattern(pattern);
//  opendialog.setDirectory(directory);


  if(opendialog.execute())
  {
    file=opendialog.getFilename();
  }


  if(!(fp=fopen((char *)file.text(),"r")))
  {
	sprintf(workbuf,"Nie mo¿na otworzyæ pliku %s do odczytu.\n",file);
//	FXMessageBox::question(this,MBOX_OK,"B³¹d!",workbuf);
  	return(1);
  }


	// procka odczytuj¹ca


	pocz=fgets(workbuf,8,fp);

	if(!(strcmp(pocz,"!PIASEK")==0))
	{
		sprintf(workbuf,"%s - to nie jest plik tego programu.\n",file.text());
	 //	FXMessageBox::question(this,MBOX_OK,"B³¹d!",workbuf);
		fclose(fp);
		return(1);
	}

	for(i=0;i<NX;i++)
	for(j=0;j<NY;j++)
	{
		S[i][j]=fgetc(fp);
		G[i][j]=fgetc(fp);
	}

	// koniec procki odczytuj¹cej
	fclose(fp);




  return 1;
  }







long PiasekWindow::onCmdSypanie(FXObject* sender,FXSelector,void*){

	(SYPANIEONOFF) ? SYPANIEONOFF = 0 : SYPANIEONOFF = 1;
	(BRZEGIONOFF) ? BRZEGIONOFF = 0 : BRZEGIONOFF = 1;
	  SYPANIEONOFF ? but_sypanie->onCheck(0,0,0) : but_sypanie->onUncheck(0,0,0);
	  BRZEGIONOFF ? but_brzegi->onCheck(0,0,0) : but_brzegi->onUncheck(0,0,0);

  return 1;
  }


long PiasekWindow::onCmdBrzegi(FXObject* sender,FXSelector,void*){

	(BRZEGIONOFF) ? BRZEGIONOFF = 0 : BRZEGIONOFF = 1;
	(SYPANIEONOFF) ? SYPANIEONOFF = 0 : SYPANIEONOFF = 1;
	  BRZEGIONOFF ? but_brzegi->onCheck(0,0,0) : but_brzegi->onUncheck(0,0,0);
	  SYPANIEONOFF ? but_sypanie->onCheck(0,0,0) : but_sypanie->onUncheck(0,0,0);


  return 1;
  }



long PiasekWindow::onUpdSypanie(FXObject* sender,FXSelector,void*){

	  SYPANIEONOFF ? but_sypanie->onCheck(0,0,0) : but_sypanie->onUncheck(0,0,0);

  return 1;
  }


long PiasekWindow::onUpdBrzegi(FXObject* sender,FXSelector,void*){

	  BRZEGIONOFF ? but_brzegi->onCheck(0,0,0) : but_brzegi->onUncheck(0,0,0);

  return 1;
  }



// obs³uga myszy i zdarzeñ




long PiasekWindow::onMouseUp(FXObject*,FXSelector,void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;

  	mousemode=0;

	return 1;
  }


long PiasekWindow::onMouseDown(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;


	mousemode=1;

	return 1;
  }

long PiasekWindow::onMouseRDown(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;


	mousemode=2;

	return 1;
  }



long PiasekWindow::onMouseMove(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
	int i,j;
	float windW = glcanvas->getWidth();
	float windH = glcanvas->getHeight();
	float mousex, mousey;
	int k,l;




	/* sypanie */
/*	if(mousemode==1)
	{
		mousex=NX*(ev->win_x/windW);
		mousey=NY-NY*(ev->win_y/windH);
		i=(int)(mousex);
		j=(int)(mousey);
		if(i>0 && i<NX-3 && j>0 && j<NY-3)
		S[i+(int)(((rand()/(float)RAND_MAX)*4))][j+(int)(((rand()/(float)RAND_MAX)*4))] = C_SND;
		drawScene();
	}
*/

// wstawianie piasku



	if(mousemode==1 && SYPANIEONOFF)
	{
		mousex=NX*(ev->win_x/windW);
		mousey=NY-NY*(ev->win_y/windH);
		i=(int)(mousex);
		j=(int)(mousey);
		if(i>0 && i<NX-3 && j>0 && j<NY-3)
		

		for(k=0;k<TOOL_SIZE;k++)
		for(l=0;l<TOOL_SIZE;l++)
		S[i+k][j+l] = C_SND;


		if(!spinning)
			drawScene();

	}
	else
	if(mousemode==1 && BRZEGIONOFF)
	{
		mousex=NX*(ev->win_x/windW);
		mousey=NY-NY*(ev->win_y/windH);
		i=(int)(mousex);
		j=(int)(mousey);
		if(i>0 && i<NX-3 && j>0 && j<NY-3)
		

		for(k=0;k<TOOL_SIZE;k++)
		for(l=0;l<TOOL_SIZE;l++)
			G[i+k][j+l]=1;

		if(!spinning)
			drawScene();

	}
	else if(mousemode==2 && BRZEGIONOFF)
	{
		mousex=NX*(ev->win_x/windW);
		mousey=NY-NY*(ev->win_y/windH);
		i=(int)(mousex);
		j=(int)(mousey);
		if(i>0 && i<NX-3 && j>0 && j<NY-3)
		

		for(k=0;k<TOOL_SIZE;k++)
		for(l=0;l<TOOL_SIZE;l++)
			G[i+k][j+l]=0;

		if(!spinning)
			drawScene();

	}
	else
	{


		if(!spinning)
		{
		    glClear(GL_COLOR_BUFFER_BIT);
			drawScene();
		}


	  glcanvas->makeCurrent();
	  glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());



		mousex=(ev->win_x/windW);
		mousey=(ev->win_y/windH);






	  glColor3f(0.5,1,0);
	  glPointSize(TOOL_SIZE*2);
		
	  glBegin(GL_POINTS);
	  glVertex2f((mousex-0.5)*2,(0.5-mousey)*2);
	  glEnd();



		  if(glvisual->isDoubleBuffer()){
		    glcanvas->swapBuffers();
		    }
		  glcanvas->makeNonCurrent();
	}



	return 1;
  }



void PiasekWindow::drawScene(){
	int i,j;






  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();

  

  glcanvas->makeCurrent();
  glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());


	glPointSize(2);
	glBegin(GL_POINTS);
	for(i=0;i<NX+1;i++)
		for(j=0;j<NY+1;j++)
		{
				glColor3f((1-G[i][j])*((S[i][j] == C_EMP)+(S[i][j] == C_SND)*SCOLr[i][j]),(1-G[i][j])*((S[i][j] == C_EMP)+(S[i][j] == C_SND)*SCOLg[i][j]),(1-G[i][j])*((S[i][j] == C_EMP)+(S[i][j] == C_SND)*SCOLb[i][j]));
				glVertex2f(((float)i/(float)WIDTH-0.5)*2,((float)j/(float)HEIGHT-0.5)*2);
		}
	glEnd();


    glFlush();
  if(glvisual->isDoubleBuffer()){
    glcanvas->swapBuffers();
    }
  
  glcanvas->makeNonCurrent();
  }



  void glinit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);      
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);      

	
	mode = GL_FALSE;     
}



#define Gul G[i][j]
#define Gur G[i+1][j]

#define Sul S[i][j]
#define Sur S[i+1][j]
#define Sll S[i][j-1]
#define Slr S[i+1][j-1]

int nSul,nSur,nSll,nSlr;

int step=0;


void PiasekWindow::timestep(void)
{
	int i,j;


	step+=1;

	for(j=NY-1-(step%2);j>1;j-=2)
		for(i=2+(step%2);i<NX-1;i+=2)
        if(!Gul && !Gur)
		{

			if(Sul==C_SND && Sur==C_SND && Sll!=C_SND && Slr!=C_SND)
			{
				if(rand()/(float)RAND_MAX>0.4)
				{
					Sll=Sul;
					Slr=Sur;

					Sul=C_EMP;
					Sur=C_EMP;
				}
			} else
			{		
		   		nSul   =Sul*(Gul + (1-Gul)*Sll*(Slr + (1-Slr) * Sur));
		   		nSur =Sur*(Gur + (1-Gur)*Slr*(Sll + (1-Sll) * Sul));
			    nSll   =Sll+(1-Sll)*(Sul*(1-Gul)+(1-Sul)*Sur*(1-Gur)*Slr);
				nSlr =Slr+(1-Slr)*(Sur*(1-Gur)+(1-Sur)*Sul*(1-Gul)*Sll);
				
				Sul = nSul;
				Sur = nSur;
				Sll = nSll;
				Slr = nSlr;
			}
		}

	drawScene(); 
}




int main(int argc,char *argv[]){

  FXApp application("Ruchome Piaski","Ruchome Piaski.");
  application.init(argc,argv);
  new PiasekWindow(&application);
  application.create();
  return application.run();
  }


