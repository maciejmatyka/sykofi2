/**
 * LBM 3D
 * Kod powstal na potrzeby ilustracji rozdzialu w drugim wydaniu
 * ksiazki Symulacje Komputerowe w Fizyce (Helion)
 * Wroclaw, 2019-02-16
 */

#include <iostream>
#include <fstream>
using namespace std;
#include <cstdlib>
#include <iomanip>
#include <GL/glut.h>
#include "lbm.h"
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
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}
int c = 1;
void simulationstep(void)
{
//   static int c = 1;
   c=1-c;
 	if(lbmonoff)   lbm(c);      				// DO LBM STEP
}

void idleFunction(void)
{
    static int s = 0;
    cout << s++ << endl;
   if(pause==0)
   {
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
   }
	glutSwapBuffers();
}
void init(void)
{
	initlbm();								// INIT LBM
}

void exportvtk(void)
{
/*
1. # vtk DataFile Version 2.0
2. Komentarz - nasze pole prÄ™dkoÅ›ci policzone metodÄ… LBM
3. ASCII
4. DATASET STRUCTURED_POINTS
5. DIMENSIONS 4 2 1
6. ORIGIN 0 0 0
7. SPACING 1 1 1
8. POINT_DATA 8
10. VECTORS PolePredkosci double
11. 1.0 0.0 0.0*/

	ofstream file("velocity.vtk");
	file << "# vtk DataFile Version 2.0\nLBM, Symulacje Komputerowe w Fizyce 2, Maciej Matyka 2019\n";
	file << "ASCII\nDATASET STRUCTURED_POINTS\n";
	file << "DIMENSIONS " << W << " " << H << " " << D << " \n";
	file << "ORIGIN 0 0 0\nSPACING 1 1 1\n";
	file << "POINT_DATA " << W*H*D << "\n";
	file << "VECTORS PolePredkosci double\n";
    file <<  std::setprecision(10);
	for(int m=0; m<D; m++)
	for(int j=0; j<H; j++)
	for(int i=0; i<W; i++)
	{
        if(F[i][j][m]==0)
    		file << UX[i][j][m] << " " << UY[i][j][m] << " " << UZ[i][j][m] << endl; 
    	else
        	file << "0 0 0" << endl;
	}

	file.close();

	// zapisz gestosc-1 (do cisnienia)
	ofstream file2("density.vtk");
	file2 << "# vtk DataFile Version 2.0\nLBM, Symulacje Komputerowe w Fizyce 2, Maciej Matyka 2019\n";
	file2 << "ASCII\nDATASET STRUCTURED_POINTS\n";
	file2 << "DIMENSIONS " << W << " " << H << " " << D << " \n";
	file2 << "ORIGIN 0 0 0\nSPACING 1 1 1\n";
	file2 << "POINT_DATA " << W*H*D << "\n";
	file2 << "SCALARS Gestosc double\n";
	file2 << "LOOKUP_TABLE default\n";

	for(int m=0; m<D; m++)
	for(int j=0; j<H; j++)
	for(int i=0; i<W; i++)
	{

		if(!F[i][j][m])
			file2 << ((R[i][j][m]-1.0)*10000.0) << endl; 
		else
			file2 << 0 << endl;
	}
	file2.close();
}

void keys(unsigned char key, int x, int y) 
{
	if (key == 27)		exit(0);
	if(key=='o')  	lbmonoff=1-lbmonoff;
	if(key=='-')  	dt/=2.0;
    if(key=='+')  	dt*=2.0;
    if(key=='p')	pause = 1-pause;   

    if(key=='s')    exportvtk();
    if(key==',') {mnoznik_alpha/=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;}
    if(key=='.') 
	 {
		mnoznik_alpha*=2; cout << "#mnoznik_alpha: " << mnoznik_alpha << endl;
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
   

    
    /*if(key== '-') fx /=2.0;//(1e-8);
    if(key== '+') fx *=2.0;//(1e-8);
    if(key== '0') fx =(0);
   
   if(key== '.') 
	 {
		int x = W*(rand()/float(RAND_MAX));
		int y = H*(rand()/float(RAND_MAX));
		
		UX[c][x][y] += 0.1*( rand()/float(RAND_MAX)-0.5);
		UY[c][x][y] += 0.1*( rand()/float(RAND_MAX)-0.5);
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
	glutInitWindowSize(W*SCALE,H*SCALE);
	glutCreateWindow("LBM 3D");
	init();						
	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keys);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunction);
	// enter GLUT event processing cycle
	glutMainLoop();
}

