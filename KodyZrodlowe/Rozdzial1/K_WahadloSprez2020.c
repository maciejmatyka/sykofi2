/*
WahadloSprez.c 

Rozwi¹zanie numeryczne wahad³a matematycznego metod¹ Eulera.
Napisane na potrzeby ksi¹¿ki "Symulacje Komputerowe w Fizyce"

Wersja poprawiona (2020, drugie wydanie ksi¹¿ki).
W tej wersji poprawione zosta³y si³y i model masy zawieszonej na wahadle
ze sprê¿yn¹ (poprzednio model zosta³ Ÿle zaprojektowany).

autor: Maciej Matyka, 2001 r.
poprawione: Maciej Matyka, 2020 r.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <math.h>

#define PI 3.14

GLenum rgb, doubleBuffer=1, windType;
GLint windW, windH;
GLenum mode=0;

float point[2] = { 1.0, 1.0 };

double omega,teta,dt,g,r,t,m;
double pr,pvr,k,pr0;
double pvx,pvy;

static void Init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
    mode = GL_FALSE;

/* Sta³e */
	dt=0.01f;
	g=-9.8f;
	k=4.5;
	m=0.3;
	point[0] = 0.1;
    point[1] = -0.3;

/* Warunki pocz¹tkowe rozwi¹zania numerycznego */

//	pvr=0.2;
	pvx=2.5;
	pvy=0.0;

	t=0;

	pr0=sqrt(point[0]*point[0] + point[1]*point[1]);
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


#define MOVEY 0.9

void RysujWektor(double x1, double y1, double x2, double y2, double scale, double px, double py)
{
		double kx,ky,kprimx,kprimy,vsize,ksize;

// Wylicz d³ugoœæ wektora v i ustal d³ugoœæ wektora k

	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	ksize=vsize*scale/4.f;


// Rysuj podstawê strza³ki


    glBegin(GL_LINE_STRIP);

//	glVertex2f(x1,y1);
//	glVertex2f(((x2-x1)/vecsize),((y2-y1)/vecsize));

 	glVertex2f(x1+px,y1+py);
 	glVertex2f(x1+(x2-x1)*scale+px,y1+(y2-y1)*scale+py);

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

	glVertex2f(x1+(x2-x1)*scale+px,y1+(y2-y1)*scale+py);
	glVertex2f(x1+(x2-x1)*scale+kprimx+px,y1+(y2-y1)*scale+kprimy+py);

    glEnd();

// obróæ k o -30 stopni

   	kprimx=kx*cos(-35*PI/180.f)+ky*sin(-35*PI/180.f);
   	kprimy=-kx*sin(-35*PI/180.f)+ky*cos(-35*PI/180.f);

// wymnó¿ przez jego d³ugoœæ

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex2f(x1+(x2-x1)*scale+px,y1+(y2-y1)*scale+py);
	glVertex2f(x1+(x2-x1)*scale+kprimx+px,y1+(y2-y1)*scale+kprimy+py);

    glEnd();

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



static void Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);


 	glColor3f(1.0,1.0,1.0);
	glLineWidth(1);


    glPointSize(24);

    glBegin(GL_POINTS);
	glVertex2f(point[0], point[1]+MOVEY);
    glEnd();

/*    glBegin(GL_LINE_STRIP);
	glVertex2f(0, 0 + MOVEY);
	glVertex2f(point[0], point[1]+MOVEY);
    glEnd();
*/

	glLineWidth(2);
	RysujSprezyne(0,0+MOVEY,point[0],point[1]+MOVEY);



	glLineWidth(3);
	glColor3f(0.6,0.6,0.5);
	//RysujWektor(point[0],point[1]+MOVEY,pr*sin(teta+omega*dt),pr*cos(teta+omega*dt)+MOVEY,15.5f,0,0);
	RysujWektor(point[0],point[1]+MOVEY,point[0],point[1]+MOVEY+g*0.002,15.5f,0,0);

	glColor3f(0.6,0.7,0.8);
	RysujWektor(0,0+MOVEY,point[0],point[1]+MOVEY,0.05*(-k*(pr-pr0)),point[0],point[1]);



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
	double Fs;

	t=t+dt;

    float x = point[0];
    float y = point[1];
    float r = sqrt(x*x + y*y);
    float nx = x/r;            // wektor wzdluz sprezyny
    float ny = y/r;

    pr = r;
	Fs=-k*(pr-pr0);            // sprezyna (wartosc)
	float Fg_y=m*g;            // sila grawitacji (wartosc y)
	
	float Fs_x = Fs * nx;
	float Fs_y = Fs * ny;
	
	float Fx = Fs_x;           // sila wypadkowa
    float Fy = Fs_y + Fg_y;
    
    pvx = pvx + (Fx/m)*dt;
    pvy = pvy + (Fy/m)*dt;

    point[0] = point[0] + pvx * dt;
    point[1] = point[1] + pvy * dt;
    
    //pvr=pvr+Fs*dt;
	//pr=pr+pvr*dt;
	
	
	
	
	

	//omega = omega + (g+fabs(Fs)*cos(teta)/pr)*sin(teta)*dt;
	//omega = omega + (g+Fs*cos(teta)/pr)*sin(teta)*dt;
	//omega = omega + ((g+Fs)*cos(teta)/pr)*sin(teta)*dt;
 	//teta  = teta + omega * dt;

	//point[0] = pr*sin(teta);
    //point[1] = pr*cos(teta);


 //	printf("%f\n",teta);



	// F=-k(x0-x)
	//

	//pvr=pvr+Fs*dt;
	//pr=pr+pvr*dt;


    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    if (Args(argc, argv) == GL_FALSE) {
	exit(1);
    }

    windW = 600;
    windH = 600;
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
