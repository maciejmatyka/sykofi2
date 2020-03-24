


/*

Program:			Waves
Wersja:				1.0
Wersja Jezykowa:	Angielska
Autor:				Maciej Matyka
System Operacyjny:		Win/Linux (Linux - wymaga rekompilacji)
Wymagane Biblioteki 
do kompilacji:			FOX, Glut

Opis:

  Symulacja zjawisk falowych. Program rozwiazuje numerycznie rownanie falowe
  rozpinajac rozwiazanie na siatce roznicowej. Wizualizacja w dwoch lub trzech 
  wymiarach (OpenGL).




*/



#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>




#include "fx.h"
#include "fx3d.h"
  
#include "fox.h"

#include "grid.h"

Grid TheWave;
//Grid TheWave2;





#define PX 0
#define PY 1
#define PZ 2


float t,dt;
GLint size; 

//GLenum rgb, doubleBuffer, windType; 
//GLenum mode; 


#define NX 70	 
#define NY 70

float SWIDTH=600;
float SHEIGHT=600;

float DX=(SWIDTH/NX);
float DY=(SHEIGHT/NY);

float DT=0.8;

int angle=0;
int anglex=0,angley=0,anglez=0;
float scale2d=0;
float scale3d=0;
float scale=0;



#define VEL 2.5


extern int GLVECTORS;
extern int ROTATEONOFF; // from fox.o
extern int SOURCESONOFF; // from fox.o
extern int FOGONOFF,FLATONOFF,LINE3DONOFF;

extern int VIEWMODE;

extern int spinning;

void glinit(void);



			   


long WavesWindow::onMotionRotX(FXObject* sender,FXSelector,void* ptr)
{
	anglex=m_pFXDial1->getValue();
	if(spinning==0)
	drawScene();
  return 1;
  }

long WavesWindow::onMotionRotY(FXObject* sender,FXSelector,void* ptr)
{
	angley=m_pFXDial2->getValue();
	if(spinning==0)
	drawScene();
  return 1;
  }


long WavesWindow::onMotionRotZ(FXObject* sender,FXSelector,void* ptr)
{
	anglez=m_pFXDial3->getValue();
	if(spinning==0)
	drawScene();
  return 1;
}




long WavesWindow::onMotionZoom(FXObject* sender,FXSelector,void* ptr)
{

  //	if(m_pFXDial1->onMotion(sender,NULL,ptr))
/*	anglex=m_pFXDial1->getValue();
	angley=m_pFXDial2->getValue();
	anglez=m_pFXDial3->getValue();
*/

	if(dial_zoom==1)
	switch(VIEWMODE)
	{
		case 1:
			scale3d=m_pFXDial4->getValue();
			break;		
		case 2:
			scale2d=m_pFXDial4->getValue();
			break;
		default:
			break;
	}

	if(spinning==0)
	drawScene();

  return 1;
  }



#define K_LEFT 2
#define K_RIGHT 3

long WavesWindow::onMouseGDown(FXObject*,FXSelector,void*)
{
	if(VIEWMODE==1)
	{
		key=K_LEFT;

   //	getApp()->cursorWindow=rotateCursor;
//setCursor(rotateCursor);

		setDefaultCursor(rotateCursor);
	}
return 1;
  }

long WavesWindow::onMouseGMove(FXObject*, FXSelector, void* ptr){
 FXEvent *ev=(FXEvent*)ptr;

if(key==K_LEFT)
{
  anglex+=ev->last_y-ev->win_y;
  angley+=ev->last_x-ev->win_x;

  m_pFXDial1->setValue(anglex);
  m_pFXDial2->setValue(angley);
}

if(key==K_RIGHT)
{
	switch(VIEWMODE)
	{
		case 1:
			scale3d+=(ev->last_y-ev->win_y);
			scale3d+=(ev->last_x-ev->win_x);
			m_pFXDial4->setValue(scale3d);
			break;		
		case 2:
			scale2d+=(ev->last_y-ev->win_y);
			scale2d+=(ev->last_x-ev->win_x);
			if(scale2d<0) 
				scale2d = 0;
			m_pFXDial4->setValue(scale2d);
			break;
		default:
			break;
	}

}

	if(spinning==0)
	drawScene();


  return 1;
  }




long WavesWindow::onMouseGUp(FXObject*,FXSelector,void* ptr)
{
	//getApp()->setDefaultCursor(normalCursor);
	if(VIEWMODE==1)
	{
		setDefaultCursor(normalCursor);
 		key=0;
	}
  return 1;
  }



long WavesWindow::onMouseGRightDown(FXObject* sender,FXSelector,void* ptr)
{
	if(VIEWMODE == 1 || VIEWMODE == 2)
	{
		key=K_RIGHT;
		setDefaultCursor(zoomCursor);
	}
//	onQueryMenu(sender,0,ptr);


	return 1;
  }

long WavesWindow::onMouseGRightUp(FXObject*,FXSelector,void* ptr)
{
	if(VIEWMODE == 1 || VIEWMODE == 2)
	{
		key=0;
		setDefaultCursor(normalCursor);
	}

  return 1;
  }













