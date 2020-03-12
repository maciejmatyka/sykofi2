


#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "punkty.h"
#include "view.h"
#include "newton.h"
#include "cloth.h"

#ifndef PI
#define PI 3.1415
#endif

extern unsigned int tekstura;
extern Wektor Graw;
extern Wektor Wind;

int pause = 0;

double aspect,sx=0.62,sy=0.62,sz=0.62,anglex=0,angley=0,anglez=0;
double tr_x=-0.1,tr_y=0,tr_z=0;			
			 
void GLWypisz(float x, float y, char *string);
int WindowDump(int wwidth, int wheight);

int saveanim=0;
int frame=0;



//extern Cloth cloth1;
//extern Cloth cloth2;
extern Cloth cloth3;






void RysujWektor(Wektor w1,Wektor w2, double scale)
{
		double kx,ky,kprimx,kprimy,vsize,ksize;
		double x1,x2,y1,y2,z1,z2;

		x1=w1.x;
		y1=w1.y;
		z1=w1.z;


		x2=w2.x;
		y2=w2.y;
		z2=w2.z;



		if(x1==x2 && y1==y2)
			return;

		

// Wylicz d¶ugoùä wektora v i ustal d¶ugoùä wektora k

	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
	ksize=vsize*scale/4.f;


// Rysuj podstaw‡ strza¶ki


    glBegin(GL_LINE_STRIP);

//	glVertex2f(x1,y1);
//	glVertex2f(((x2-x1)/vecsize),((y2-y1)/vecsize));

 	glVertex3f(x1,y1,z1);
 	glVertex3f(x1+(x2-x1)*scale,y1+(y2-y1)*scale,z1+(z2-z1)*scale);

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

	glVertex3f(x1+(x2-x1)*scale,y1+(y2-y1)*scale,z1+(z2-z1)*scale);
	glVertex3f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy,z1+(z2-z1)*scale);

    glEnd();

// obr°ä k o -30 stopni

   	kprimx=kx*cos(-35*PI/180.f)+ky*sin(-35*PI/180.f);
   	kprimy=-kx*sin(-35*PI/180.f)+ky*cos(-35*PI/180.f);

// wymn°+ przez jego d¶ugoùä

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex3f(x1+(x2-x1)*scale,y1+(y2-y1)*scale,z1+(z2-z1)*scale);
	glVertex3f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy,z1+(z2-z1)*scale);

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





void CartesianVectors(void)
{

#define pointersize 0.4
#define pointerwingsize 0.2*pointersize

	glLineWidth(4);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

		// x		

		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0,0.0);
		glVertex3f(pointersize,0,0.0);

		glVertex3f(pointersize,0,0.0);
		glVertex3f(pointersize-pointerwingsize,0+pointerwingsize,0.0);

		glVertex3f(pointersize,0,0.0);
		glVertex3f(pointersize-pointerwingsize,0-pointerwingsize,0.0);


		// y		

		glColor3f(0.0,1.0,0.0);
		glVertex3f(0,0,0.0);
		glVertex3f(0,pointersize,0.0);

		glVertex3f(0,pointersize,0.0);
		glVertex3f(0-pointerwingsize,pointersize-pointerwingsize,0.0);

		glVertex3f(0,pointersize,0.0);
		glVertex3f(0+pointerwingsize,pointersize-pointerwingsize,0.0);

		// z		

		glColor3f(0.0,0.0,1.0);
		glVertex3f(0,0,0.0);
		glVertex3f(0,0,pointersize);

		glVertex3f(0,0,pointersize);
		glVertex3f(0-pointerwingsize,0+pointerwingsize,pointersize-pointerwingsize);

		glVertex3f(0,0,pointersize);
		glVertex3f(0+pointerwingsize,0-pointerwingsize,pointersize-pointerwingsize);

	glEnd();

	glEnable(GL_LIGHTING);

}




void GLWypisz(float x, float y, char *string)
{
  int len, i;

  glRasterPos3f(x, y,0);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}





