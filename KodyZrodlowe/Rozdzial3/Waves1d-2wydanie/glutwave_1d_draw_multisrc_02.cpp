/**
Rozwiązanie numeryczne fali jednowymiarowej.
Wersja do rysowania wykresów dla książki.

Zagadnienie rozwiązane z użyciem schematu centralnego
aproksymacji 2 pochodnej przestrzennej.
Symulacje Komputerowe w Fizyce, Wydanie 2, Helion 2020

Maciej Matyka, 2020-04-20

ps. podziękowania dla autora erraty do wydania 1 dzięki
któremu udało się naprawić błędy związane z oryginalnym podejściem.
**/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
using namespace std;
GLint windW, windH;
GLenum mode=0;
#ifndef PI
#define PI 3.14159265358979323846
#endif
float t = 0;

#define WWIDTH 500
#define WHEIGHT	600
//int init=0,line=0,step=0,k=0;

// computational mesh

const int NX = 500;
const float v2 = 0.01; // prędkość^2 fali
double U[NX+1];
double E[NX+1];             // dla lepszej czytelności kodu zmianę wychylenia w czasie oznaczamy literką
int Flag[NX+1];             // E zamiast literką V jak zapisano w książce
double srcF[NX+1];             // src frequency
double srcP[NX+1];             // src phase
double srcA[NX+1];             // src amplitude

// numerical constants

const float DT = 0.045;    // krok czasowy
const float DX = 0.5;      // rozmiar oczka siatki obliczeniowej
const float FREQ = 0.01;    // pulsating source frequency
const float AMPLITUDE = 0.1;
const int C_SRC = 2;        // pulsating source cell flag
const int C_BND = 1;        // bundary cell flag

// vis
float point1[3] = { 0.0, 1.0, 0.0 };
float point2[3] = { 0.0, 1.0, 0.0 };


// A - Inicjalizuj tablice wychyleń oraz oznaczenia
// komórek (oznaczenia jak w książce)

void init_waves()
{
    // zdejmij flagi SRC
   for(int i=0; i<NX; i++)
   {
    Flag[i] &= ~(C_SRC);
    U[i]=E[i] = 0;
   }

    // dołóż losowe
    int liczba = rand() % 100;
    for(int i=0; i<liczba; i++)
    {
//        Flag[i * NX/100] = C_SRC;
        int poz = rand() % NX;
        Flag[  poz  ] = C_SRC;
        /*srcF[ poz ]  = (rand()/(float)(RAND_MAX)) * 0.02;
        srcA[ poz ]  = (rand()/(float)(RAND_MAX)) * 0.15;
        srcP[ poz ]  = 0;//(rand()/(float)(RAND_MAX)) * 2*3.14;*/
    }

    t=0;
}

// pętla symulacji
static void idle(void)
{
	int i,j;
	float x,y;

	// przyrost czasu
	t = t + DT;

	// B - źródło fali
	for(i=0;i<=NX;i++)
    {
        if(Flag[i] & C_SRC)
        {
            if(0&&t>PI/FREQ)      // czas trwania imulsu minął?
                Flag[i] &= (~C_SRC); // zdejmij flagę komórki źródłowej
            else
                U[i] = sin( t*FREQ ) * AMPLITUDE;
                //U[i] = sin( t*srcF[i] + srcP[i] ) * srcA[i];
        }
    }

    // C - oblicz nową wartość wychylenia i jego pochodnej E
	for(i=0;i<=NX;i++)
    {
       if(Flag[i] == 0)            // tylko komórki puste - nie źródłowe i nie brzegowe
          E[i] = E[i] + v2*DT*(U[i+1] - 2*U[i] + U[i-1])/(DX*DX);
    }
	for(i=0;i<=NX;i++)
    {
       if(Flag[i] == 0)            // tylko komórki puste - nie źródłowe i nie brzegowe
        U[i] = U[i] + E[i] * DT;
    }

    // D- warunki brzegowe
	for(i=0;i<=NX;i++)
    {
      if(Flag[i] & C_BND)
      {
        U[i] = E[i] = 0.0f;
      }
    }


	glutPostRedisplay();
}





