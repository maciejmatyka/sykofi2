


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


double aspect,sx=0.1,sy=0.1,sz=0.1,anglex=0,angley=0,anglez=0;
double tr_x=0,tr_y=0;			
			 

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




//#define rysuj_wektory

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

  glRasterPos3f(x, y,0);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}


float move=0;

void Draw(void)
{
	
	Punkt *roboczyP;
	Sprezyna *roboczaS;
	Obszar *roboczyO;

   static GLfloat light_position[] = {2.05, 2.075, 2.15, 2.1};
 const GLfloat lightAmbient[]={.5f,.5f,.5f,0.5};
  const GLfloat lightDiffuse[]={.8f,.8f,.8f,.8};
   
 static GLfloat wall_mat[] = {0.3f, 0.3f, 0.3f, 0.4f};
    
	GLfloat wall_mat2[] = {.5f, .5f, .5f, .5f};






	aspect=(double)szer/(double)wys;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);
  


	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();  




	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.,aspect, 1., 100.);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10.,10.,10.,0.,0.,0.,.0,.1,.0);





	glPushMatrix();

//	glPushMatrix();



	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
    GLfloat local_view[] = { 0.0 };      
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);  






	glRotated(80., 1., 0., 0.);
	glScalef(sx,sy,sz);
    glRotated(-90.,1,0,0);
	glRotated(anglex,1,0,0);
	glRotated(angley,0,1,0);
	glRotated(anglez,0,0,1);

    glTranslatef(tr_x,tr_y,0);



//	glClear(GL_COLOR_BUFFER_BIT);


  GLfloat fogColour[] = { 0.2f, 0.3f, 0.2f, 0.1f };
 //	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColour);
	glFogf(GL_FOG_DENSITY, 0.079f);

//	glPushMatrix();





     glShadeModel (GL_FLAT); 


	


	glDisable(GL_LIGHTING); 

		glColor3f(0.9,0.0,0.0);


	glBegin(GL_QUADS);

		glColor3f(0.2,0.2,0.2);

    	glVertex3f(xmin,ymax,zmin);
		glVertex3f(xmax,ymax,zmin);
		glVertex3f(xmax,ymax,zmax);
		glVertex3f(xmin,ymax,zmax);

		glColor3f(0.3,0.3,0.3);

    	glVertex3f(xmin,ymin,zmin);
		glVertex3f(xmax,ymin,zmin);
		glVertex3f(xmax,ymin,zmax);
		glVertex3f(xmin,ymin,zmax);

		glColor3f(0.4,0.4,0.45);


		glVertex3f(xmin,ymin,zmin);
		glVertex3f(xmin,ymax,zmin);
		glVertex3f(xmin,ymax,zmax);
		glVertex3f(xmin,ymin,zmax);

		glColor3f(0.2,0.4,0.4);
		
		glVertex3f(xmax,ymin,zmin);
		glVertex3f(xmax,ymax,zmin);
		glVertex3f(xmax,ymax,zmax);
		glVertex3f(xmax,ymin,zmax);

	glEnd();







//  cartesian vectors