//extern float STARTX;
extern float SPHERERAD;
float move=0;
void Draw(void)
{
//    cloth3.ROTATEX -= 0.2;
//    cloth3.updatePointsSTARTX();
    
	aspect=(double)szer/(double)wys;
	
		//	angley-=2.5*dt;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_BLEND); 
    glClearColor (1.0, 1.0, 1.0, 1.0);
//	glClearColor (0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();  

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.,aspect, .1, 100.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.,0.,1.,0.,0.,0.,.0,.1,.0);

	glPushMatrix();

	glScalef(sx,sy,sz);
	glRotated(anglex,1,0,0);
	glRotated(angley,0,1,0);
	glRotated(anglez,0,0,1);

    glTranslatef(tr_x,tr_y,tr_z);

//  	MyBox();

/*	glColor3f(1,1,0);
	glLineWidth(5);
	RysujWektor(Wektor(0.01,0.01,0.01),Wind,0.1);
*/
    /*glFrontFace(GL_CW); 
    glEnable(GL_AUTO_NORMAL); 
    glEnable(GL_NORMALIZE);   
    glEnable(GL_FOG);   
    GLfloat fogColor[4] = {0.6, 0.6, 0.6, 1.0}; 
    glFogi (GL_FOG_MODE, GL_EXP);
    glFogfv (GL_FOG_COLOR, fogColor); 
    glFogf (GL_FOG_DENSITY, 0.35); 
    glHint (GL_FOG_HINT, GL_DONT_CARE); */

// 2 - oswietlenie deLamberta
// 3 - teksturowanie

//	cloth1.DisplayGL(3);	
//	cloth2.DisplayGL(3);	
//	cloth3.DisplayGL(0);

  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
glDisable(GL_LINE_SMOOTH);
glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);	
  cloth3.DisplayGL(1);	
  
  glDisable(GL_FOG);

  glEnable (GL_BLEND); 
  glBlendFunc (GL_ONE, GL_ONE); 
   glDisable (GL_BLEND); 
  glColor4f(0.1,0.1,0.1,0.3); 
   //glColor4f(0.9,0.6,0.6,1.0); 
  glTranslatef(cloth3.STARTX,0,0);
  glRotatef(cloth3.STARTX,1,0,0);
  glutSolidSphere(SPHERERAD*1.09,32,32);  
	//cloth3.DisplayGL(1);	

/*
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glColor4f(0.5,0.5,0.5,0.5);
	glBindTexture(GL_TEXTURE_2D,tekstura);
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-1,-1,0);
	glTexCoord2f(0,1);
	glVertex3f(-1,1,0);
	glTexCoord2f(1,1);
	glVertex3f(1,1,0);
	glTexCoord2f(1,0);
	glVertex3f(1,-1,0);
	glEnd();
	
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glDisable(GL_TEXTURE_2D);
*/


	glColor3f(1,1,1);
	glLineWidth(10);
//	RysujWektor(Wektor(0.01,0.01,0.01),Wind,0.1);

/*
    glBegin(GL_LINE_STRIP);
 	glVertex3f(-0.4,-2,0);
 	glVertex3f(-0.4,0.3,0);

    glEnd();

	glEnable(GL_POINT_SMOOTH);
	glPointSize(19);
    glBegin(GL_POINTS);
	glVertex3f(-0.4,0.3,0.003);
    glEnd();
*/

  /* float q = 0.5;//0.5*(1+sin(t*0.1))*.90;
		glAccum(GL_MULT, q);
		glAccum(GL_ACCUM, 1-q);
		glAccum(GL_RETURN, 1.0);
*/
        
        
    glutSwapBuffers();

	if(saveanim)
	{
		frame+=1;
		if(!(frame%3))
		WindowDump(szer,wys);
	}


}




