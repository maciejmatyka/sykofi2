/**
 * Implementacja metody LBM dla relaksacji tau=1
 * oparta na pomysle, aby zrezygnowac z przechowywania funkcji rozkladu.
 * Autor: Maciej Matyka, podziekowania dla Jonasa Latta za dyskusje!
 * Kod powstal na potrzeby ilustracji rozdzialu w drugim wydaniu
 * ksiazki Symulacje Komputerowe w Fizyce (Helion)
 * Wroclaw, 2019-02-16
 */

#include <iostream>
using namespace std;
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include "lbm.h"
#include "particles.h"
extern float fx;

float dt=0.05;
int mode=1;		// draw mode
int pause=0;
float mnoznik_alpha=0.1;
int interponoff = 1;			// interpolować predksci?
int lbmonoff = 1;


void changeSize(int w, int h) 
{
	float ratio = 1.0 * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0,W,0,H);
	glMatrixMode(GL_MODELVIEW);
}
int c = 1;
void simulationstep(void)
{
//   static int c = 1;
   c=1-c;
                                    
   

 	if(lbmonoff)   LBMTAU1(c);      				// DO LBM STEP
    
    if(interponoff)
    	movepar(dt,c);                        // MOVE PARTICLES//moveparinterpolate(dt);	
    else
    	movepar(dt,c);                        // MOVE PARTICLESs
    //

}

void idleFunction(void)
{
   //if(pause==0)
   {
   	static int step = 0;
   	if(!(step++ % 400))	cout << step << " " << ReynoldsNumber(c) << endl;
   	simulationstep();		
   }

	glutPostRedisplay();
}
void renderScene(void) 
{
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	
	if(pause==0)
   	{	   
   		if(mode==1)
			glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

	   // draw obstacles
	    /*float dx = W / (float) LX;
	    float dy = H / (float) LY;
	    for(int i=0; i<LX; i++)
	    for(int j=0; j<LY; j++)
	   	if(F[i][j]==1)
	    {
	        float x = i * dx;
	        float y = j * dy;
	        float c =  6.5*exp(-0.01*pow(x-W/2.0,2))*exp(-0.01*pow(y-H/2.0,2));//0.25+0.75*x;	// linear gradient 
	        glColor3f(c,c,c);

	        glBegin(GL_QUADS);
	            glVertex2f(x,y);
	            glVertex2f(x+dx,y);
	            glVertex2f(x+dx,y+dy);
	            glVertex2f(x,y+dy);
	        glEnd();
	    }*/

		drawpar(mode);								// DRAW PARTICLES
		//drawpar();								// DRAW PARTICLES
   }
	glutSwapBuffers();
}
void init(void)
{
	initlbm();								// INIT LBM
	initpar();
}
void keys(unsigned char key, int x, int y) 
{
	if (key == 27)		exit(0);
	//if(key=='o')  	lbmonoff=1-lbmonoff;
	if(key=='-')  	dt/=2.0;
    if(key=='+')  	dt*=2.0;
    if(key=='p')	pause = 1-pause;   

    if(key==',') {mnoznik_alpha/=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;}
    if(key=='.') 
	 {
		mnoznik_alpha*=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;
	 }
   
    if(key=='[')	//if(npar>NPARMAX/256 && npar<NPARMAX) 
	 				if(npar/2>2) {	npar/=1.2; cout << npar << endl;	}
    if(key==']') 	if(npar*2<=NPARMAX)	{npar*=1.2; cout << npar << endl;}
   
    if(key=='m')
	 {
		glClear(GL_COLOR_BUFFER_BIT);
		mode=1-mode;
	 }
	 if(key=='x')
	 {
	 		fx=fx*1.1;
	 		cout << "#force: " << fx << endl;
	 }
	 if(key=='z')
	 {
	 		fx=fx/1.1;
	 		cout << "#force: " << fx << endl;
	 }

    if(key=='i')
	 {
		interponoff = 1-interponoff;
		cout << "#interponoff = " << interponoff << endl;
	 }
   
	if (key==' ') 	initparALL();
	if(key=='o') initparprobability(c);
	if(key=='O') initparprobabilityINVERSE(c);
   
    /*if(key== '-') fx /=2.0;//(1e-8);
    if(key== '+') fx *=2.0;//(1e-8);
    if(key== '0') fx =(0);
   
   if(key== '.') 
	 {
		int x = LX*(rand()/float(RAND_MAX));
		int y = LY*(rand()/float(RAND_MAX));
		
		U[c][x][y] += 0.1*( rand()/float(RAND_MAX)-0.5);
		V[c][x][y] += 0.1*( rand()/float(RAND_MAX)-0.5);
	 }
   cout << "fx: " << fx << endl;
   */
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|
	GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(100,100);
	const int SCALE = 2;
	glutInitWindowSize(LX*SCALE,LY*SCALE);
	glutCreateWindow("LBM TAU=1");
	init();						
	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keys);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunction);
	// enter GLUT event processing cycle
	glutMainLoop();
}