#define pointersize 3.4
#define pointerwingsize 0.2*pointersize

	glLineWidth(4);

	glBegin(GL_LINES);


		// x		

		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0,+0.7);
		glVertex3f(pointersize,0,+0.7);

		glVertex3f(pointersize,0,+0.7);
		glVertex3f(pointersize-pointerwingsize,0+pointerwingsize,+0.7);

		glVertex3f(pointersize,0,+0.7);
		glVertex3f(pointersize-pointerwingsize,0-pointerwingsize,+0.7);


		// y		



		glColor3f(0.0,1.0,0.0);
		glVertex3f(0,0,+0.7);
		glVertex3f(0,pointersize,+0.7);

		glVertex3f(0,pointersize,+0.7);
		glVertex3f(0-pointerwingsize,pointersize-pointerwingsize,+0.7);

		glVertex3f(0,pointersize,+0.7);
		glVertex3f(0+pointerwingsize,pointersize-pointerwingsize,+0.7);


		// z		



		glColor3f(0.0,0.0,1.0);
		glVertex3f(0,0,+0.7);
		glVertex3f(0,0,pointersize+0.7);

		glVertex3f(0,0,pointersize+0.7);
		glVertex3f(0-pointerwingsize,0+pointerwingsize,pointersize-pointerwingsize+0.7);

		glVertex3f(0,0,pointersize+0.7);
		glVertex3f(0+pointerwingsize,0-pointerwingsize,pointersize-pointerwingsize+0.7);





	glEnd();



  	
	glEnable(GL_LIGHTING); 

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat2);


	glLineWidth(3);

	for(roboczaS=KorzenSprezyn;roboczaS != NULL; roboczaS=roboczaS->prawy)
	{

#ifdef rysuj_sprezyny
		glBegin(GL_LINE_STRIP);
		glColor3f(1,0.8,0.7);
			glVertex3f(roboczaS->punkt1->r.x,roboczaS->punkt1->r.y,roboczaS->punkt1->r.z);
		glColor3f(0.6,0.7,0.5);
			glVertex3f(roboczaS->punkt2->r.x,roboczaS->punkt2->r.y,roboczaS->punkt2->r.z);
	   	glEnd();
#endif

		glColor3f(0.5,0.6,0.7);

	}

		glColor3f(1,1,1);


/*  swiat
		glBegin(GL_LINE_STRIP);
			glVertex3f(xmin-0.01,ymin-0.01,zmin-0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmin-0.01);

			glVertex3f(xmin-0.01,ymin-0.01,zmin-0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmin-0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmin-0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmin-0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmin-0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmin-0.01);

	   	glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(xmin-0.01,ymin-0.01,zmax+0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmax+0.01);

			glVertex3f(xmin-0.01,ymin-0.01,zmax+0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmax+0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmax+0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmax+0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmax+0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmax+0.01);

	   	glEnd();
*/





#ifdef rysuj_wektory

	glDisable(GL_LIGHTING);

	glLineWidth(2);

	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_NRYS))
	if(roboczyP->flaga & P_NORM)
	{

		glColor3f(1,0.6,0.5);

		RysujWektor(roboczyP->r,roboczyP->r+(roboczyP->v*dt),30.0);


		glColor3f(0.5,0.5,0.6);
	if(roboczyP->v.dlugosc() > 0.1)
		{
			Wektor v=roboczyP->v;
			v.normuj();
			v = v * roboczyP->v.dlugosc();

			glColor3f(1,0.6,0.5);


			GLWypisz(roboczyP->r.x+(v.x)*dt*40,roboczyP->r.y+(v.y)*dt*40,"v");
		}
	}

	glEnable(GL_LIGHTING);


#endif





	glColor3f(1.0,1.0,1.0);


//    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);

	glLineWidth(1);



	for(roboczyO=KorzenObszarow;roboczyO != NULL; roboczyO=roboczyO->prawy)
	if(roboczyO->flaga == O_AKT)
	{

    glTranslatef(roboczyO->r1.x,roboczyO->r1.y,roboczyO->r1.z);
    glPushMatrix();

/*		glBegin(GL_POINTS);
		glVertex3f(roboczyO->r1.x,roboczyO->r1.y,roboczyO->r1.z);
	   	glEnd();
*/

	//glColor3f(roboczyO->r,roboczyO->g,roboczyO->b);

	if(roboczyO->punkt)
		glutSolidSphere(roboczyO->R,24,16);
	else
	{
/*
	    glEnable(GL_DEPTH_TEST); 
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glEnable(GL_BLEND);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
*/
	 	glColor3f(1.0,1.0,1.0);
	 	glutWireSphere(roboczyO->R,16,16);

 //		glutSolidSphere(roboczyO->R,24,16);

   //	    glDisable(GL_BLEND);
   //		glDisable(GL_CULL_FACE);
   //	    glDisable(GL_DEPTH_TEST); 
		
	}


    glTranslatef(-roboczyO->r1.x,-roboczyO->r1.y,-roboczyO->r1.z);
    glPushMatrix();


//	glPopMatrix();      

	}