void Key(unsigned char key, int x, int y)
{
	int i,j;
	float r;
	
    switch (key) {

    case '[':
        cloth3.STARTX -= 0.02;
        cloth3.updatePointsSTARTX();
        break;

    case ']':
        cloth3.STARTX += 0.02;
        cloth3.updatePointsSTARTX();
        break;

    case 'r':
        cloth3.ROTATEX -= 0.1;
        cloth3.updatePointsSTARTX();
        break;

    case 't':
        cloth3.ROTATEX += 0.1;
        cloth3.updatePointsSTARTX();
        break;

    case 'e':
        cloth3.ROTATEX += 0.015;
        cloth3.STARTX += 0.02;
        cloth3.updatePointsSTARTX();
        
        break;



    case ' ':
        
        cloth3.releasePoints();
        break;


	case 'p':
		 pause = 1-pause;
		    break;
	case ';':
		tr_y += 0.02;
		break;
	case '.':
		tr_y -= 0.02;
		break;
	case '/':
		tr_x += 0.02;
		break;
	case ',':
		tr_x -= 0.02;
		break;

	case 'k':
		tr_z += 0.02;
		break;
	case 'l':
		tr_z -= 0.02;
		break;


	case '+':
		sx+=0.03;
		sy+=0.03;
		sz+=0.03;

		break;
	case '-':
		sx-=0.03;
		sy-=0.03;
		sz-=0.03;

		break;

	case '8':
		anglex+=1;
		break;
	case '2':	
		anglex-=1;
		break;
	case '6':
		angley+=1;
		break;
	case '4':
		angley-=1;
		break;
	case '7':
		anglez+=1;
		break;
	case '9':
		anglez-=1;
		break;
	case '5':
		anglex=angley=anglez=0;
		sx=sy=sz=0.008;
		break;



	case 'd':
		 r=rand()/float(RAND_MAX);
		 
	break;



	case 's':
		if(!saveanim)
			saveanim=1;
		else
			saveanim=0;
		break;
	case 'a':
		saveanim=0;
		break;

	case 'z':
        // ZAWIES OSTATNIE PUNKTY
    	for(j=0;j<cloth3.ny;j++)
//		for(i=0;i<nx;i++)
		{
            cloth3.Points[cloth3.nx-1][j]->flaga |= P_ZAW;
            cloth3.PointsCPY[cloth3.nx-1][j].flaga |= P_ZAW;
        }


		break;
	case 'x':
        // ZAWIES OSTATNIE PUNKTY
    	for(j=0;j<cloth3.ny;j++)
//		for(i=0;i<nx;i++)
		{
            cloth3.Points[cloth3.nx-1][j]->flaga &= (~P_ZAW);
            cloth3.PointsCPY[cloth3.nx-1][j].flaga &= (~P_ZAW);
        }


		break;
	case 'g':

//		if(Graw.dlugosc())
	//		Graw = Wektor(0,0,0);
	//	else
			Graw = Wektor(0,-0.015,0);
	break;
	
	
		case 'h':

			Graw = Wektor(0,0,0);
	break;
		case 'f':

			Graw = Wektor(0,0.015,0);
    	break;

	case 'w':
//Wektor Wind(0.0,0,8);

		if(!Wind.dlugosc())
			Wind = Wektor(0,0,3);
		else
			Wind = Wektor(0,0,0);
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
    glClearColor(0.2, 0.1, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

	glViewport(0, 0, szer, wys);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-xmin, xmax, -ymin, ymax);
    glMatrixMode(GL_MODELVIEW);

}



void Reshape(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluOrtho2D(-xmin, xmax, -ymin, ymax);
//    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}





#define FALSE 0
#define TRUE 1

int WindowDump(int wwidth, int wheight)
{
   int i,j;
   FILE *fptr;
   static int counter = 0; //* This supports animation sequences 
   char fname[32];
   unsigned char *image;

   //* Allocate our buffer for the image 
   if ((image = (unsigned char *) malloc(3*wwidth*wheight*sizeof(char))) == NULL ) {
      fprintf(stderr,"Failed to allocate memory for image\n");
      return(FALSE);
   }

   glPixelStorei(GL_PACK_ALIGNMENT,1);
   sprintf(fname,"frames/frame%04d.ppm",counter);

   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"Failed to open file for window dump\n");
      return(FALSE);
   }

   //* Copy the image into our buffer 
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(0,0,wwidth,wheight,GL_RGB,GL_UNSIGNED_BYTE,image);

   fprintf(fptr,"P6\n%d %d\n255\n",wwidth,wheight); /*for ppm */
   for (j=wheight-1;j>=0;j--) {
      for (i=0;i<wwidth;i++) {
         fputc(image[3*j*wwidth+3*i+1],fptr);
         fputc(image[3*j*wwidth+3*i+2],fptr);
         fputc(image[3*j*wwidth+3*i+3],fptr);
      }
   }

   fclose(fptr);

   //* Clean up /
   counter++;
   free(image);
   return(TRUE);
}



void idle(void)
{

 	 if(!pause)
	 	Timestep();


    glutPostRedisplay();
}



    
	








    
	
