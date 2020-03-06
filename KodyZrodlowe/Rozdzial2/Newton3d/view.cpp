


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


double aspect,sx=0.2,sy=0.2,sz=0.2,anglex=0,angley=55,anglez=0;
double tr_x=1.7,tr_y=0,tr_z=0;			
			 
void GLWypisz(float x, float y, char *string);

int saveanim=0;


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

		

// Wylicz d¦ugoŠ wektora v i ustal d¦ugoŠ wektora k

	vsize=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
	ksize=vsize*scale/4.f;


// Rysuj podstawà strza¦ki


    glBegin(GL_LINE_STRIP);

//	glVertex2f(x1,y1);
//	glVertex2f(((x2-x1)/vecsize),((y2-y1)/vecsize));

 	glVertex3f(x1,y1,z1);
 	glVertex3f(x1+(x2-x1)*scale,y1+(y2-y1)*scale,z1+(z2-z1)*scale);

    glEnd();


// wyznacz wektor jednostkowy skierowany naprzeciwko v

	kx=-(x2-x1)/vsize;
	ky=-(y2-y1)/vsize;

// obr¡Š k o 30 stopni

   	kprimx=kx*cos(35*PI/180.f)+ky*sin(35*PI/180.f);
   	kprimy=-kx*sin(35*PI/180.f)+ky*cos(35*PI/180.f);

// wymn¡+ przez jego d¦ugoŠ

	kprimx*=ksize;
	kprimy*=ksize;

// rysuj kprim

    glBegin(GL_LINE_STRIP);

	glVertex3f(x1+(x2-x1)*scale,y1+(y2-y1)*scale,z1+(z2-z1)*scale);
	glVertex3f(x1+(x2-x1)*scale+kprimx,y1+(y2-y1)*scale+kprimy,z1+(z2-z1)*scale);

    glEnd();

// obr¡Š k o -30 stopni

   	kprimx=kx*cos(-35*PI/180.f)+ky*sin(-35*PI/180.f);
   	kprimy=-kx*sin(-35*PI/180.f)+ky*cos(-35*PI/180.f);

// wymn¡+ przez jego d¦ugoŠ

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




void MyBox(void)
{

//    glShadeModel(GL_SMOOTH); 

	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);


//  swiat
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


		glBegin(GL_LINE_STRIP);
			glVertex3f(xmin-0.01,ymax+0.01,zmin-0.01);
			glVertex3f(xmax+0.01,ymax+0.01,zmin-0.01);

			glVertex3f(xmin-0.01,ymax+0.01,zmin-0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmax+0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmax+0.01);
			glVertex3f(xmax+0.01,ymax+0.01,zmin-0.01);

			glVertex3f(xmax+0.01,ymax+0.01,zmax+0.01);
			glVertex3f(xmin-0.01,ymax+0.01,zmax+0.01);

	   	glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(xmin-0.01,ymin-0.01,zmin-0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmin-0.01);

			glVertex3f(xmin-0.01,ymin-0.01,zmin-0.01);
			glVertex3f(xmin-0.01,ymin-0.01,zmax+0.01);

			glVertex3f(xmax+0.01,ymin-0.01,zmax+0.01);
			glVertex3f(xmax+0.01,ymin-0.01,zmin-0.01);

			glVertex3f(xmax+0.01,ymin-0.01,zmax+0.01);
			glVertex3f(xmin-0.01,ymin-0.01,zmax+0.01);

	   	glEnd();






/*
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



  
		glVertex3f(xmin/4,0.4,zmin/4);
		glVertex3f(xmin/4,0.4,zmax/4);
		glVertex3f(xmax/4,0.4,zmax/4);
		glVertex3f(xmax/4,0.4,zmin/4);
  



	glEnd();
*/


	glEnable(GL_LIGHTING);


}



void DrawSprings(void)
{


   glShadeModel(GL_SMOOTH); 

float mat[3];      

mat[0] = 0.01745; 
mat[1] = 0.01175; 
mat[2] = 0.01175;    
     glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);     





#define lsx ( t * (roboczaS->punkt1->r.x) + (1-t) * (roboczaS->punkt2->r.x) )
#define lsy ( t * (roboczaS->punkt1->r.y) + (1-t) * (roboczaS->punkt2->r.y) )
#define lsz ( t * (roboczaS->punkt1->r.z) + (1-t) * (roboczaS->punkt2->r.z) )

	Sprezyna *roboczaS;

	glEnable(GL_LIGHTING);
	glLineWidth(4);



	for(roboczaS=KorzenSprezyn;roboczaS != NULL; roboczaS=roboczaS->prawy)
	if(roboczaS->flag & S_OK) 
 	if(!(roboczaS->flag & S_NRYS)) 
	{
	  if((roboczaS->flag & S_RYSL))
	  {
		for(float t=0; t<=1; t+=0.04)///roboczaS->dlugosc)
		{
			glTranslatef(lsx,lsy,lsz);
 			glPushMatrix();
			glutSolidCube(0.01);
			glTranslatef(-lsx,-lsy,-lsz);
			glPushMatrix();
		}
	  }
	  else
	  {


	glDisable(GL_LIGHTING);

	glColor3f(1,1,1);

		glBegin(GL_LINE_STRIP);
			glVertex3f(roboczaS->punkt1->r.x,roboczaS->punkt1->r.y,roboczaS->punkt1->r.z);
			glVertex3f(roboczaS->punkt2->r.x,roboczaS->punkt2->r.y,roboczaS->punkt2->r.z);
	   	glEnd();

	glEnable(GL_LIGHTING);




	  }



	}

}