static void Key(unsigned char key, int x, int y)
{
    switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		//t=0;
		init_waves();
		break;
	default:
		break;
        }

    glutPostRedisplay();
}

static void Init(void)
{
    GLint i;
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    init_waves();
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

int step = 0;
int init = 0;

void drawwave()
{
    point1[0]=-1+(2/(float)NX);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<NX;i+=1)
    {
    //	point1[PY]=U[i];
    //	point2[PY]=U[i+1];

        glVertex2f(point1[0],point1[1]+U[i]/8);
        glVertex2f(point1[0]+(2/(float)NX),point2[1]+U[i+1]/8);

        point1[0]+=(2/(float)NX);

    }
    glEnd();
}

#define ANIMATION

static void Draw(void)
{
    #ifdef ANIMATION
    int i;
	glClearColor(1.0, 1.0, 1.0, 0.0);
  	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);

	glColor3f(0.0,0.0,0.0);
	glLineWidth(2);

	point1[0]=-1+(2/(float)NX);

	// E - rysuj falę na podstawie obliczonych
	//     wychyleń E

		for(i=1;i<NX;i++)
		{
            glPointSize(2);
		    glColor4f(0,0,0,0);
            glBegin(GL_POINTS);
			point1[0]+=(2/(float)NX);
			point1[1]=U[i];
			glVertex3fv(point1);
            glEnd();

            if(Flag[i] & C_SRC)     // rysuj komórki źródłowe na czerwono
            {
                glPointSize(4);
                glColor4f(1,0,0,0);
                glBegin(GL_POINTS);
                point1[0]+=(2/(float)NX);
                point1[1]=U[i];
                glVertex3fv(point1);
                glEnd();
            }
	 	}
	glutSwapBuffers();

    #endif // ANIMATION

    #ifndef ANIMATION               // rysuj statycznie
   int i;

/*    if(init==0)
    {
        init=1;
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
*/
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glColor3f(0.0,0.0,0.0);
	glPointSize(4);
	glLineWidth(2);

/*if(step==60 && k<7)
{	step=0;
	k++;}
*/
    /*glColor4f(0.0,0.0,0.0,0.5);
    glRotatef(step*20,0,0,1);
    glBegin(GL_LINES);//_STRIP);
		glVertex2f(0,0);
        glVertex2f(1,1);
	glEnd();*/
    step++;


if(step==5660)
{
	step=0;
	point1[1]-=0.08;
	point2[1]-=0.08;
    drawwave();
    glutSwapBuffers();
    drawwave();
    glutSwapBuffers();

}
else
    glutSwapBuffers();

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



//---------------------------------------------------------------------------------------------------------------
  /* int i;
	glClearColor(1.0, 1.0, 1.0, 0.0);
  	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);

	glColor3f(0.0,0.0,0.0);
	glLineWidth(2);

	point1[0]=-1+(2/(float)NX);

	// E - rysuj falę na podstawie obliczonych
	//     wychyleń E

		for(i=1;i<NX;i++)
		{
            glPointSize(2);
		    glColor4f(0,0,0,0);
            glBegin(GL_POINTS);
			point1[0]+=(2/(float)NX);
			point1[1]=U[i];
			glVertex3fv(point1);
            glEnd();

            if(Flag[i] & C_SRC)     // rysuj komórki źródłowe na czerwono
            {
                glPointSize(4);
                glColor4f(1,0,0,0);
                glBegin(GL_POINTS);
                point1[0]+=(2/(float)NX);
                point1[1]=U[i];
                glVertex3fv(point1);
                glEnd();
            }
	 	}*/
	//glutSwapBuffers();
	//glutPostRedisplay();
	#endif
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    windW = WWIDTH;
    windH = WHEIGHT;
    glutInitWindowPosition(150, 150);
    glutInitWindowSize( windW, windH);
//    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);//|GLUT_RGB);

    if (glutCreateWindow("Wave 1D") == GL_FALSE)
		exit(1);
	Init();
    glutKeyboardFunc(Key);
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(idle);
    glutMainLoop();
	return 0;
}
