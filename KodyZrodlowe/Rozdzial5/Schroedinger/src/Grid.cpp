
#include <iostream>
#include <complex>

#include <windows.h>

#include <GL/GL.h>
#include <GL/glut.h>

#include "fx.h"
#include "fx3d.h"

#include "grid.h"

#include "fox.h"

#include <stdlib.h>



#define eps	(0.333)


extern float DT,DX,DY;
extern int GLVECTORS,BOXONOFF, FOGONOFF, FLATONOFF, LINE3DONOFF;

void Grid::clearsources(void)
{
		int i,j,k;

		for(j=1;j<NY;j++)
		for(i=1;i<NX;i++)
			if(Flag[i][j] & C_SRC)
			{
				Flag[i][j] &= ~C_SRC;
				
			}

}



void Grid::copy_psi(void)
{

}




void Grid::psi_iteration(int i, int j)
{

}


void Grid::clear_psi()
{
  for(i=1;i<NX-1;i++)
	for(j=1;j<NY-1;j++)
		psi[i][j] = complex<double>(0,0);
}




void Grid::place_gauss(int ki, int kj)
{
   for(j=0;j<NY-1;j++)
   {
    Amplituda=pow(j-SPaczki-kj,2)/(SPaczki*SPaczki);
    Amplituda=exp(-Amplituda);

    for(i=0;i<NX;i++)
	{
      zp=exp(-pow((i-SPaczki-ki),2)/(SPaczki*SPaczki));       
	  zp=zp*Amplituda;

      psi_t[i][j].real(zp*cos(i));
      psi_t[i][j].imag(zp*sin(i));
	}

   }

    for(i=0;i<NX;i++)
		psi_t[i][0]=psi_t[i][NY]=complex<double>(0,0);

    for(j=0;j<NY;j++)
		psi_t[0][j]=psi_t[NX][j]=complex<double>(0,0);

}



	double p;

/*
mozna przyspieszyc dajac w iteracji

  
	//	psi[i][j].real(psi_t[i][j].real() - DT * (psi[i+1][j].imag()+psi[i-1][j].imag()+psi[i][j-1].imag()+psi[i][j+1].imag()-4*psi[i][j].imag()));
	//	psi[i][j].imag(psi_t[i][j].imag() + DT * (psi[i+1][j].real()+psi[i-1][j].real()+psi[i][j-1].real()+psi[i][j+1].real()-4*psi[i][j].real()));


*/

void Grid::solveshroedinger(float t, float DT, float DX, float DY, int sources)
{
	int l,i,j,ll,czas;
	complex<double> c_i(0,1);		// po prostu i
	complex<double> lapl;

// potencjal

  for(i=1;i<NX-1;i++)
	for(j=1;j<NY-1;j++)
		if(Flag[i][j] & C_BND) 
			psi_t[i][j]=((double)2*exp_vdt)*psi_t[i][j];
		else
			psi_t[i][j]=(double)2*psi_t[i][j];

// procedura iteracyjna na Psi

for(l=1;l<IloscIteracji;l++)
{
	for(i=1;i<NX-1;i++)
		for(j=1;j<NY-1;j++)
		{
			psi[i][j].real(psi_t[i][j].real() - DT * (psi[i+1][j].imag()+psi[i-1][j].imag()+psi[i][j-1].imag()+psi[i][j+1].imag()-4*psi[i][j].imag()));
			psi[i][j].imag(psi_t[i][j].imag() + DT * (psi[i+1][j].real()+psi[i-1][j].real()+psi[i][j-1].real()+psi[i][j+1].real()-4*psi[i][j].real()));
		}

	for(i=NX-1;i>1;i--)
		for(j=NY-1;j>1;j--)
		{
			psi[i][j].real(psi_t[i][j].real() - DT * (psi[i+1][j].imag()+psi[i-1][j].imag()+psi[i][j-1].imag()+psi[i][j+1].imag()-4*psi[i][j].imag()));
			psi[i][j].imag(psi_t[i][j].imag() + DT * (psi[i+1][j].real()+psi[i-1][j].real()+psi[i][j-1].real()+psi[i][j+1].real()-4*psi[i][j].real()));
		}
}


// ostatni krok - wyliczmy psi_t dla nastepnego kroku

  for(i=1;i<NX-1;i++)
	for(j=1;j<NY-1;j++)
	    psi_t[i][j]=psi[i][j] - psi_t[i][j]/(double)2;


// warunki brzegowe

    for(i=0;i<NX;i++)
		psi_t[i][0]=psi_t[i][NY]=complex<double>(0,0);

    for(j=0;j<NY;j++)
		psi_t[0][j]=psi_t[NX][j]=complex<double>(0,0);

}