void DrawVectors(void)
{
	Punkt *roboczyP;

#ifdef rysuj_wektory

	glDisable(GL_LIGHTING);

	glLineWidth(2);

	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_NRYS))
//	if(roboczyP->flaga & P_WEKT)
	{

		glColor3f(1.0,1.0,1.0);

		RysujWektor(roboczyP->r,roboczyP->r+(roboczyP->v*dt),80.0);


		glColor3f(1.0,0.5,1.0);
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


}



void DrawAreas(void)
{
	Obszar *roboczyO;

    glShadeModel(GL_SMOOTH); 

float mat[3];      


mat[0] = 0.01745; 
mat[1] = 0.01175; 
mat[2] = 0.01175;    
     glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);     


	for(roboczyO=KorzenObszarow;roboczyO != NULL; roboczyO=roboczyO->prawy)
	if(roboczyO->flaga & O_AKT)
	{


//	mat[0] = roboczyO->r/100;    
//	mat[1] = roboczyO->g/100;    
//	mat[2] = roboczyO->b;    

    glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
glMaterialf (GL_FRONT, GL_SHININESS, 0.2*128.0);     


    glTranslatef(roboczyO->r1.x,roboczyO->r1.y,roboczyO->r1.z);
    glPushMatrix();

	if(roboczyO->punkt)
	{

			if(roboczyO->flaga & O_RYS) 
			{
				glColor3f(0,0,0);
//			 	glutWireSphere(0.03,8,8);
			glutSolidCube(0.07);
//				glutSolidSphere(roboczyO->R,16,16);
			}
	}
	else
	{
	 	glColor3f(roboczyO->r,roboczyO->g,roboczyO->b);
//	 	glutWireSphere(roboczyO->R,24,24);
		glutSolidSphere(roboczyO->R,24,24);
	}


    glTranslatef(-roboczyO->r1.x,-roboczyO->r1.y,-roboczyO->r1.z);
    glPushMatrix();


//	glPopMatrix();      

	}

    mat[0] = 0.1745; 
mat[1] = 0.01175; 
mat[2] = 0.01175;    
     glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);     


}


// lampka

void DrawModel(void)
{
	Punkt p1,p2,p3,p4;		// pod³o¿e
	float mat[3];      

	glShadeModel(GL_SMOOTH); 
	glDisable(GL_LIGHTING);

	mat[0] = 0.01745;
	mat[1] = 0.01175; 
	mat[2] = 0.01175;    
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
	glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
	glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);     


	p1=*PrzesunNaKorzeniu(KorzenPunktow,2);
	p2=*PrzesunNaKorzeniu(KorzenPunktow,3);
	p3=*PrzesunNaKorzeniu(KorzenPunktow,4);
	p4=*PrzesunNaKorzeniu(KorzenPunktow,5);


// rysujê kwadrat podstawy

	glBegin(GL_QUADS);
		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p1.r.x,p1.r.y,p1.r.z);
		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p2.r.x,p2.r.y,p2.r.z);
		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p3.r.x,p3.r.y,p3.r.z);
		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p4.r.x,p4.r.y,p4.r.z);
	glEnd();
	
// rysujê przesuniêty lekko o wektor normalny do tej podstawy

	Wektor norm1;
	Wektor norm2;
	norm1=p1.r-p2.r;
	norm2=p1.r-p3.r;
	Wektor norm=norm1 % norm2;
	norm.normuj();

	norm = norm * 0.04;

	glBegin(GL_QUADS);
		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p1.r.x+norm.x,p1.r.y+norm.y,p1.r.z+norm.z);
		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p2.r.x+norm.x,p2.r.y+norm.y,p2.r.z+norm.z);
   		glVertex3f(p3.r.x+norm.x,p3.r.y+norm.y,p3.r.z+norm.z);
		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p4.r.x+norm.x,p4.r.y+norm.y,p4.r.z+norm.z);


		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p1.r.x,p1.r.y,p1.r.z);
   		glVertex3f(p1.r.x+norm.x,p1.r.y+norm.y,p1.r.z+norm.z);
		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p2.r.x+norm.x,p2.r.y+norm.y,p2.r.z+norm.z);
   		glVertex3f(p2.r.x,p2.r.y,p2.r.z);
	


		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p3.r.x,p3.r.y,p3.r.z);
   		glVertex3f(p3.r.x+norm.x,p3.r.y+norm.y,p3.r.z+norm.z);
		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p4.r.x+norm.x,p4.r.y+norm.y,p4.r.z+norm.z);
   		glVertex3f(p4.r.x,p4.r.y,p4.r.z);


		glColor3f(0.6,0.6,0.6);
   		glVertex3f(p1.r.x,p1.r.y,p1.r.z);
   		glVertex3f(p1.r.x+norm.x,p1.r.y+norm.y,p1.r.z+norm.z);
		glColor3f(0.3,0.3,0.3);
   		glVertex3f(p4.r.x+norm.x,p4.r.y+norm.y,p4.r.z+norm.z);
   		glVertex3f(p4.r.x,p4.r.y,p4.r.z);

		glEnd();

}


