#include <iostream>
#include <GL/glut.h>
#include "lbm.h"
#include "particles.h"
#include "dump.h"

using namespace std;
float dt=1.0;
int mode=1;		// draw mode
int pause=0;
float mnoznik_alpha=0.1;
int interponoff = 1;			// interpolować predksci?
int lbmonoff = 1;
extern float tau;
extern float vis;
extern float rho0;

const int windW = LX * 3;
const int windH = LY * 3;

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
   if(pause==0)
   {		
    if(lbmonoff)    lbm();      
    /*if(interponoff)
    	moveparinterpolate(dt);	
    else
    	movepar(dt);			// bez interpolacji prędkości
    	*/
    //
   }
   static int s = 0;
   //if(s==0)
   //		cout << "# step Reynolds Tortuosity " << endl;

   //if(!(++s % 100))
   //{
  	//cout << s++ << " " << Reynolds() << " " << tortuosity() << " " << U[LX/2][LY/2] << endl; 
  //	cout << s++ << " " << Reynolds() << " " << tortuosity() << " " << U[LX/4][LY/2] << " " << V[LX/4][LY/2] << endl; 

   //}
//   if(s==8000)	// tylko do kanału z przeszkodą
  // {
   	//	exportvtk();			// save velocity.vtk
	//	exportprofile();
		//exit(0);
  // }
  	
  	
   char workbuf[80];
   static int frame = 0; 
   frame++;
   static GLubyte *pixels;
   if(!(frame%10))
     {
		
 	  	sprintf(workbuf, "file%05d.ppm",frame);
//	   	screenshot_ppm(workbuf, windW, windH, &pixels);
	 }
  	
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
	//drawpar(mode);								// DRAW PARTICLES

	// draw density
	float dx = 1.0 / (float) LX;
	float dy = 1.0 / (float) LY;
	for(int i=0; i<LX; i++)
	for(int j=0; j<LY; j++)
	{	
		float x = i * dx;
		float y = j * dy;
		float c = R[i][j] / rho0;
		glColor3f(c,c,c);
		glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x+dx,y);
			glVertex2f(x+dx,y+dy);
			glVertex2f(x,y+dy);
		glEnd();
	}

   }



   glutSwapBuffers();
}
void init(void)
{
	initbnd(0);								// EMPTY
	initlbm();								// INIT LBM
	
	//cout << "# initialize lbm loop "<< endl;
	//for(int i=0; i<1e2; i++) lbm();
    //cout << "# done" << endl;
	//   npar = 1536;//NPARMAX;
	//initpar();
}
void keys(unsigned char key, int x, int y) 
{
 //   cout << key << endl;
	if(key == 27)	exit(0);
   	if(key=='o')  	lbmonoff=1-lbmonoff;

	if(key=='s') 
	{
		exportvtk();			// save velocity.vtk
		exportprofile();
    
	}
	if(key=='h')
	{
		tau *= 1.1;            // 
		vis = (2*tau-1)/6.0;	// viscosity D2Q9
		cout << "tau: " << tau << " vis: " << vis << endl;
	}
	if(key=='j')
	{
		tau /= 1.1;            // 
		vis = (2*tau-1)/6.0;	// viscosity D2Q9
		cout << "tau: " << tau << " vis: " << vis << endl;
	}

   	if(key=='-')  	dt/=2.0;
    if(key=='+')  	dt*=2.0;
    if(key=='p')	pause = 1-pause;   

    if(key==',') {mnoznik_alpha/=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;}
    if(key=='.') 
	 {
		mnoznik_alpha*=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;
	 }
   
    if(key=='[')	//if(npar>NPARMAX/256 && npar<NPARMAX) 
	 if(npar/2>0) {	npar/=2; cout << npar << endl;	}
    if(key==']') 	if(npar*2<=NPARMAX)	{npar*=2; cout << npar << endl;}
   
    if(key=='m')
	 {
		glClear(GL_COLOR_BUFFER_BIT);
		mode=1-mode;
	 }
	 if(key=='1')
	 {
            initbnd(0); // empty
    }
	 if(key=='2')
	 {
            initbnd(1); // circle
    }
   	 if(key=='3')
	 {
            initbnd(2); // quad
    }
   	 if(key=='4')
	 {
            initbnd(3); // tesla
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
   
	if (key==' ') 	initpar();
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|
	GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windW,windH);
	glutCreateWindow("LBM by Maciej Matyka");
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

