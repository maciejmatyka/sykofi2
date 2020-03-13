
/**
 * Waves
 *
 * by Maciej Matyka,
 * Bremen, 6-12-2009
 *
 * http://panoramix.ift.uni.wroc.pl/~maq/eng/
 * (or http://www.matyka.pl)
 *
 */
 
 /** Rules
 *
 * 1. Use at your own risk.
 * 2. It's free 4 learning.
 * 3. You like it? You may donate my ferrari. Follow PayPal link at my home page.
 *
 * Enjoy!
 */

/**
 * @todo: clear opengl part
 */

#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

// computational mesh

const int NX = 55;
const int NY = 55;

double U[NX+1][NY+1];
double E[NX+1][NY+1];
int Flag[NX+1][NY+1];

// numerical constants

const float DT = 0.045;    // time dt
const float DX = 0.1;       // spatial dx
const float FREQ = 0.5;    // pulsating source frequency

const float EPS = 0.333;    // laplace operator
const int C_SRC = 2;        // pulsating source cell flag
const int C_BND = 1;        // bundary cell flag

// physical timing

double t=0; 			// actual time in idle loop
double realdt; 			// time elapsed in idle loop


// init (num = configuration number)

void init_waves(int num)
{
	// clear flags & fields
	for(int i=0; i<NX; i++)
	for(int j=0; j<NY; j++)
	{
		Flag[i][j] = 0;
		U[i][j] = E[i][j] = 0;
	}

	// sqitch between different configs

	switch(num)
	{

		case 1:     // source + a gap
			// place wall with small gap
			for(int i=0; i<NX; i++)
				if( fabs(i-NX/2) >= 2 )
				    Flag[i][NY/4] = C_BND;

			// place 'sources'
			for(int i=0; i<NX; i++)
				if( fabs(i-NX/2) <= 1 )
				    Flag[i][NY/8] = C_SRC;
		break;

		case 2:     // source in the middle
		    Flag[NX/2][NY/2] = C_SRC;
		    Flag[NX/2][NY/2+1] = C_SRC;
		    Flag[NX/2][NY/2-1] = C_SRC;
		    Flag[NX/2+1][NY/2] = C_SRC;
		    Flag[NX/2-1][NY/2] = C_SRC;
		break;

		case 3:     // interference of two sources
		    Flag[NX/2][NY/2] = C_SRC;
		    Flag[NX/2][NY/2+1] = C_SRC;
		    Flag[NX/2][NY/2-1] = C_SRC;
		    Flag[NX/2+1][NY/2] = C_SRC;
		    Flag[NX/2-1][NY/2] = C_SRC;

		    Flag[NX/4][NY/4] = C_SRC;
		    Flag[NX/4][NY/4+1] = C_SRC;
		    Flag[NX/4][NY/4-1] = C_SRC;
		    Flag[NX/4+1][NY/4] = C_SRC;
		    Flag[NX/4-1][NY/4] = C_SRC;
		break;


		default:
			break;

	}
}

// idle - simulation step

static void idle(void)
{
    realdt = glutGet(GLUT_ELAPSED_TIME) / 100.0 - t;
    t = glutGet(GLUT_ELAPSED_TIME) / 100.0;

	int i,j;
	float x,y;

	// solve equation for U
	// dU/dt = nabla^2 E

	for(j=1;j<NY;j++)
	for(i=1;i<NX;i++)
	{
		U[i][j]=U[i][j] + 
		(
		(1-EPS) * (E[i+1][j]+E[i-1][j]+E[i][j-1]+E[i][j+1]-4*E[i][j])
		+ EPS   * (E[i+1][j+1]+E[i+1][j-1]+E[i-1][j-1]+E[i-1][j+1]-4*E[i][j])
		)*(realdt*DT)/(DX*DX);
	}

	// solve equation for E
	// dE/dt = U

	for(j=0;j<=NY;j++)
	for(i=0;i<=NX;i++)
	{
		E[i][j] = E[i][j]+U[i][j] * (realdt*DT);
	}

	// start boundaries: pulsating sources

	const float AMPLITUDE = 0.01;

	for(j=0;j<=NY;j++)
	for(i=0;i<=NX;i++)
		if(Flag[i][j] & C_SRC)
			E[i][j]= sin( t*FREQ ) * AMPLITUDE;

	// boundaries

	for(i=0;i<=NX;i++)
	for(j=0;j<=NX;j++)
		if(Flag[i][j] & C_BND)
			E[i][j]=U[i][j]=0.0f;

	// rigid wall 1

	for(i=0;i<=NX;i++)
	{
		U[i][NY]=U[i][NY-1]=0;
		E[i][NY]=E[i][NY-1]=0;

		U[i][0]=U[i][1]=0;
		E[i][0]=E[i][1]=0;

	}

	// rigid wall 2

	for(j=0;j<=NY;j++)
	{
		U[0][j]=U[1][j]=0;
		E[0][j]=E[1][j]=0;

		U[NX][j]=U[NX-1][j]=0;
		E[NX][j]=E[NX-1][j]=0;
	}


	glutPostRedisplay();
}