/* ok, z knigi

  void Grid::solveshroedinger(float t, float DT, float DX, float DY, int sources)
{
	int l,i,j,ll,czas;
	double laplasjan;
	complex<double> c_i(0,1);		// po prostu i
	complex<double> lapl;


  for(i=1;i<NX-1;i++)
	for(j=1;j<NY-1;j++)
		if(Flag[i][j] & C_BND) 
			psi_t[i][j]=((double)2*exp_vdt)*psi_t[i][j];
		else
			psi_t[i][j]=(double)2*psi_t[i][j];


for(l=1;l<IloscIteracji;l++)
{
	for(i=1;i<NX-1;i++)
		for(j=1;j<NY-1;j++)
		{
		psi[i][j].real(psi_t[i][j].real() - DT * (psi[i+1][j].imag()+psi[i-1][j].imag()+psi[i][j-1].imag()+psi[i][j+1].imag()-4*psi[i][j].imag()));
		psi[i][j].imag(psi_t[i][j].imag() + DT * (psi[i+1][j].real()+psi[i-1][j].real()+psi[i][j-1].real()+psi[i][j+1].real()-4*psi[i][j].real()));
	//		lapl = (psi[i+1][j]+psi[i-1][j]+psi[i][j-1]+psi[i][j+1]-(double)4*psi[i][j]);
	//		psi[i][j] = psi_t[i][j] +  (double)DT *c_i * lapl;
		}

	for(i=NX-1;i>1;i--)
		for(j=NY-1;j>1;j--)
		{
		psi[i][j].real(psi_t[i][j].real() - DT * (psi[i+1][j].imag()+psi[i-1][j].imag()+psi[i][j-1].imag()+psi[i][j+1].imag()-4*psi[i][j].imag()));
		psi[i][j].imag(psi_t[i][j].imag() + DT * (psi[i+1][j].real()+psi[i-1][j].real()+psi[i][j-1].real()+psi[i][j+1].real()-4*psi[i][j].real()));
//			lapl = (psi[i+1][j]+psi[i-1][j]+psi[i][j-1]+psi[i][j+1]-(double)4*psi[i][j]);
//			psi[i][j] = psi_t[i][j] +  (double)DT *c_i * lapl;
		}
}



  for(i=1;i<NX-1;i++)
	for(j=1;j<NY-1;j++)
	    psi_t[i][j]=psi[i][j] - psi_t[i][j]/(double)2;


// warunki brzegowe
    for(i=0;i<NX;i++)
		psi_t[i][0]=psi_t[i][NY]=complex<double>(0,0);

    for(j=0;j<NY;j++)
		psi_t[0][j]=psi_t[NX][j]=complex<double>(0,0);

}




  */

void Grid::solvewave(float t, float DT, float DX, float DY, int sources)
{


}


void Grid::reset(void)
{
	int i,j;
	
	for(i=0;i<NX+1;i++)
	for(j=0;j<NY+1;j++)
	{
		U[i][j]=0.0f;
		E[i][j]=0.0f;
	}

	for(i=0;i<NX+1;i++)
	for(j=0;j<NY+1;j++)
		Flag[i][j] &= ~C_BND;

}