#ifdef rysuj_punkty	

	   glEnable(GL_DEPTH_TEST); 
	   glEnable(GL_BLEND);
	   glEnable(GL_CULL_FACE);


	glColor3f(0.7,0.6,0.5);
	glPointSize(2);

	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(roboczyP->flaga & P_NORM)
	if(!(roboczyP->flaga & P_NRYS))
	if(!roboczyP->R)
	{
	glBegin(GL_POINTS);						
		glVertex3f(roboczyP->r.x,roboczyP->r.y,roboczyP->r.z);
   	glEnd();
	}
	else
	{


		wall_mat2[0] = wall_mat2[1] = wall_mat2[2] =  roboczyP->col;
		wall_mat2[3] = 1.0f;	// alfab


	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat2);


		glColor3f(0.6,0.6,0.6);
	    glTranslatef(roboczyP->r.x,roboczyP->r.y,roboczyP->r.z);
		glPushMatrix();

		glutSolidSphere(roboczyP->R,16,16);

	    glTranslatef(-roboczyP->r.x,-roboczyP->r.y,-roboczyP->r.z);
		glPushMatrix();
	}

#endif





	glDisable(GL_LIGHTING); 


 



//		glColor3f(0,1.0,0);
	  

//		char dup[100];
//		sprintf(dup,"pik %f",sx);		
//		GLWypisz(-1, 1, dup);

	  /*
		RysujWektor(Wektor(-0.85,0.88,0),Wektor(-0.85,0.88,0)+Graw,0.15);
		glColor3f(1,1,1);
		GLWypisz(-0.8, 0.8, "g");


//		char dup[100];
//		sprintf(dup,"%f",sx);		
//		GLWypisz(-1, 1, dup);

 
		glColor3f(0.8,0.7,0.6);
		GLWypisz(1.1 - move , -0.85, "Newton by maq/floppy");

		move = move + 0.005;
	*/

//		glutStrokeCharacter(GLUT_BITMAP_HELVETICA_10,(int)"M");

    
	//glPopMatrix();

		
    glutSwapBuffers();
}




void Key(unsigned char key, int x, int y)
{
	Punkt *roboczyP;

    switch (key) {


	case ';':
		tr_y += 0.2;
		break;
	case '.':
		tr_y -= 0.2;
		break;
	case '/':
		tr_x += 0.2;
		break;
	case ',':
		tr_x -= 0.2;
		break;



	case '+':
		sx+=0.3;
		sy+=0.3;
		sz+=0.3;

		break;
	case '-':
		sx-=0.3;
		sy-=0.3;
		sz-=0.3;

		break;

	case '8':
		anglex+=3;
		break;
	case '2':	
		anglex-=3;
		break;
	case '6':
		angley+=3;
		break;
	case '4':
		angley-=3;
		break;
	case '7':
		anglez+=3;
		break;
	case '9':
		anglez-=3;
		break;
	case '5':
		anglex=angley=anglez=0;
		sx=sy=sz=0.008;
		break;



	case 'd':


	dodaj_punkt();

	break;


	case 'f':


	dodaj_punkt2();

	break;

	case 's':
		dodaj_punktS();
	break;



	case ' ':

		if(dt==0)
			dt = 0.01;
		else
			dt = 0;

		break;



	case 'g':

		if(Graw.dlugosc())
			Graw = Wektor(0,0,0);
		else
			Graw = Wektor(0,-10,0);
	break;

	case 'h':

		if(!Graw.x)
			Graw = Wektor(-1,-1,0);
		else
			Graw = Wektor(0,-1,0);
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
    glClearColor(0.5, 0.5, 0.6, 0.0);
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





void idle(void)
{
	Timestep();
    glutPostRedisplay();
}








    
	