long WavesWindow::onMouseDown(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;

	if(mousedown==0)
	{
		mousedown=1;
		startx=ev->win_x;
		starty=ev->win_y;
	}

//	TheWave.clearsources();

	return 1;
  }



long WavesWindow::onMouseMove(FXObject*, FXSelector, void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
 float w,h;

	w=glcanvas2->getWidth();
	h=glcanvas2->getHeight();

	if(mousedown==1)
	{
		 glcanvas2->makeCurrent();
		 glViewport(0,0,w,h);

		 TheWave.renderBnd(w,h);
		 TheWave.renderSrc(w,h);

		 endx=ev->win_x;
		 endy=ev->win_y;

		 if(endx<0) endx = 0;
		 if(endx>w) endx = w;

		 if(endy<0) endy = 0;
		 if(endy>h) endy = h;


		 TheWave.renderLine((startx/w - 0.5)*2,-(starty/h - 0.5)*2,(endx/w - 0.5)*2,-(endy/h - 0.5)*2 ,w,h);
		 TheWave.clearBndTemp();
		 TheWave.addBndTemp((startx/w - 0.5)*2,-(starty/h - 0.5)*2,(endx/w - 0.5)*2,-(endy/h - 0.5)*2 ,w,h);


		glColor3f(1,1,0);
	 	glBegin(GL_POINTS);
		glVertex2f((endx/w - 0.5)*2,-(endy/h - 0.5)*2);
		glEnd();


		  if(glvisual2->isDoubleBuffer()){
		    glcanvas2->swapBuffers();
		    }
		  glcanvas2->makeNonCurrent();
	}
	else
	{
		 glcanvas2->makeCurrent();
		 glViewport(0,0,w,h);

		 TheWave.renderBnd(w,h);
		 TheWave.renderSrc(w,h);

		 endx=ev->win_x;
		 endy=ev->win_y;

		 if(endx<0) endx = 0;
		 if(endx>w) endx = w;

		 if(endy<0) endy = 0;
		 if(endy>h) endy = h;




		glColor3f(1,1,0);
	 	glBegin(GL_POINTS);
		glVertex2f((endx/w - 0.5)*2,-(endy/h - 0.5)*2);
		glEnd();


		  if(glvisual2->isDoubleBuffer()){
		    glcanvas2->swapBuffers();
		    }
		  glcanvas2->makeNonCurrent();



	}

	

  return 1;
  }
/*
long WavesWindow::onMouseMove(FXObject*, FXSelector, void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
 float w,h;



	w=glcanvas2->getWidth();
	h=glcanvas2->getHeight();

	 glcanvas2->makeCurrent();
	 glViewport(0,0,w,h);

		 endx=ev->win_x;
		 endy=ev->win_y;
		 if(endx<0) endx = 0;
		 if(endx>w) endx = w;

		 if(endy<0) endy = 0;
		 if(endy>h) endy = h;


  
	if(mousedown==1)
	{

		 TheWave.renderLine((startx/w - 0.5)*2,-(starty/h - 0.5)*2,(endx/w - 0.5)*2,-(endy/h - 0.5)*2 ,w,h);
		 TheWave.clearBndTemp();
		 TheWave.addBndTemp((startx/w - 0.5)*2,-(starty/h - 0.5)*2,(endx/w - 0.5)*2,-(endy/h - 0.5)*2 ,w,h);

		glColor3f(1,1,0);
	 	glBegin(GL_POINTS);
		glVertex2f((endx/w - 0.5)*2,-(endy/h - 0.5)*2);
		glEnd();


	}
	else
	{

// draw a point

 		 TheWave.renderBnd(w,h);
		 TheWave.renderSrc(w,h);

		glColor3f(1,1,0);
	 	glBegin(GL_POINTS);
		glVertex2f((endx/w - 0.5)*2,-(endy/h - 0.5)*2);
		glEnd();



	}


		  if(glvisual2->isDoubleBuffer()){
		    glcanvas2->swapBuffers();
		    }
		  glcanvas2->makeNonCurrent();

  return 1;
  }
*/


long WavesWindow::onMouseUp(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
 FXuint answer;
 float w,h;

	w=glcanvas2->getWidth();
	h=glcanvas2->getHeight();

	mousedown=0;


	answer=FXMessageBox::question(this,MBOX_YES_NO,"Please confirm.","Add that boundary?");
    if(answer==MBOX_CLICKED_YES)
		 TheWave.addBnd((startx/w - 0.5)*2,-(starty/h - 0.5)*2,(endx/w - 0.5)*2,-(endy/h - 0.5)*2 ,w,h);

		TheWave.clearBndTemp();


		 glcanvas2->makeCurrent();
		 glViewport(0,0,w,h);

		 TheWave.renderBnd(w,h);
		 TheWave.renderSrc(w,h);

		  if(glvisual2->isDoubleBuffer()){
		    glcanvas2->swapBuffers();
		    }
		  glcanvas2->makeNonCurrent();
		

  return 1;
  }