void Grid::render2d(int width, int height, float scale)
{
	float size;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.,width/height, 1., 100.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(5.,0.,90.,0.,0.,0.,0.,1.,0.);


  glPushMatrix();
  glRotated(0, 1., 1., 1.);


	glPushMatrix();
	glRotated(0., 1.,1.,1.);
	glScalef(25.+(float)scale,25.+(float)scale,25.+(float)scale);



	glDisable(GL_FOG);


	size=2*(height/(float)((NX+NY)/2));

	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(size);     


// Rendering 2d color

	point1[PX]=0;
	point1[PY]=-1;

	glBegin(GL_POINTS);
	for(j=1;j<NY;j++)
	{
		for(i=1;i<NX;i++)
 		{
			point1[PX]+=size/(float)width;
			
			if(Flag[i][j] & (C_BND|C_BNDTEMP))
			glColor3f(0.8,0.6,0.4);
			else
			glColor3f((E[i][j]+0.01)/2*40,(E[i][j]+0.01)/2*30,(E[i][j]+0.01)/2*20);
			
			glVertex3fv(point1);
		}
		point1[PY]+=size/(float)height;
		point1[PX]=-1;
	}
	glEnd();

  glPopMatrix();
  glPopMatrix();

}







void Grid::addBndTemp(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height)
{
	float t;

	int i,j;



	point1[PX]=-1;
	point1[PY]=-1;

	for(t=0.0f;t<=1.0f;t+=0.01f)
	{
	 // (1-t)*x1+t*x2
	 point1[PX]=(1-t)*x1+t*x2;
	 point1[PY]=(1-t)*y1+t*y2;

	
	i=(int)(((point1[PX]+1)/2) * NX)+1;
	j=(int)(((point1[PY]+1)/2) * NY)+1;


		Flag[i][j] |= C_BNDTEMP;


	}

}


void Grid::clearBndTemp(void)
{
	int i,j;
	
	for(i=0;i<NX+1;i++)
	for(j=0;j<NY+1;j++)
	{
    	Flag[i][j] &= ~C_BNDTEMP;
	}
}



/*
float itosrceen - give a screen coordinates (-1;1) for point determined by cell number(i)
with ((i-0.5)/NX-0.5)*2
*/


float Grid::itoscreen(int i)
{
	return (((i-0.5)/(float)(NX/2))-1);
}

/*
float countE - give me an E value for triangle with algorithm

*/

void Grid::makemeE(void)
{
	for(i=0;i<NX+1;i++)
	for(j=0;j<NY+1;j++)
	if(
	(Flag[i-1][j] & (C_BND|C_BNDTEMP)) || (Flag[i][j-1] & (C_BND|C_BNDTEMP)) ||
	(Flag[i][j] & (C_BND|C_BNDTEMP)) || (Flag[i-1][j-1] & (C_BND|C_BNDTEMP))
	  )
		E[i][j] = 0;
	else
	{
      z1=psi_t[i][j].real(); 
	  z2=psi_t[i][j].imag();
      pr=z1*z1+z2*z2;
	  E[i][j] = pr/1.3f;
	}

}

float Grid::countE(int i, int j)
{
	float w,Fi;

      z1=psi_t[i][j].real(); 
	  z2=psi_t[i][j].imag();
      pr=z1*z1+z2*z2;


	  return pr/1.3f;


}





/*
   Write the current view to a file
   The multiple fputc()s can be replaced with
      fwrite(image,width*height*3,1,fptr);
   If the memory pixel order is the same as the destination file format.
*/

int width=640,height=400,stereo=1;