// --------------------------------------------------
//
// OpenGL stuff & visualisation
//
// --------------------------------------------------


// mouse handling

int mousedown[2] = {0}, prevx[2]={0}, prevy[2]={0};
float posx[2]={0}, posy[2]={0}, translatex[2]={0}, translatey[2]={0};
#define L 0
#define R 1

// model rotation & position

float rotx=-65;
float roty=0;
float rotz=0;
float scale=1.0;
float trx=-0.5;
float tr_y=-0.3;
float trz=-2.6;
int mode=1;
int axisonoff=1;
float fov=15.0;

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 1111125);
	gluPerspective(fov,ar,0,10);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void *font = GLUT_BITMAP_TIMES_ROMAN_10;
void GLprintf(float x, float y,float z, char *string)
{
  int len, i;

  glRasterPos3f(x, y, z);
  len = (int) strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(font, string[i]);
  }
}



void draw_axises()
{
	float a=0.15;
	glPushMatrix();
	glTranslatef(-a/2,-a/2,-a/2);
	if(axisonoff>0)
	{
		glLineWidth(2);
		glColor3f(1.0,0,0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0,0,0);
			glVertex3f(a,0,0);
		glEnd();
		glColor3f(0.0,1.0,0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0,0,0);
			glVertex3f(0,a,0);
		glEnd();
		glColor3f(0.0,0,1.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0,0,0);
			glVertex3f(0,0,a);
		glEnd();

	glColor3f(1.0,0,0.0);
	GLprintf(a+0.02, 0, 0, "[X]");
	glColor3f(0.0,1.0,0.0);
	GLprintf(0, a+0.02, 0, "[Y]");
	glColor3f(0.0,0,1.0);
	GLprintf(0, 0, a+0.02, "[Z]");
	}
	glPopMatrix();
}


float itoscreen(float i)
{
	return ((i/(float)NX));
}

float countE(int i,int j)
{
	const float AMPL = 3.0;
	return E[i][j] * AMPL;
}

