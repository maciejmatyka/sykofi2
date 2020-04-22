/**
Rozwiązanie numeryczne fali jednowymiarowej.
Wersja do rysowania wykresów dla książki +
wersja do zabawy - wykresy + przejście + ewolucja czasowa obok.
Wrocław, 2020-04-21

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
int line = 0;
int numsrc = 10;

#define WWIDTH 300*3
#define WHEIGHT	1000
//int init=0,line=0,step=0,k=0;

// computational mesh
const int NX = 1000;
const float v2 = 0.025; // prędkość^2 fali
double U[NX+1];
double E[NX+1];             // dla lepszej czytelności kodu zmianę wychylenia w czasie oznaczamy literką
int Flag[NX+1];             // E zamiast literką V jak zapisano w książce
double srcF[NX+1];             // src frequency
double srcP[NX+1];             // src phase
double srcA[NX+1];             // src amplitude

// numerical constants
const float DT = 0.255;    // krok czasowy
const float DX = 0.5;      // rozmiar oczka siatki obliczeniowej
const float FREQ = 0.01;    // pulsating source frequency
const float AMPLITUDE = 0.1;
const int C_SRC = 2;        // pulsating source cell flag
const int C_BND = 1;        // bundary cell flag

// vis
float point1[3] = { 0.0, 1.0, 0.0 };
float point2[3] = { 0.0, 1.0, 0.0 };

float point1DIST[3] = { 0.0, 1.0, 0.0 };
float point2DIST[3] = { 0.0, 1.0, 0.0 };

// rys 3.7 - pojedynczy impuls
/*#define DY 0.21
#define S 1.5
#define STEP 1200
#define TSTART 0
// rys 3.8 - pojedynczy impuls przez ścianę
#define DY 0.21
#define S 1.5
#define STEP 700
#define STEPSTART 10400
*/
// rys 3.9 - b) interferencja MAP
/*#define DY 0.21
#define S 1.5
#define STEP 40
#define STEPSTART 0
//#define ANIMATION
#define MODEMAP
*/
// rys 3.9 - a) standard

// rys 3.11 - a) los
/*#define BYLODY (1.0/1000)
#define DY 0.01
#define S 2.5//(4*0.1*(BYLODY / DY))
#define STEP 16//*(DY/BYLODY)
#define STEPSTART 0
//#define ANIMATION
#define MODEMAP
#define SRAND 305
*/
// rys 3.11 - a) standard
#define BYLODY (1.0/1000)
#define DY 0.01
#define S 2.5//(4*0.1*(BYLODY / DY))
#define STEP 16//*(DY/BYLODY)
#define STEPSTART 0
//#define ANIMATION
#define MODEMAP
#define CONTINUE
#define SRAND 305


#define BYLODY (1.0/1000)
#define DY 0.05
#define SDIST  20.5*(BYLODY / DY)
#define STEPDIST 18*(DY/BYLODY)
//#define ANIMATION


void clearscr()
{
/*        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        glVertex2f(-1,-1);
        glVertex2f(-1,1);
        glVertex2f(1,1);
        glVertex2f(1,-1);
        glEnd();*/
        glClearColor(1.0,1.0,1.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT);       // gl hack
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
}

// A - Inicjalizuj tablice wychyleń oraz oznaczenia
// komórek (oznaczenia jak w książce)
void init_waves()
{
	for(int i=0; i<=NX; i++)
	{
		Flag[i] = 0;
		U[i] = E[i] = 0;
	}
    Flag[0] = Flag[NX] = C_BND;
	// place 'sources'
    /*Flag[NX/2] = C_SRC;
    Flag[NX/2-1] = C_SRC;*/
    Flag[0] |= C_SRC;
//    Flag[NX/2] |= C_SRC;
 //   Flag[NX/4] |= C_SRC;
 //   Flag[3*NX/4-4] |= C_SRC;

     // dołóż losowe
    int liczba =numsrc;//4;//rand() % 1000;
    for(int i=0; i<liczba; i++)
    {
//        Flag[i * NX/100] = C_SRC;
        int poz = rand() % NX;
        Flag[  poz  ] = C_SRC;
        srcF[ poz ]  = 0.02+(rand()/(float)(RAND_MAX)) * 0.01;
        srcA[ poz ]  = 0.014+(rand()/(float)(RAND_MAX)) * 0.001;
        srcP[ poz ]  = 0.0;//(rand()/(float)(RAND_MAX)) * 120.2*3.14;
    }
    t=0;
    line = 0;
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
            //if(t>1.0*PI/FREQ)      // czas trwania imulsu minął?
            if(0&&t>1.0*PI/FREQ)      // czas trwania imulsu minął?
                    Flag[i] &= (~C_SRC); // zdejmij flagę komórki źródłowej
            else
                //U[i] = sin( t*FREQ ) * AMPLITUDE;
                U[i] = sin( t*srcF[i] + srcP[i] ) * srcA[i];
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
  //  U[NX]=U[NX-1];// free
  //  E[NX]=E[NX-1];

	glutPostRedisplay();
}