long WavesWindow::onMouseRightDown(FXObject* sender,FXSelector,void* ptr)
{

	return 1;
  }

long WavesWindow::onMouseRightUp(FXObject*,FXSelector,void* ptr)
{
 FXEvent *ev=(FXEvent*)ptr;
 float w,h;
 FXuint answer;


	w=glcanvas2->getWidth();
	h=glcanvas2->getHeight();


		 glcanvas2->makeCurrent();
		 glViewport(0,0,w,h);

		 TheWave.renderBnd(w,h);
		 TheWave.renderSrc(w,h);

		 endx=ev->win_x;
		 endy=ev->win_y;

		 if(endx<0) endx = 0;
		 if(endx>w) endx = w;

		 if(endy<0) endy = 0;
		 if(endy>h) endy = h;


		 TheWave.changeSRC((endx/w - 0.5)*2,-(endy/h - 0.5)*2,w,h);

	  return 1;
  }






long WavesWindow::onCmdSources(FXObject* a,FXSelector b,void* c){
	(SOURCESONOFF) ? SOURCESONOFF = 0 : SOURCESONOFF = 1;
	(SOURCESONOFF) ? C_SOURCES->onCheck(0,0,0) : C_SOURCES->onUncheck(0,0,0);

//   TheWave.clearsources();
   return 1;
}



long WavesWindow::onChore(FXObject*,FXSelector,void*){
  GLdouble windW = glcanvas->getWidth();
  GLdouble windH = glcanvas->getHeight();


  // onMotionZoom(NULL,NULL,NULL);


if(ROTATEONOFF)
{
	angle+=1;
	if(angle > 360) angle-=360;
}
	
	t+=DT*10*(PI/180);

	TheWave.solvewave(t,DT,DX,DY,SOURCESONOFF);
//	TheWave2.solvewave(t,DT,DX,DY,SOURCESONOFF);


  drawScene();
  chore=getApp()->addChore(this,ID_CHORE);

  return 1;
  }


long WavesWindow::onCmdReset(FXObject*,FXSelector,void*)
 {
	TheWave.reset();
	drawScene(); 

	glcanvas2->makeCurrent();
	glViewport(0,0,glcanvas2->getWidth(),glcanvas2->getHeight());

	TheWave.renderBnd(glcanvas2->getWidth(),glcanvas2->getHeight());
	TheWave.renderSrc(glcanvas2->getWidth(),glcanvas2->getHeight());

	if(glvisual2->isDoubleBuffer()){
	  glcanvas2->swapBuffers();
	   }
	glcanvas2->makeNonCurrent();
	

	TheWave.clearsources();

 
	return 1;
 }

long WavesWindow::onUpdReset(FXObject* sender,FXSelector,void*)
 {
  FXButton* button=(FXButton*)sender;
  return 1;
 }


void WavesWindow::drawScene(){
	const GLfloat lightPosition[]={10.,4.,5.,1.};
	const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
	const GLfloat lightDiffuse[]={.9f,.9f,.9f,1.};
	const GLfloat redMaterial[]={1.,0.,0.,1.};
	const GLfloat blueMaterial[]={0.,0.,1.,1.};

	GLfloat fogColour[] = { 0.0f, 0.2f, 0.2f, 0.0f };
	 

  GLdouble width = glcanvas->getWidth();
  GLdouble height = glcanvas->getHeight();
  GLdouble aspect = width / height;



	//gl output 1
	glcanvas->makeCurrent();
	glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());

	switch(VIEWMODE)
	{
		case 1:

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			TheWave.render3d(width,height,aspect,angle,anglex+180, angley, anglez,scale3d/7,0);
 //			TheWave2.render3d(width,height,aspect,angle,anglex+180, angley, anglez,scale3d/7,1);


			break;		
		case 2:
			TheWave.render2dlines(glcanvas->getWidth(), glcanvas->getHeight(),scale2d/8);

			break;
		case 3:
			TheWave.render2d(glcanvas->getWidth(),glcanvas->getHeight(),0);
			break;
		default:
			break;

	}



	if(glvisual->isDoubleBuffer())
	{
		glcanvas->swapBuffers();
	}
	glcanvas->makeNonCurrent();




/// gl output 2


if(mousedown==0)
{

	glcanvas2->makeCurrent();
	glViewport(0,0,glcanvas2->getWidth(),glcanvas2->getHeight());

	TheWave.renderBnd(glcanvas2->getWidth(),glcanvas2->getHeight());
	TheWave.renderSrc(glcanvas2->getWidth(),glcanvas2->getHeight());

	if(glvisual2->isDoubleBuffer()){
	  glcanvas2->swapBuffers();
	   }
	glcanvas2->makeNonCurrent();
}



}



  void glinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);      
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);      

	
//	mode = GL_FALSE;     
	size = 4; 

 	dt=(float)0.1;
   
}



int main(int argc,char *argv[]){

   

	FXApp application("Waves","Waves Fox");

	application.init(argc,argv);

	new WavesWindow(&application);
   // new CreditsWindow(&application);


	application.create();

	glinit();
 
  return application.run();
}