int WindowDump(void)
{
   int i,j;
   FILE *fptr;
   static int counter = 0; /* This supports animation sequences */
   char fname[32];
   unsigned char *image;

   /* Allocate our buffer for the image */
   if ((image = (unsigned char *) malloc(3*width*height*sizeof(char))) == NULL ) {
      fprintf(stderr,"Failed to allocate memory for image\n");
      return(FALSE);
   }

   glPixelStorei(GL_PACK_ALIGNMENT,1);

   /* Open the file */
   if (stereo)
      sprintf(fname,"L_%04d.ppm",counter);
   else
      sprintf(fname,"C_%04d.ppm",counter);

   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"Failed to open file for window dump\n");
      return(FALSE);
   }

   /* Copy the image into our buffer */
   glReadBuffer(GL_BACK_LEFT);
   glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the raw file */
    fprintf(fptr,"P6\n%d %d\n255\n",width,height); /*for ppm */
   for (j=height-1;j>=0;j--) {
      for (i=0;i<width;i++) {
         fputc(image[3*j*width+3*i+1],fptr);
         fputc(image[3*j*width+3*i+2],fptr);
         fputc(image[3*j*width+3*i+3],fptr);
      }
   }
   fclose(fptr);

   if (stereo) {
      /* Open the file */
      sprintf(fname,"R_%04d.ppm",counter);
      if ((fptr = fopen(fname,"w")) == NULL) {
         fprintf(stderr,"Failed to open file for window dump\n");
         return(FALSE);
      }

      /* Copy the image into our buffer */
      glReadBuffer(GL_BACK_RIGHT);
      glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

      /* Write the raw file */
       fprintf(fptr,"P6\n%d %d\n255\n",width,height); /*for ppm */
      for (j=height-1;j>=0;j--) {
         for (i=0;i<width;i++) {
            fputc(image[3*j*width+3*i+1],fptr);
            fputc(image[3*j*width+3*i+2],fptr);
            fputc(image[3*j*width+3*i+3],fptr);
         }
      }
      fclose(fptr);
   }

   /* Clean up */
   counter++;
   free(image);
   return(TRUE);
}





void Grid::setEColour(int i, int j)
{
//	if(Flag[i][j] & C_SRC)
//		glColor3f(1,1,0.1);
//	else

		glColor3f(0.1+E[i][j],0.3+E[i][j],0.5+E[i][j]);

}


void Grid::makebnda(void)
{
  for(j=0;j<NY;j++)
	for(i=0;i<NX;i++)
	{
		if(Flag[i][j] & C_BND)
		{
			V[i][j]=2; 
		}
	else  
			V[i][j]=1; 
   	}
}




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

  glRasterPos3f(x, y, -1);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) 
  {
    glutBitmapCharacter(font, string[i]);
  }
}



void Grid::render3d(GLdouble width,GLdouble height, GLdouble aspect, int angle, int anglex, int angley, int anglez, float scale, float py)
{
  int i,j;
  GLfloat fogColour[] = { 0.2f, 0.5f, 0.5f, 0.3f };




  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_DEPTH_TEST);
  

  glDisable(GL_DITHER);

if(FOGONOFF)
{
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColour);
	glFogf(GL_FOG_DENSITY, 0.059f);
}
else
	glDisable(GL_FOG);



// licz calke z Psi po calej przestrzeni (==1?)

  char string[100];
  int len;

  if(p>1)
	glColor3f(1,0.5,0.5);
  else
	glColor3f(1,1,1);


/*
  sprintf(string,"Calka z |Psi^2| = %f\n",p);

  glRasterPos3f(-1, -2, 5);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) 
  {
    glutBitmapCharacter(font, string[i]);
  }


 */


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.,aspect, 1., 100.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(5.,10.,15.,0.,0.,0.,0.,1.,0.);


  glPushMatrix();
  glRotated(angle, 0., 1., 0.);

  glRotated(anglex, 1., 0., 0.);
  glRotated(angley, 0., 1., 0.);
  glRotated(anglez, 0., 0., 1.);





	glPushMatrix();
	glRotated(80., 1., 0., 0.);
	glScalef(8.+(float)scale,8.+(float)scale,8.+(float)scale);

	glTranslatef(0,0,py);					   