void DrawPoints(void)
{
	Punkt *roboczyP;


float mat[3];      

    mat[0] = 0.1745; 
mat[1] = 0.01175; 
mat[2] = 0.01175;    
     glMaterialfv (GL_FRONT, GL_AMBIENT, mat);     
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);     
glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);     



//#ifdef rysuj_punkty	


	glEnable(GL_LIGHTING); 

	   glEnable(GL_DEPTH_TEST); 
	   glEnable(GL_BLEND);


	glColor3f(4.0,7.0,9.0);
	glPointSize(2);

	for(roboczyP=KorzenPunktow;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(roboczyP->flaga & P_NORM)
	if(!(roboczyP->flaga & P_NRYS))
	{
	glBegin(GL_POINTS);						
		glVertex3f(roboczyP->r.x,roboczyP->r.y,roboczyP->r.z);
  	glEnd();

/*		glTranslatef(roboczyP->r.x,roboczyP->r.y,roboczyP->r.z);
		glPushMatrix();
 		glutWireSphere(0.03,16,16);

    glTranslatef(-roboczyP->r.x,-roboczyP->r.y,-roboczyP->r.z);
    glPushMatrix();
*/
//		glPopMatrix();


	}




	glEnable(GL_LIGHTING); 

// glowa
/* 	glTranslatef(P(12)->r.x,P(12)->r.y,P(12)->r.z);
 	glPushMatrix();
 		glutSolidSphere(0.1,16,16);
 	glTranslatef(-P(12)->r.x,-P(12)->r.y,-P(12)->r.z);
    glPushMatrix();
*/
 



//#endif





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


float move=0;

void Draw(void)
{
	
	Punkt *roboczyP;
	Obszar *roboczyO;

    static GLfloat light_position[] = {2.5, 2.75, 2.25, .1};
	const GLfloat lightAmbient[]={1.1f,1.1f,1.1f,0.5};
   const GLfloat lightDiffuse[]={1.8f,1.8f,1.8f,1.8};
   


	aspect=(double)szer/(double)wys;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);
  


	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();  




	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.,aspect, .1, 100.);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.,1.,1.,0.,0.,0.,.0,0.1,.0);





	glPushMatrix();

	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
 //   GLfloat local_view[] = { 0.0 };      
//	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);  




    glFrontFace(GL_CW); 
    glEnable(GL_AUTO_NORMAL); 
    glEnable(GL_NORMALIZE);   
    glEnable(GL_FOG);   
    GLfloat fogColor[4] = {0.6, 0.6, 0.6, 1.0}; 
    glFogi (GL_FOG_MODE, GL_EXP);
    glFogfv (GL_FOG_COLOR, fogColor); 
    glFogf (GL_FOG_DENSITY, 0.045); 
    glHint (GL_FOG_HINT, GL_DONT_CARE); 

    glClearColor(0.31, 0.32, 0.33, 0.0);     




	glRotated(80., 1., 0., 0.);
	glScalef(sx,sy,sz);
    glRotated(-90.,1,0,0);
	glRotated(anglex,1,0,0);
	glRotated(angley,0,1,0);
	glRotated(anglez,0,0,1);

    glTranslatef(tr_x,tr_y,tr_z);



	glutSetCursor(GLUT_CURSOR_NONE);





  	MyBox();
 //  	CartesianVectors();
	DrawSprings();
	DrawVectors();
//	DrawPoints();
	DrawAreas();
	DrawModel();

		
    glutSwapBuffers();
}




void Key(unsigned char key, int x, int y)
{
	Punkt *roboczyP;

    switch (key) {


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

	case 'r':

	anim();

	break;


	case 'd':


	dodaj_punkt();

	break;


	case 'f':


	dodaj_punkt2();

	break;

//	case 's':
//		dodaj_punktS();
//	break;



	case ' ':


	for(roboczyP=KorzenPunktow;roboczyP->prawy != NULL; roboczyP=roboczyP->prawy)
	{
		if(roboczyP->flaga & P_ZAW)
			roboczyP->flaga &= ~P_ZAW;
	}




	  /*
		if(dt==0)
			dt = 0.01;
		else
			dt = 0;
	  */
		break;
	  

	case 's':
		saveanim=1;
		break;

	case 'a':
		saveanim=0;
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
    glClearColor(0.2, 0.1, 0.0, 0.0);
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







int frame=0;

void idle(void)
{

	Timestep();
    glutPostRedisplay();

	if(saveanim)
	{
		frame+=1;
	}

}



    
	








    
	
