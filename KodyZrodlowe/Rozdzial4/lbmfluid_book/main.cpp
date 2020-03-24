#include <iostream>
#include <GL/glut.h>
#include "lbm.h"
#include "particles.h"
using namespace std;
float dt=100.0;
int mode=1;		// draw mode
int pause=0;
float mnoznik_alpha=0.1;
int interponoff = 1;			// interpolować prędkości?

#define SCALE 400

void changeSize(int w, int h) 
{
	float ratio = 1.0 * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0, W, 0,H);
	glMatrixMode(GL_MODELVIEW);
}
void idleFunction(void)
{
   if(pause==0)
   {		
    lbm();
    if(interponoff)
    	moveparinterpolate(dt);	
    else
    	movepar(dt);			// bez interpolacji prędkości
    //
   }

   //static int s = 0;
   //if(!(s++ % 100))
  //	 cout << tortuosity() << endl;
   glutPostRedisplay();
}

void timerFunc(int i)
{

//	glutPostRedisplay();
//  glutTimerFunc (0,&timerFunc,0);  
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
	initlbm();								// INIT LBM
    cout << "# initialize lbm loop "<< endl;
	//for(int i=0; i<1e2; i++) lbm();
    cout << "# done" << endl;
	   npar = 1536;//NPARMAX;
	initpar();
}
void keys(unsigned char key, int x, int y) 
{
	if(key == 27)	exit(0);
   	if(key=='-')  	dt/=2.0;
    if(key=='+')  	dt*=2.0;
    if(key=='p')	pause = 1-pause;   
    if(key=='x')    {fx*=2.0; cout << "f: " << fx << endl;}
    if(key=='z')    {fx/=2.0; cout << "f: " << fx << endl;}
    if(key==',') {	mnoznik_alpha/=2; cout << "mnoznik_alpha: " << mnoznik_alpha << endl;}
    if(key=='.') 
	 {
		mnoznik_alpha*=2; cout << "mnoznik_alpha: " << mnoznik_alpha << endl;
	 }
    if(key=='t') if(PSIZ>1) PSIZ--;
    if(key=='y') PSIZ++;
   
    if(key=='[')	//if(npar>NPARMAX/256 && npar<NPARMAX) 
	 if(npar/2>0) {npar/=2; cout << npar << endl; initpar();}
    if(key==']') 	if(npar*2<=NPARMAX)	{npar*=2; cout << npar << endl; initpar();}
   
    if(key=='m')
	 {
		//glClear(GL_COLOR_BUFFER_BIT);
		mode=1-mode;
	 }
    if(key=='i')
	 {
		interponoff = 1-interponoff;
		cout << "interponoff = " << interponoff << endl;
	 }
   
	if (key==' ') 	initpar();
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(SCALE * W,SCALE * H);
	glutCreateWindow("lbmfluid by Maciej Matyka (Sykofi2)");
	init();									// INIT LBM
	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keys);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunction);
    glutTimerFunc (0,&timerFunc,0);
	// enter GLUT event processing cycle
	glutMainLoop();
}