#define BH 0.35

/* NOWE */
						   




// cartesian vectors


 if(GLVECTORS)
 {

// cartesian vectors

#define pointersize 0.4
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



 }

// box

 if(BOXONOFF)
 {

	glColor3f(0.444,0.433,0.422);
	glBegin(GL_QUADS);
    	glVertex3f(itoscreen(1)-0.04,itoscreen(1)-0.02,-BH*30);
		glVertex3f(itoscreen(NX)+0.04,itoscreen(1)-0.02,-BH*30);
 		glVertex3f(itoscreen(NX)+0.04,itoscreen(1)-0.02,BH*30);
		glVertex3f(itoscreen(1)-0.04,itoscreen(1)-0.02,BH*30);


		glVertex3f(itoscreen(1)-0.02,itoscreen(1)-0.04,-BH*30);
		glVertex3f(itoscreen(1)-0.02,itoscreen(NY)+0.04,-BH*30);
		glVertex3f(itoscreen(1)-0.02,itoscreen(NY)+0.04,BH*30);
		glVertex3f(itoscreen(1)-0.02,itoscreen(1)-0.04,BH*30);

	glColor3f(0.2,0.2,0.2);
		glVertex3f(itoscreen(1)-0.04,itoscreen(1)-0.04,-BH*30);
		glVertex3f(itoscreen(1)-0.04,itoscreen(NY)+0.04,-BH*30);
		glVertex3f(itoscreen(NX)+0.04,itoscreen(NY)+0.04,-BH*30);
		glVertex3f(itoscreen(NX)+0.04,itoscreen(1)-0.04,-BH*30);

	glEnd();

 }


// global boundaries

		glColor3f(0.4,0.4,0.3);

	glBegin(GL_QUADS);

    	glVertex3f(itoscreen(1),itoscreen(NY),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),0.08);
		glVertex3f(itoscreen(1),itoscreen(NY),0.08);

		glVertex3f(itoscreen(1),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(1),0.08);
		glVertex3f(itoscreen(1),itoscreen(1),0.08);

		glVertex3f(itoscreen(1),itoscreen(1),0);
		glVertex3f(itoscreen(1),itoscreen(NY),0);
		glVertex3f(itoscreen(1),itoscreen(NY),0.08);
		glVertex3f(itoscreen(1),itoscreen(1),0.08);

		glVertex3f(itoscreen(NX),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),0.08);
		glVertex3f(itoscreen(NX),itoscreen(1),0.08);

	glEnd();
						 


	int flatcol=1;
	glLineWidth(2);



