/**
Rozwiązanie numeryczne fali jednowymiarowej.

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
float point1[3] = { 0.0, 0.0, 0.0 };
float t = 0;

#define WWIDTH 500
#define WHEIGHT	250
//int init=0,line=0,step=0,k=0;

// computational mesh

const int NX = 500;
const float v2 = 0.01; // prędkość^2 fali
double U[NX+1];
double E[NX+1];             // dla lepszej czytelności kodu zmianę wychylenia w czasie oznaczamy literką
int Flag[NX+1];             // E zamiast literką V jak zapisano w książce

// numerical constants

const float DT = 0.045;    // krok czasowy
const float DX = 0.5;      // rozmiar oczka siatki obliczeniowej
const float FREQ = 0.01;    // pulsating source frequency
const float AMPLITUDE = 0.1;
const int C_SRC = 2;        // pulsating source cell flag
const int C_BND = 1;        // bundary cell flag


// A - Inicjalizuj tablice wychyleń oraz oznaczenia
// komórek (oznaczenia jak w książce)
void init_waves()
{
	for(int i=0; i<NX; i++)
	{
		Flag[i] = 0;
		U[i] = E[i] = 0;
	}
    Flag[0] = Flag[NX-1] = C_BND;

	// place 'sources'
    Flag[NX/2] = C_SRC;
    Flag[NX/2-1] = C_SRC;
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
            if(t>2*PI/FREQ)      // czas trwania imulsu minął?
                Flag[i] &= (~C_SRC); // zdejmij flagę komórki źródłowej
            else
                U[i] = sin( t*FREQ ) * AMPLITUDE;
        }
    }

    // C - oblicz nową wartość wychylenia i jego pochodnej E
	for(i=0;i<=NX;i++)
    {
       if(Flag[i] == 0)            // tylko komórki puste - nie źródłowe i nie brzegowe
       {
          E[i] = E[i] + v2*DT*(U[i+1] - 2*U[i] + U[i-1])/(DX*DX);
          U[i] = U[i] + E[i] * DT;
       }
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
		t=0;
		break;
	default:
		break;
        }

    glutPostRedisplay();
}

static void Init(void)
{
    GLint i;
    glClearColor(0.0, 0.0, 0.0, 0.0);

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

static void Draw(void)
{
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
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    windW = WWIDTH;
    windH = WHEIGHT;
    glutInitWindowPosition(150, 150); glutInitWindowSize( windW, windH);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

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
