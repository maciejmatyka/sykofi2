


/*

 cc point.c -lglut -lGLU -lGL -o point

 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <math.h>

#define CI_RED COLOR_RED
#define CI_ANTI_ALIAS_GREEN 16
#define CI_ANTI_ALIAS_YELLOW 32
#define CI_ANTI_ALIAS_RED 48

#ifndef PI
#define PI 3.147
#endif


GLenum rgb, doubleBuffer=1, windType;
GLint windW, windH;

#include "tkmap.c"

GLenum mode=0;
GLint size;

float point[3] = {
    1.0, 1.0, 0.0
};
float point2[3] = {
    1.0, 1.0, 0.0
};
float point3[3] = {
    1.0, 1.0, 0.0
};


double y=0,dy=0,v=-6.40,dv=0,t=0,dt=0.01,g=-9.8;
double k=0.4;

double y2=0,dy2=0,v2=-6.40,dv2=0;
double k2=0.2;

double y3=12,dy3=0,v3=0.40,dv3=0;
double k3=0.3;




int mmode;

double mass=3;
double mass2=2;
double mass3=0.6;


static void Key(unsigned char key, int x, int y)
{

    switch (key) {
	case 'q':
		v=40;	
	
	break;
	case 'w':
//		v2=20;		
	break;
	
	case '+':
		mass+=1;
	break;
	
	case ' ':
	
	if(mmode==0)
	    mmode=1;
	else
	    mmode=0;

	
	case '-':
		mass -=1;
	if(mass <= 0)
	     mass = 1;	    
	break;
	
      default:
	return;
        }

    glutPostRedisplay();
}

static void Init(void)
{
    GLint i;

    glClearColor(1.0, 1.0, 1.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    if (!rgb) {
	for (i = 0; i < 16; i++) {
	    glutSetColor(i+CI_ANTI_ALIAS_RED, i/15.0, 0.0, 0.0);
	    glutSetColor(i+CI_ANTI_ALIAS_YELLOW, i/15.0, i/15.0, 0.0);
	    glutSetColor(i+CI_ANTI_ALIAS_GREEN, 0.0, i/15.0, 0.0);
	}
    }

	point[0]=-0.55;
	point2[0]=0;
	point3[0]=0.55;

	point[1]=-0.85;
	point2[1]=0;
	point3[1]=0.55;




    mode = GL_FALSE;
    size = 1;
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



#define frac (double)26


void RysujSprezyne(double x1, double y1, double x2, double y2)
{
	double vsize;
	double wx1,wy1,wx2,wy2;
	double _wx1,_wy1,_wx2,_wy2;
	int i;


	
	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

	x2=(x2-x1);
	y2=(y2-y1);

	wx1=x1;
	wy1=y1;
	wx2=x1+(x2/vsize)*vsize*4/frac;
	wy2=y1+(y2/vsize)*vsize*4/frac;

	glBegin(GL_LINE_STRIP);
	glVertex2f(wx1,wy1);
	glVertex2f(wx2,wy2);
	glEnd();


	wx1 += (x2/vsize)*vsize*4/frac;
	wy1 += (y2/vsize)*vsize*4/frac;
	wx2 += (x2/vsize)*vsize*4/frac;
	wy2 += (y2/vsize)*vsize*4/frac;


	for(i=0;i<frac-(frac/2);i++)
	{

// wektor prostopadly do w
   	_wx1=wy1;
   	_wy1=-wx1;
   	_wx2=wy2;
   	_wy2=-wx2;

	vsize=sqrt((_wx2-_wx1)*(_wx2-_wx1)+(_wy2-_wy1)*(_wy2-_wy1));

	_wx1/=vsize;
	_wx2/=vsize;
	_wy1/=vsize;
	_wy2/=vsize;

	_wx2=_wx2-_wx1;
	_wy2=_wy2-_wy1;

//	_wx2/=2.f;
//	_wy2/=2.f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(wx1-_wx2/frac,wy1-_wy2/frac);
	glVertex2f(wx1+_wx2/frac+(x2/vsize)*vsize/frac,wy1+_wy2/frac+(y2/vsize)*vsize/frac);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(wx1+_wx2/frac+(x2/vsize)*vsize/frac,wy1+_wy2/frac+(y2/vsize)*vsize/frac);
	glVertex2f(wx1+_wx2/frac+(x2/vsize)*vsize*1.5/frac,wy1+_wy2/frac+(y2/vsize)*vsize*1.5/frac);
	glEnd();


	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));


		wx1 += (x2/vsize)*vsize/frac;
		wy1 += (y2/vsize)*vsize/frac;
		wx2 += (x2/vsize)*vsize/frac;
		wy2 += (y2/vsize)*vsize/frac;


/*		glBegin(GL_LINE_STRIP);
 		glVertex2f(wx1,wy1);
 		glVertex2f(wx2,wy2);
		glEnd();
*/

	}


	glBegin(GL_LINE_STRIP);
	glVertex2f(wx1,wy1);
	glVertex2f(wx2+ (x2/vsize)*vsize*4/frac,wy2+ (y2/vsize)*vsize*4/frac);
	glEnd();


}


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