for(j=1;j<=NY-1;j+=1)
{

for(i=1;i<=NX-1;i+=1)
{

	if(Flag[i][j] & C_SRC)
	{

	glColor3f(1,1,0.1);

	glBegin(GL_QUADS);

		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i][j]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i+1][j]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i+1][j+1]);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i][j+1]);

	glEnd();
	} // no src


  if(!( Flag[i][j] & (C_BNDTEMP|C_BND) ))
  {


	if(FLATONOFF)
	{

 	glColor3f(0,0,0);
	glBegin(GL_QUADS);

		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i][j]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i+1][j]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i+1][j+1]);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i][j+1]);

	glEnd();

	}


	if(LINE3DONOFF)
	{
		glLineWidth(1);

		glColor3f(1,1,1);

		glBegin(GL_LINE_STRIP);

			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i][j]);
			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i+1][j]);
			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i+1][j+1]);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i][j+1]);

		glEnd();
	}



	if(!(FLATONOFF|LINE3DONOFF))
	{

	glBegin(GL_QUADS);

		glColor3f((E[i][j]+0.01)/2*40,(E[i][j]+0.01)/2*30,(E[i][j]+0.01)/2*20);
//	    glColor3f(0.1+E[i][j],0.3+E[i][j]*1.5,0.5+E[i][j]);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i][j]);


		glColor3f((E[i+1][j]+0.01)/2*40,(E[i+1][j]+0.01)/2*30,(E[i+1][j]+0.01)/2*20);
//	  	glColor3f(0.1+E[i+1][j],0.3+E[i+1][j]*1.5,0.5+E[i+1][j]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i+1][j]);

		glColor3f((E[i+1][j+1]+0.01)/2*40,(E[i+1][j+1]+0.01)/2*30,(E[i+1][j+1]+0.01)/2*20);
//	    glColor3f(0.1+E[i+1][j+1],0.3+E[i+1][j+1]*1.5,0.5+E[i+1][j+1]);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i+1][j+1]);

		glColor3f((E[i][j+1]+0.01)/2*40,(E[i][j+1]+0.01)/2*30,(E[i][j+1]+0.01)/2*20);
//	    glColor3f(0.1+E[i][j+1],0.3+E[i][j+1]*1.5,0.5+E[i][j+1]);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,E[i][j+1]);

	glEnd();

	}

  }	/* F[i][j] & C_BND */


    if(Flag[i][j] & (C_BNDTEMP|C_BND) )
    {
// wall 1




	glColor3f(0.3,0.3,0.1);

	glBegin(GL_QUADS);

		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,0);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,0);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);

// wall 2


		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,0);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,0);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);


	
// wall 3
	if(!(Flag[i-1][j] & (C_BND|C_BNDTEMP)))
	{


			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,0);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,0);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);

	}

// wall 4
	if(!(Flag[i+1][j] & (C_BND|C_BNDTEMP)))
	{

			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,0);
			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,0);
			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);
			glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);

	}

// and over

	glColor3f(0.6,0.5,0.4);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,BH);
		glVertex3f(((i+1-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);
		glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+1-0.5)/(float)(NX/2))-1,BH);



	glEnd();

	} 


}




}



glLineWidth(1);


glColor3f(1,1,1);
for(j=1;j<=NY-1;j+=1)
{

for(i=1;i<=NX-1;i+=1)
{
  if(!(Flag[i][j] & (C_BND|C_BNDTEMP)))
  {

	if((FLATONOFF))
	{
		glBegin(GL_LINE_STRIP);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i][j]+0.04);
			glVertex3f(((i+2-0.5)/(float)(NX/2))-1,((j-0.5)/(float)(NX/2))-1,E[i+2][j]+0.04);
			glVertex3f(((i+2-0.5)/(float)(NX/2))-1,((j+2-0.5)/(float)(NX/2))-1,E[i+2][j+2]+0.04);
			glVertex3f(((i-0.5)/(float)(NX/2))-1,((j+2-0.5)/(float)(NX/2))-1,E[i][j+2]+0.04);
		glEnd();

	}
  }

}
}






	glPopMatrix();



  glPopMatrix();








}







void Grid::render2dlines(GLdouble width,GLdouble height, float scale)
{
	int i,j,b;

	glDisable(GL_FOG);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   



  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.,width/height, 1., 100.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(5.,0.,90.,0.,0.,0.,0.,1.,0.);


  glPushMatrix();
  glRotated(0, 1., 1., 1.);


	glPushMatrix();
	glRotated(0., 1.,1.,1.);
	glScalef(29.+(float)scale,25.+(float)scale,25.+(float)scale);

	glColor3f(1.0,1.0,1.0);
	glLineWidth(1);     

  for(j=0;j<NY;j++)
  {
	point1[PX]=-1;
	glBegin(GL_LINE_STRIP);
	b=j;
	for(i=0;i<NX;i+=1)
 	{
		glColor3f((E[i][NY-b]+0.02)*24,(E[i][NY-b]+0.02)*38.5,(E[i][NY-b]+0.02)*63);
		point1[PY]=1+E[i][NY-b]-((2.3*height/(float)NY)/(float)width)*j;
		point2[PY]=1+E[i+1][NY-b]-((2.3*height/(float)NY)/(float)width)*j;
		glVertex2f(point1[PX],point1[PY]);     
		glVertex2f(point1[PX]+2/(float)NX,point2[PY]);     
		point1[PX]+=(2/(float)NX);
	}
	glEnd();      
  }








  glPopMatrix();
  glPopMatrix();



}

