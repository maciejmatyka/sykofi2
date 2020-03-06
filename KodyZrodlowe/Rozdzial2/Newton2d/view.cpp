


#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "punkty.h"
#include "view.h"
#include "newton.h"

#ifndef PI
#define PI 3.1415
#endif



double t;
extern float bak_dt;




void RysujWektor(Wektor w1,Wektor w2, double scale)
{
		double kx,ky,kprimx,kprimy,vsize,ksize;
		double x1,x2,y1,y2;

		x1=w1.x;
		y1=w1.y;

		x2=w2.x;
		y2=w2.y;


		if(x1==x2 && y1==y2)
			return;

		

// Wylicz d¶ugoùä wektora v i ustal d¶ugoùä wektora k

	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	ksize=vsize*scale/4.f;


// Rysuj podstaw‡ strza¶ki


    glBegin(GL_LINE_STRIP);

//	glVertex2f(x1,y1);
//	glVertex2f(((x2-x1)/vecsize),((y2-y1)/vecsize));

 	glVertex2f(x1,y1);
 	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);

    glEnd();


// wyznacz wektor jednostkowy skierowany naprzeciwko v

	kx=-(x2-x1)/vsize;
	ky=-(y2-y1)/vsize;

// obr°ä k o 30 stopni

   	kprimx=kx*cos(35*PI/180.f)+ky*sin(35*PI/180.f);
   	kprimy=-kx*sin(35*PI/180.f)+ky*cos(35*PI/180.f);

// wymn°+ przez jego d¶ugoùä

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);
	glVertex2f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy);

    glEnd();

// obr°ä k o -30 stopni

   	kprimx=kx*cos(-35*PI/180.f)+ky*sin(-35*PI/180.f);
   	kprimy=-kx*sin(-35*PI/180.f)+ky*cos(-35*PI/180.f);

// wymn°+ przez jego d¶ugoùä

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex2f(x1+(x2-x1)*scale,y1+(y2-y1)*scale);
	glVertex2f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy);

    glEnd();

}




#define rysuj_wektory

#define rysuj_punkty
#define rysuj_sprezyny



void *font = GLUT_BITMAP_TIMES_ROMAN_24;
void *fonts[] =
{
  GLUT_BITMAP_9_BY_15,
  GLUT_BITMAP_TIMES_ROMAN_10,
  GLUT_BITMAP_TIMES_ROMAN_24
};

void GLWypisz(float x, float y, char *string)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}


float move=0;

int bylo = 0;

int rozklad=0;


void Draw(void)
{
	
	Punkt *roboczyP;
	Sprezyna *roboczaS;
	Obszar *roboczyO;


	
   	glClear(GL_COLOR_BUFFER_BIT);

	

	glPushMatrix();

 	

	glLineWidth(1);



	glLineWidth(1);


	
/* 		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.5,0,0);
			glVertex3f(0.5,0,0);
	   	glEnd();
*/

	for(roboczaS=KorzenSprezyn;roboczaS != NULL; roboczaS=roboczaS->prawy)
	if(roboczaS->flag & S_OK) 
	{


#ifdef rysuj_sprezyny
		glColor3f(1,1,1);

		glBegin(GL_LINE_STRIP);
			glVertex3f(roboczaS->punkt1->r.x,roboczaS->punkt1->r.y,roboczaS->punkt1->r.z);
			glVertex3f(roboczaS->punkt2->r.x,roboczaS->punkt2->r.y,roboczaS->punkt2->r.z);
	   	glEnd();
#endif

		glColor3f(0.5,0.6,0.7);

	}

		glColor3f(1,1,1);


// rysuj pokoj


		glBegin(GL_LINE_STRIP);
			glVertex3f(xmin-0.01,ymin-0.01,0);
			glVertex3f(xmax+0.01,ymin-0.01,0);

			glVertex3f(xmin-0.01,ymin-0.01,0);
			glVertex3f(xmin-0.01,ymax+0.01,0);

			glVertex3f(xmax+0.01,ymax+0.01,0);
			glVertex3f(xmax+0.01,ymin-0.01,0);

			glVertex3f(xmax+0.01,ymax+0.01,0);
			glVertex3f(xmin-0.01,ymax+0.01,0);


	   	glEnd();


/*
		glBegin(GL_LINE_STRIP);

// kolizje G. by sie wkurzyl za takie cos



			glVertex3f(-1,0,0);
			glVertex3f(0,-1,0);


			glVertex3f(1,0,0);
			glVertex3f(0,-1,0);


	   	glEnd();

*/




	glColor3f(0.3,0.4,0.4);

	for(roboczyO=KorzenObszarow;roboczyO != NULL; roboczyO=roboczyO->prawy)
	if(roboczyO->flaga == O_AKT)
	{

     glPushMatrix ();
     glTranslatef(roboczyO->r1.x,roboczyO->r1.y,roboczyO->r1.z);


/*		glBegin(GL_POINTS);
		glVertex3f(roboczyO->r1.x,roboczyO->r1.y,roboczyO->r1.z);
	   	glEnd();
*/


	if(roboczyO->punkt)
		if(roboczyO->punkt->v.dlugosc() > 0.1)
		{
			glColor3f(0.35,0.45,0.45);
			glutWireSphere(roboczyO->R,24,16);
		}
		else
		{
			glColor3f(0.3,0.4,0.4);
			glutWireSphere(roboczyO->R,24,16);
		}
	else
	{
	 	glColor3f(0.35,0.45,0.45);
	 	glutWireSphere(roboczyO->R,24,16);
	}

 	glPopMatrix ();      

	}


 

	glColor3f(1.0,0.9,0.7);






	glPointSize(2);

#ifdef rysuj_punkty	
	glBegin(GL_POINTS);
   	glEnd();
	glEnable(GL_POINT_SMOOTH);

	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(roboczyP->flaga & P_NORM)
	if(!(roboczyP->flaga & P_NRYS))
	{
		glVertex3f(roboczyP->r.x,roboczyP->r.y,roboczyP->r.z);
	}
#endif

		glColor3f(1,0.6,0.5);


#ifdef rysuj_wektory

	glLineWidth(2);
   
	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_NRYS))
	if(roboczyP->flaga & P_NORM && roboczyP->flaga & P_WEKT)
	{

		glColor3f(1,0.6,0.5);

		RysujWektor(roboczyP->r,roboczyP->r+(roboczyP->v*dt),70.0);

		glColor3f(0.5,0.5,0.6);
	if(roboczyP->v.dlugosc() > 0.1)
		{													   
			Wektor v=roboczyP->v;
			v.normuj();
			v = v * roboczyP->v.dlugosc();

	   //		GLWypisz(roboczyP->r.x+(v.x)*dt*40,roboczyP->r.y+(v.y)*dt*40,"v");
		}
	}