#define MOVEY 0.25 
int sstep=0;
static void Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0,0,0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);


 

	glColor3f(0.3,0.5,0.7);
	RysujSprezyne(point[0],1-MOVEY,point[0], point[1]/(double)windH-MOVEY);

	glColor3f(0.4,0.4,0.3);
	RysujWektor(point[0],point[1]/(double)windH-MOVEY,point[0],point[1]/(double)windH-MOVEY-(k/mass)*y+g,0.05);

    glPointSize(2);
    glBegin(GL_POINTS);
	glVertex2f(point[0],point[1]/(double)windH-MOVEY);
    glEnd();




	glColor3f(0.3,0.5,0.7);
	RysujSprezyne(point2[0],1-MOVEY,point2[0], point2[1]/(double)windH-MOVEY);

	glColor3f(0.4,0.4,0.3);
	RysujWektor(point2[0],point2[1]/(double)windH-MOVEY,point2[0],point2[1]/(double)windH-MOVEY-(k2/mass2)*y2+g,0.05);

    glPointSize(2);
    glBegin(GL_POINTS);
	glVertex2f(point2[0],point2[1]/(double)windH-MOVEY);
    glEnd();





	glColor3f(0.3,0.5,0.7);
	RysujSprezyne(point3[0],1-MOVEY,point3[0], point3[1]/(double)windH-MOVEY);

 	glColor3f(0.4,0.4,0.3);
	RysujWektor(point3[0],point3[1]/(double)windH-MOVEY,point3[0],point3[1]/(double)windH-MOVEY-(k3/mass3)*y3+g,0.05);

    glPointSize(2);
    glBegin(GL_POINTS);
	glVertex2f(point3[0],point3[1]/(double)windH-MOVEY);
    glEnd();

 

	glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);
   	glVertex2f(-0.75, 1 - MOVEY);
 	glVertex2f(0.75, 1 - MOVEY);
	glColor3f(0.3,0.2,0.1);
 	glVertex2f(0.75, 1 - MOVEY+0.15);
 	glVertex2f(-0.75, 1 - MOVEY+0.15);

  
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


    dv=(-(k/mass)*y+g)*dt;
    v+=dv;
    dy=v*dt;
    y=y+dy;

    dv2=(-(k2/mass2)*y2+g)*dt;
    v2+=dv2;
    dy2=v2*dt;
    y2=y2+dy2;

    dv3=(-(k3/mass3)*y3+g)*dt;
    v3+=dv3;
    dy3=v3*dt;
    y3=y3+dy3;



    point[1] = y;
    point2[1] = y2;
    point3[1] = y3;
    
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

    if (glutCreateWindow("Point Test") == GL_FALSE) {
	exit(1);
    }

    InitMap();

    Init();

    glutKeyboardFunc(Key);
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(idle);
    glutMainLoop();
    
	return 0;
}