void Grid::renderClear(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
}


void Grid::renderBnd(GLdouble width,GLdouble height)
{
	float size;
	
	size=2*(height/(float)((NX+NY)/2));

	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(size);     


// Rendering 2d color

	point1[PX]=-1;
	point1[PY]=-1;

	glBegin(GL_POINTS);
	for(j=1;j<=NY;j++)
	{
		for(i=1;i<=NX;i++)
 		{
			
			if(Flag[i][j] & C_BND)
				glColor3f(0.8,0.5,0.2);
			else
				glColor3f(0.3,0.2,0.1);

			glVertex3fv(point1);

			point1[PX]+=size/(float)width;
		}
		point1[PY]+=size/(float)height;
		point1[PX]=-1;
	}
	glEnd();

  glPopMatrix();
  glPopMatrix();
}


void Grid::renderSrc(GLdouble width,GLdouble height)
{
	float size;
	
	size=2*(height/(float)((NX+NY)/2));

	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(size);     


// Rendering 2d color

	point1[PX]=-1;
	point1[PY]=-1;

	glBegin(GL_POINTS);
	for(j=1;j<=NY;j++)
	{
		for(i=1;i<=NX;i++)
 		{
			if(Flag[i][j] & C_SRC)
			{
				glColor3f(1.0,1.0,0.0);
				glVertex3fv(point1);
			}
			point1[PX]+=size/(float)width;
		}
		point1[PY]+=size/(float)height;
		point1[PX]=-1;
	}
	glEnd();

  glPopMatrix();
  glPopMatrix();
}



void Grid::renderLine(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height)
{
	float size;
	float t;

	size=2*(height/(float)((NX+NY)/2));

	glDisable(GL_BLEND);     
	glDisable(GL_POINT_SMOOTH);     
	glPointSize(1);     



// Rendering 2d line

	point1[PX]=-1;
	point1[PY]=-1;

	glBegin(GL_POINTS);
	glColor3f(0.9f,0.6f,0.4f);
	for(t=0.0f;t<=1.0f;t+=0.01f)
	{
	 // (1-t)*x1+t*x2
	 point1[PX]=(1-t)*x1+t*x2;
	 point1[PY]=(1-t)*y1+t*y2;

     glVertex3fv(point1);
	}

	glEnd();

}


void Grid::addBnd(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height)
{
	float t;

	int i,j;

// Rendering 2d line

	point1[PX]=-1;
	point1[PY]=-1;

	for(t=0.0f;t<=1.0f;t+=0.01f)
	{
	 // (1-t)*x1+t*x2
	 point1[PX]=(1-t)*x1+t*x2;
	 point1[PY]=(1-t)*y1+t*y2;

	
	i=(int)(((point1[PX]+1)/(float)2) * NX)+1;
	j=(int)(((point1[PY]+1)/(float)2) * NY)+1;

//	Flag[i][j] |= C_VELHALF;

	Flag[i][j] |= C_BND;


	}

}




void Grid::changeSRC(float x, float y,GLdouble width,GLdouble height)
{

	int i,j;

// Rendering 2d line

	
	i=(int)(((x+1)/(float)2) * NX)+1;
	j=(int)(((y+1)/(float)2) * NY)+1;

	if(Flag[i][j] & C_SRC)
		Flag[i][j] &= ~C_SRC;
	else
		Flag[i][j] |=C_SRC;
}










