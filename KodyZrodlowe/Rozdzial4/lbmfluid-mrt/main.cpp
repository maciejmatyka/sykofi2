#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include "lbm.h"
#include "particles.h"
using namespace std;
float dt=1.0;
int mode=1;		// draw mode
int pause=0;
int interpolate=0;
float mnoznik_alpha=1.0;
void changeSize(int w, int h) 
{
	float ratio = 1.0 * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}
void idleFunction(void)
{
//	glutPostRedisplay();
}

static int finished = 0;
const float eps = 0.01;
float delta = 10;
static int step=0;				// 0-1001 i zerowanie
static float flux1=1e10,flux2=-1e10;
static int s = 0;
void timerFunc(int i)
{
   if(pause==0)
   {		
    lbm();



// kryterium stop
    static int CHECKINTERVAL = 1001;
    step++;
    delta = 10;
    if(step==1)
	    flux1 = volumeflux2d();
	else
	if(step==CHECKINTERVAL)
	{
	    flux2 = volumeflux2d();  
		if(flux2)
    	  delta = 100*(fabs(flux1-flux2)/flux2);

	  	if(s*CHECKINTERVAL > 5*L && delta < eps)
	      finished = 1;
	
		s++;
		cout << s << " " << delta << " " << finished << endl;
		 cout << "volumeflux() - volumeflux2d() = " << volumeflux() - volumeflux2d() << endl;
		step=0;
	}

    if(interpolate==0)
	    movepar(dt);
	else
		moveparinterpolate(dt);
   }
   
    glutPostRedisplay();
    glutTimerFunc (1,&timerFunc,0);  
}

void renderScene(void) 
{
   if(pause==0)
   {	   
   	if(mode==1)
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	drawpar(mode);								// DRAW PARTICLES
   }

   glutSwapBuffers();
}
void init(void)
{
   //float visc = 0.333*(tau-0.5);
   //cout << "Visc: "<< visc << endl;
   cout << "Re: " << ReynoldsNumber() << endl;
	initlbm();								// INIT LBM
	//for(int i=0; i<1e2; i++) lbm();
    npar = NPARMAX/10.0;
	initpar();
}
void keys(unsigned char key, int x, int y) 
{
	if(key == 27)	exit(0);
   	if(key=='-')  	dt/=2.0;
    if(key=='+')  	dt*=2.0;
    if(key=='p')	pause = 1-pause;   

   	if(key=='R')    {
	   					updateRe(goalRe+(10./100.)*goalRe);
	   					cout << ReynoldsNumber() << endl;
	   					finished = 0;
	   					delta = 10;
	   				}
   if(key=='r')
	 {
			updateRe(goalRe-(10./100.)*goalRe);
		    cout << ReynoldsNumber() << endl;		
			finished = 0;
			delta = 10;
	 }
   
static int vdfn = 0;
      if(key=='s')
	         savevdf(vdfn++);
   
    if(key==',') {mnoznik_alpha/=2; cout << "mnoznik_alpha: " << mnoznik_alpha << endl;}
    if(key=='.') 
	 {
		mnoznik_alpha*=2; cout << "mnoznik_alpha: " << mnoznik_alpha << endl;
	 }
   
    if(key=='[')	//if(npar>NPARMAX/256 && npar<NPARMAX) 
	 if(npar/2>0) {npar/=2; cout << npar << endl;}
    if(key==']') 	if(npar*2<=NPARMAX)	{npar*=2; cout << npar << endl;}
   
    if(key=='m')
	 {
		glClear(GL_COLOR_BUFFER_BIT);
		mode=1-mode;
	 }
   
	if (key==' ') 	initpar();
	if (key=='o') 	initparprobability();
	if (key=='O') 	initparprobabilityINVERSE();
	//if (key=='i') 	{interpolate = 1-interpolate; cout<<"intepo: " << interpolate << endl;}
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|
	GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("LBM MRT");
	init();									// INIT LBM
	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keys);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunction);
    glutTimerFunc (30,&timerFunc,0);
	// enter GLUT event processing cycle
	glutMainLoop();
}