static void Key(unsigned char key, int x, int y)
{
    switch (key) {
	case 27:
		exit(0);
		break;
	case 'k':
		t=0;
		break;
    case ' ':
        init_waves();
        clearscr();
        break;
    case '+':
        numsrc++;
        init_waves();
        clearscr();
        break;
    case '-':
        numsrc--;
        if(numsrc<0) numsrc=0;
        init_waves();
        clearscr();
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
int stepDIST = 0;
int init = 0;

void drawwave(float SCALE, int line)
{
    point1DIST[0]=-0.5*(2.0/3.0)+0.333*(2/(float)NX);
    point1DIST[1]=1-(float)line/(float)WHEIGHT;
    point2DIST[1]=1-(float)line/(float)WHEIGHT;
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<NX;i+=1)
    {
    //	point1[PY]=U[i];
    //	point2[PY]=U[i+1];
        glVertex2f(point1DIST[0],point1DIST[1]+SCALE*U[i]);
        glVertex2f((point1DIST[0]+0.333*(2/(float)NX)),point2DIST[1]+SCALE*U[i+1]);
        point1DIST[0]+=0.333*(2/(float)NX);
    }
    glEnd();
}


void drawwaveMAP(float SCALE, int &line)
{
//    static int przejscie = 0;
//    przejscie++;
    if(line > WHEIGHT*2.0)
    {
        clearscr();

        line -= WHEIGHT*2.0;
    }

        point1[0]=-1+(2/(float)NX);
        point1[1]=1-(float)line/(float)WHEIGHT;
        glBegin(GL_POINTS);
		for(int i=1;i<NX;i++)
		{
		    #define SC1 1
		    #define SC2 3
		    #define SC3 5
//			glColor3f(fabs(U[i]*2),fabs(U[i]*4),fabs(U[i]*8));
			//glColor3f(fabs(SC*(U[i]+0.09)*9),fabs(SC*(U[i]+0.01)*3),fabs(SC*(U[i]+0.0)*2));
			glColor3f(fabs(SC1*(U[i]+0.09)*9),fabs(SC2*(U[i]+0.01)*3),fabs(SC3*(U[i]+0.0)*2));
			//glVertex3fv(point1);
			glVertex3f(2.0*(2.0/3.0)+point1[0], point1[1], point1[2]);
			point1[0]+=0.333*(2/(float)NX);
		}
        glEnd();
}

static void Draw(void)
{
   // #ifdef ANIMATION
    int i;
	//glClearColor(1.0, 1.0, 1.0, 0.0);
  	//glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);

	// clear part 1
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex2f(-1,-1.0);
	glVertex2f(-0.5*(2.0/3.0),-1.0);
	glVertex2f(-0.5*(2.0/3.0),1.0);
	glVertex2f(-1,1.0);
	glEnd();
	glColor3f(0.0,0.0,0.0);
	glLineWidth(2);
	point1[0]=-1+0.333*(2/(float)NX);

	// E - rysuj falę na podstawie obliczonych
	//     wychyleń E

		for(i=0;i<=NX;i++)
		{
            glPointSize(2);
		    glColor4f(0,0,0,0);
            glBegin(GL_POINTS);
			point1[1]=U[i];
			glVertex3fv(point1);
            glEnd();

            if(Flag[i] & C_SRC)     // rysuj komórki źródłowe na czerwono
            {
                glPointSize(4);
                glColor4f(1,0,0,0);
                glBegin(GL_POINTS);
                //point1[0]+=0.5*(2/(float)NX);
                point1[1]=U[i];
                glVertex3fv(point1);
                glEnd();
            }
			point1[0]+=0.333*(2/(float)NX);
	 	}
	glutSwapBuffers();

   // #endif // ANIMATION

   // #ifndef ANIMATION               // rysuj statycznie






   //
   // LINE DIST
   //

	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glColor3f(0.0,0.0,0.0);
	glPointSize(4);
	glLineWidth(2);
    stepDIST++;
    static int steppersistDIST = 0;
    steppersistDIST++;

    if(stepDIST>STEPDIST && steppersistDIST > STEPSTART)
    {

        stepDIST=0;
        point1DIST[1]-=DY;
        point2DIST[1]-=DY;
        drawwave(SDIST, line);
        glutSwapBuffers();
        drawwave(SDIST, line);
        glutSwapBuffers();

    }
    else
        glutSwapBuffers();


   //
   // MAP
   //
    glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glColor3f(0.0,0.0,0.0);
	glPointSize(4);
	glLineWidth(2);

    step++;
    static int steppersist = 0;
    steppersist++;


    if(step>STEP && steppersist > STEPSTART)
    {
        line++;
        step=0;
        drawwaveMAP(S, line);
        glutSwapBuffers();
        drawwaveMAP(S, line);
        glutSwapBuffers();

    }
    else
        glutSwapBuffers();

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
//	#endif
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    srand(SRAND);
    windW = WWIDTH;
    windH = WHEIGHT;
    glutInitWindowPosition(150, 10);
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
