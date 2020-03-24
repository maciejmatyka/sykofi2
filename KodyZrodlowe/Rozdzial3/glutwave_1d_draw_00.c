

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <math.h>

#define CI_RED COLOR_RED
#define CI_ANTI_ALIAS_GREEN 16
#define CI_ANTI_ALIAS_YELLOW 32
#define CI_ANTI_ALIAS_RED 48



GLenum rgb, doubleBuffer=1, windType;
GLint windW, windH;

#include "tkmap.c"

GLenum mode=0;
GLint size;

#ifndef PI
#define PI 3.14159265358979323846
#endif




#define PX 0
#define PY 1
#define PZ 2


float point[3] = { 0.0, 0.0, 0.0 };

float point1[3] = { 0.0, 1.0, 0.0 };
float point2[3] = { 0.0, 1.0, 0.0 };


float t,dt;

#define WWIDTH 500
#define WHEIGHT	600

#define NX 2000
#define SWIDTH 1000
float DX=(SWIDTH/(float)NX);
float DT=0.05;
float VEL=77.8;

float U[NX+1]={0};		// U - wych.
float V[NX+1]={0};		// velocity
float T[NX+1]={0};		// teta



int	FLAG[NX+1]={0};		// komórki znaczone


#define C_BND    0x00000001 			/* boundary cells */
#define C_DONT   0x00000002             /* do not calculate U,T,E for that cells */					

int init=0,line=0,step=0,k=0;



static void Key(unsigned char key, int x, int y)
{

    switch (key) {
	case 'q':
		exit(0);

	break;

	case ' ':
		init=0;
		t=0;
		line=0;
		point1[PY]=point2[PY]=1.0;

	break;
      default:
	return;
        }

    glutPostRedisplay();
}

static void Init(void)
{
    GLint i;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    mode = GL_FALSE;
    size = 1;


// warunki brzegowe

	for(i=0;i<19;i++)
	{
		FLAG[i] |= C_DONT;
	 //	FLAG[NX-i] |= C_DONT;
	}

//	for(i=0;i<20;i++)
//		FLAG[i] |= C_DONT;


//	FLAG[NX/2-2] |= C_BND;
//	FLAG[NX/2-3] |= C_BND;

	for(i=0;i<20;i++)
	{
	   	//FLAG[i] |= C_BND;
		FLAG[NX-i] |= C_BND;
	}



}

static void Reshape(int width, int height)
{

    windW = (GLint)width;
    windH = (GLint)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-windW/2, windW/2, -windH/2, windH/2);
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}



static void Draw(void)
{
   int i;

if(init==0)
{
 	init=1;
	glClearColor(1.0, 1.0, 1.0, 0.0);      
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
}
//	AUX_SETCOLOR(windType, AUX_WHITE);     


	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     

	glColor3f(0.0,0.0,0.0);

	glPointSize(4);
	glLineWidth(2);     
	

step++;

/*if(step==60 && k<7)
{
	step=0;
	k++;
}
*/
if(step==60)
{
	step=0;
		
	point1[PX]=-1+(2/(float)NX);
	point1[PY]-=0.08;
	point2[PY]-=0.08;


	glBegin(GL_LINE_STRIP);     
	for(i=0;i<NX;i+=1)
 	{
	//	point1[PY]=U[i];
	//	point2[PY]=U[i+1];

		glVertex2f(point1[PX],point1[PY]+U[i]/8);     
		glVertex2f(point1[PX]+(2/(float)NX),point2[PY]+U[i+1]/8);     

		point1[PX]+=(2/(float)NX);

	}
	glEnd();      

}


/*
	point1[PX]=-1+(2/(float)NX);

 	glBegin(GL_POINTS);     
		for(i=1;i<NX;i++)
		{
			point1[PY]=U[i]*5;
			point1[PX]+=(2/(float)NX);
			glVertex3fv(point1);
	 	}
	glEnd();      
*/


/*
step++;
if(step==10)
{
	step=0;

	line++;

	point1[PX]=-1+(2/(float)NX);
	point1[PY]=1-(float)line/(float)WHEIGHT;


 	glBegin(GL_POINTS);     
	
		for(i=1;i<NX;i++)
		{
			glColor3f(fabs(U[i]*2),fabs(U[i]*4),fabs(U[i]*8));
			glVertex3fv(point1);
			point1[PX]+=(2/(float)NX);
		}
	glEnd();  
}
*/


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




void bndconditions(void)
{
	int i;

	for(i=0;i<NX;i++)
	if(FLAG[i] == C_BND)
	{
	
		/*	V[i]=V[i-1];
	   	T[i]=T[i-1];
	   	U[i]=U[i-1];  */
	   	V[i]=0;
	   	T[i]=0;
	   	U[i]=0;  
	
	}
}


void idle(void)
{
	int i;




t+=(PI/(float)180);


//if(t<PI) 

  
// U[NX/2]=sin(t)/3;
// U[NX/2-1]=sin(t)/3;
/// U[NX/2]=T[NX/2]=0;


 if(t<PI)
  	for(i=0;i<NX;i++)
	if(FLAG[i]==C_DONT)
	  V[i]=sin(t)/8;


		
		for(i=0;i<NX;i++)
		if( (FLAG[i] != (C_DONT|C_BND)) && (FLAG[i+1] != (C_DONT|C_BND)) )
		{		  
			V[i] = (V[i+1]+V[i-1])/2 + VEL*(DT/DX)*(T[i+1]-T[i]);
			T[i] = (T[i+1]+T[i-1])/2 + VEL*(DT/DX)*(V[i+1]-V[i]);

		}


 	bndconditions();


		for(i=0;i<NX;i++)
		if(FLAG[i] != (C_DONT|C_BND))
					U[i] = U[i]+V[i]*DT;			

 
	for(i=0;i<NX;i++)
		if( (FLAG[i] != (C_DONT|C_BND)) && (FLAG[i+1] != (C_DONT|C_BND)) )
				T[i] = (U[i+1]-U[i-1])/(2*DX);	
	
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    if (Args(argc, argv) == GL_FALSE) {
	exit(1);
    }

    windW = WWIDTH;
    windH = WHEIGHT;
    glutInitWindowPosition(150, 50); glutInitWindowSize( windW, windH);

    windType = (rgb) ? GLUT_RGB : GLUT_INDEX;
    windType |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
    glutInitDisplayMode(windType);

    if (glutCreateWindow("Wave 1D") == GL_FALSE) {
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