#endif


		glColor3f(0.1,0.4,0.2);
	  

		RysujWektor(Wektor(-0.85,0.88,0),Wektor(-0.85,0.88,0)+Graw,0.15);
//		glColor3f(1,1,1);
		GLWypisz(-0.8, 0.8, "g");


	
		glColor3f(0.8,0.7,0.6);
  //		GLWypisz(1.1 - move , -0.85, "Newton by maq/floppy");

		move = move + 0.005;


//		glutStrokeCharacter(GLUT_BITMAP_HELVETICA_10,(int)"M");



  /*
	glPopMatrix();
   glutSwapBuffers();
	return;
   */





	float kx,ky,d,f;
	Wektor r21,k,fw;

if(rozklad)
{
	glPointSize(9);
	glBegin(GL_POINTS);
	for(kx=-1;kx<1;kx+=0.025)
		for(ky=-1;ky<1;ky+=0.025)
		{
			f=0.0f;
			fw=Wektor(0,0,0);
			k=Wektor(kx*0.25,ky*0.25,0);

			for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
			if(roboczyP->flaga & P_NORM)
			{
  				r21 = roboczyP->r - k;
				d = (r21).dlugosc();
				r21.normuj();
				if(d!=0)
				fw=fw - r21 * 0.1 *(roboczyP->masa/(d*d));
			}


			f = fw.dlugosc();
			
			glColor3f(f*0.0006,f*0.00065,f*0.0007);
			glVertex3f(kx,ky,0);
						 
			//printf("%f\n",f);

		}


	glEnd();	
}
    
	glPopMatrix();

		
    glutSwapBuffers();
}



void Key(unsigned char key, int x, int y)
{
	Punkt *roboczyP;

    switch (key) {


	case 'd':


	dodaj_punkt();

	break;


	case 'f':


	dodaj_punkt2();

	break;

	case 's':
		dodaj_punktS();
	break;


	case 'r':
		if(rozklad==0)
			rozklad=1;
		else
			rozklad=0;

		break;


	case ' ':


		if(dt==0)
		dt=bak_dt;
		else
		dt=0;

		break;


		for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
		{
			if(roboczyP->flaga & P_ZAW)
			{
				roboczyP->flaga |= P_NORM;
				roboczyP->flaga &= (~P_ZAW);
			}
  //			else
  //				roboczyP->flaga |= P_AKT;

		}

		break;



	case 'g':

		if(Graw.dlugosc())
			Graw = Wektor(0,0,0);
		else
			Graw = Wektor(0,-1,0);
	break;

	case 'h':

		if(!Graw.x)
			Graw = Wektor(-1,-1,0);
		else
			Graw = Wektor(0,-1,0);
	break;




	case '2':
		Graw=Graw + Wektor(0,-0.05,0);
		break;

	case '8':
		Graw=Graw + Wektor(0,0.05,0);
		break;

	case '4':
		Graw=Graw + Wektor(-0.05,0,0);
		break;

	case '6':
		Graw=Graw + Wektor(0.05,0,0);
		break;

	case '5':
		Graw=Wektor(0,-1,0);
		break;

 




	case 27:
		exit(0);	
	
	break;
	
      default:
	return;
        }

    glutPostRedisplay();
}



void Init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    glViewport(0, 0, szer, wys);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-xmin, xmax, -ymin, ymax);
    glMatrixMode(GL_MODELVIEW);

}



void Reshape(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-xmin, xmax, -ymin, ymax);
//    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}






void idle(void)
{
	t+=dt;

	if(dt!=0)
	Timestep();


    glutPostRedisplay();

}








    
	
