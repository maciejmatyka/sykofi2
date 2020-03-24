

/*


Wahadlo2.c 

Rozwi¹zanie numeryczne wahad³a matematycznego metod¹ Eulera.
Napisane na potrzeby ksi¹¿ki "Symulacje Komputerowe w Fizyce"

Figury Lisajous.

autor: Maciej Matyka, 2001 r.

 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <math.h>

#define PI 3.14

GLenum rgb, doubleBuffer=1, windType;
GLint windW, windH;

//#include "tkmap.c"

GLenum mode=0;

float point[2] = { 1.0, 1.0 };
float point2[2] = { 1.0, 1.0 };


double omega,teta,omega2,teta2,dt,g,r1,r2,t;


static void Init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    mode = GL_FALSE;


/* Sta³e */

	dt=0.01f;
	g=9.8f;
	r1=0.9;
	r2=0.4;


/* Warunki pocz¹tkowe rozwi¹zania numerycznego */

	t=0;
	omega=0.0f;
	teta=4.0f;					 // x

	omega2=0.0f;
	teta2=2.0f;				 // y



}

static void Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}


#define MOVEY 0.5


void RysujWektor(double x1, double y1, double x2, double y2, double scale)
{
		double kx,ky,kprimx,kprimy,vsize,ksize;

// Wylicz d³ugoœæ wektora v i ustal d³ugoœæ wektora k

	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	ksize=vsize*scale/4.f;


// Rysuj podstawê strza³ki


    glBegin(GL_LINE_STRIP);

//	glVertex2f(x1,y1);
//	glVertex2f(((x2-x1)/vecsize),((y2-y1)/vecsize));

 	glVertex2f(x1,y1);
 	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);

    glEnd();


// wyznacz wektor jednostkowy skierowany naprzeciwko v

	kx=-(x2-x1)/vsize;
	ky=-(y2-y1)/vsize;

// obróæ k o 30 stopni

   	kprimx=kx*cos(35*PI/180.f)+ky*sin(35*PI/180.f);
   	kprimy=-kx*sin(35*PI/180.f)+ky*cos(35*PI/180.f);

// wymnó¿ przez jego d³ugoœæ

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);
	glVertex2f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy);

    glEnd();

// obróæ k o -30 stopni

   	kprimx=kx*cos(-35*PI/180.f)+ky*sin(-35*PI/180.f);
   	kprimy=-kx*sin(-35*PI/180.f)+ky*cos(-35*PI/180.f);

// wymnó¿ przez jego d³ugoœæ

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);
	glVertex2f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy);

    glEnd();

}


#define MOVEPX1 (-0.25)
#define MOVEPX2	(0.25)


static void Draw(void)
{
 	float dy=0;
	int i;



	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);


    glClear(GL_COLOR_BUFFER_BIT);


 	glColor3f(0.0,0.0,0.0);
	glLineWidth(1);


    glPointSize(4);

    glBegin(GL_POINTS);
	glVertex2f(point[0]+MOVEPX1, point[1]+MOVEY);
    glEnd();

    glBegin(GL_POINTS);
	glVertex2f(point2[0]+MOVEPX2, point2[1]+MOVEY);
    glEnd();


    glBegin(GL_LINE_STRIP);
	glVertex2f(0+MOVEPX1, 0 + MOVEY);
	glVertex2f(point[0]+MOVEPX1, point[1]+MOVEY);
    glEnd();

    glBegin(GL_LINE_STRIP);
	glVertex2f(MOVEPX2, 0 + MOVEY);
	glVertex2f(point2[0]+MOVEPX2, point2[1]+MOVEY);
    glEnd();


	glLineWidth(3);
	glColor3f(0.5,0.5,0.4);

	RysujWektor(point[0]+MOVEPX1,point[1]+MOVEY,r1*sin(teta+omega*dt)+MOVEPX1,r1*cos(teta+omega*dt)+MOVEY,15.5f);
	RysujWektor(point2[0]+MOVEPX2,point2[1]+MOVEY,r2*sin(teta2+omega2*dt)+MOVEPX2,r2*cos(teta2+omega2*dt)+MOVEY,15.5f);

	glColor3f(0.7,0.7,0.8);

	RysujWektor(point[0]+MOVEPX1,point[1]+MOVEY,point[0]+MOVEPX1,point[1]+MOVEY-0.3,1);
	RysujWektor(point2[0]+MOVEPX2,point2[1]+MOVEY,point2[0]+MOVEPX2,point2[1]+MOVEY-0.3,1);


	glLineWidth(1);
	glColor3f(0.4,0.4,0.4);


    glBegin(GL_QUADS);
   	glVertex2f(-0.5, 0 + MOVEY);
 	glVertex2f(0.5, 0 + MOVEY);
 	glVertex2f(0.5, 0 + MOVEY+0.05);
 	glVertex2f(-0.5, 0 + MOVEY+0.05);
    glEnd();





    glFlush();


    if (doubleBuffer) {
	glutSwapBuffers();
    }
}

static GLenum Args(int argc, char **argv)
{
    rgb = GL_TRUE;
    doubleBuffer = GL_TRUE;

    return GL_TRUE;
}


void idle(void)
{
	t=t+dt;

// m=1

 //   printf("%f     %f       %f      %f\n",t,((omega*omega)*(r*r)/2.f)*10,(9.8 + g*point[1])*10,( (omega*omega)*(r*r)/2.f + 9.8 + g*point[1])*10);     


   //	printf("%f %f\n",teta,teta2);

	omega = omega + (g/r1)*sin(teta)*dt;
	teta  = teta + omega * dt;

	omega2 = omega2 + (g/r2)*sin(teta2)*dt;
	teta2  = teta2 + omega2 * dt;
    
    point[0] = r1*sin(teta);
    point[1] = r1*cos(teta);

    point2[0] = r2*sin(teta2);
    point2[1] = r2*cos(teta2);


    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    if (Args(argc, argv) == GL_FALSE) {
	exit(1);
    }

    windW = 300;
    windH = 300;
    glutInitWindowPosition(0, 0); glutInitWindowSize( windW, windH);

    windType = (rgb) ? GLUT_RGB : GLUT_INDEX;
    windType |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
    glutInitDisplayMode(windType);

    if (glutCreateWindow("Wahad³o") == GL_FALSE) {
	exit(1);
    }

    //InitMap();

    Init();

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(idle);
    glutMainLoop();
    
	return 0;
}