static void display(void)
{
//    const double a = t*90.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



	//glClearColor(0.7,0.7,0.7,0);
	glClearColor(0.0,0.0,0.0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,0,0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_LESS);


	glPushMatrix();
		glTranslatef(trx, tr_y, trz);
		glRotated(rotx,1,0,0);
		glRotated(roty,0,1,0);
		glRotated(rotz,0,0,1);

		glScalef(scale,scale,scale);

		glColor3f(1,1,1);
		GLprintf(0, 1, 0.3, "Press: 1,2,3");
	glPopMatrix();

	int i,j;
	float x,y;

	const float MOUSE_T_X = 0.005;
	const float MOUSE_T_Y = -0.005;
	const float MOUSE_R_X = 0.05;
	const float MOUSE_R_Y = -0.05;

	glTranslatef(trx+(posx[L]+translatex[L])*MOUSE_T_X, tr_y+(posy[L]+translatey[L])*MOUSE_T_Y, trz);

//    	glRotated(80,1,0,0);
	glRotated(rotx+(posy[R]+translatey[R])*MOUSE_R_Y,1,0,0);
	glRotated(roty+(posx[R]+translatex[R])*MOUSE_R_X,0,1,0);
	glRotated(rotz,0,0,1);

	glScalef(scale,scale,scale);

	glPointSize(1.0);
	glLineWidth(1.0);
	glColor3f(1.0,1.0,1.0);

	for(j=0; j<NY; j+=1)
	for(i=0; i<NX; i+=1)
	{
		
		#define st 0.3
		#define sf 20
		/*
		glBegin(GL_TRIANGLES);
			glColor3f(st+fabs(countE(i,j))*sf,st+fabs(countE(i,j))*sf,st+fabs(countE(i,j))*sf);
			glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));

			glColor3f(st+fabs(countE(i+1,j))*sf,st+fabs(countE(i+1,j))*sf,st+fabs(countE(i+1,j))*sf);
			glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));

			glColor3f(st+fabs(countE(i+1,j+1))*sf,st+fabs(countE(i+1,j+1))*sf,st+fabs(countE(i+1,j+1))*sf);
			glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
		glEnd();
		glBegin(GL_TRIANGLES);
			glColor3f(st+fabs(countE(i+1,j+1))*sf,st+fabs(countE(i+1,j+1))*sf,st+fabs(countE(i+1,j+1))*sf);
			glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));

			glColor3f(st+fabs(countE(i,j+1))*sf,st+fabs(countE(i,j+1))*sf,st+fabs(countE(i,j+1))*sf);
			glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

			glColor3f(st+fabs(countE(i,j))*sf,st+fabs(countE(i,j))*sf,st+fabs(countE(i,j))*sf);
			glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glEnd();
		*/



		glColor3f(1,1,1);
		glBegin(GL_LINE_STRIP);
			glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
			glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
			glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
			glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));
			glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glEnd();

	}

	float d;
	for(j=1; j<NY; j+=1)
	for(i=1; i<NX; i+=1)
	if(Flag[i][j] & (C_SRC|C_BND))
	{
		if(Flag[i][j] & C_SRC)
		{
   			glColor3f(1,0,0);
			d=0.19;
		}

		if(Flag[i][j] & C_BND)
		{
   			glColor3f(1,0.8,0.4);
			d=0.5;
		}

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(itoscreen(i-d),itoscreen(j+d),countE(i,j));
			glVertex3f(itoscreen(i+d),itoscreen(j+d),countE(i,j));
			glVertex3f(itoscreen(i+d),itoscreen(j-d),countE(i,j));
			glVertex3f(itoscreen(i-d),itoscreen(j-d),countE(i,j));
			glVertex3f(itoscreen(i-d),itoscreen(j+d),countE(i,j));
		glEnd();
	}

	draw_axises();

	// bounding box
	glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 27 : 
        case 'q':
            exit(0);
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
			init_waves(atoi( (const char * )&key));
           break;

        case '+':
			scale+=0.1;
            break;
        case '-':
			scale-=0.1;
			cout << scale << endl;
            break;

		default:
			cout << int(key) << endl;
			break;
	}

    glutPostRedisplay();
}



void Mouse (int button, int state, int x, int y)
{
	int K;

	if (button == GLUT_LEFT_BUTTON) K=L;
	else if (button == GLUT_RIGHT_BUTTON) K=R;

	if (state == GLUT_DOWN)
	{
		mousedown[K] = 1;
		prevx[K]=x;
		prevy[K]=y;
	} else if (state == GLUT_UP)
	{
		mousedown[K] = 0;
		posx[K] = posx[K]+translatex[K];
		posy[K] = posy[K]+translatey[K];
		translatex[K] = 0;
		translatey[K] = 0;
	}



}

/* Mouse Motion */
void Motion (int x, int y)
{
	if (mousedown[L])
	{
		translatex[L] = (x-prevx[L]);
		translatey[L] = (y-prevy[L]);
		glutPostRedisplay();
	}

	if (mousedown[R])
	{
		translatex[R] = (x-prevx[R]);
		translatey[R] = (y-prevy[R]);
		glutPostRedisplay();
	}

}



/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);


    glutCreateWindow("Waves by Maciej Matyka (2009)");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

    glClearColor(1,1,1,1);

	init_waves(1);
	
    glutMainLoop();

    return EXIT_SUCCESS;
}

