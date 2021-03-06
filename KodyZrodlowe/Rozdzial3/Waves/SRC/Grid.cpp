

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



void Grid::solvewave(float t, float DT, float DX, float DY, int sources)
{

	int iw=0,jw=0;
	float MYVEL;

 // E[NX/2][NY/2]=E[NX/2+1][NY/2]=E[NX/2][NY/2+1]=E[NX/2-1][NY/2]=E[NX/2][NY/2-1]=sin(t)/6;
 


/* 			for(iw=-1;iw<=1;iw++)
		 //	for(iw=1;iw<NX;iw++)
			for(jw=-1;jw<=1;jw++)
//			{
				E[NX/2+iw][NY/2+jw]=sin(t)/10;
//				E[NX/2+iw][NY/4+jw]=sin(t)/20;
//  			E[NX/4+iw][NY/4+jw]=sin(t)/20;
//				E[NX/2+iw][NY/2+jw]=sin(t)/20;
//				E[iw][NY/4+jw]=sin(t)/20;
//			}
	}
*/



		for(j=1;j<NY;j++)
		for(i=1;i<NX;i++)
		{
			if(Flag[i][j] & C_VELHALF)
				MYVEL = VEL / 12;
			else
				MYVEL = VEL;

				U[i][j]=U[i][j] + MYVEL* 	 
				(
				(1-eps) * (E[i+1][j]+E[i-1][j]+E[i][j-1]+E[i][j+1]-4*E[i][j]) 
				+ eps   * (E[i+1][j+1]+E[i+1][j-1]+E[i-1][j-1]+E[i-1][j+1]-4*E[i][j])
				)*DT/(DX*DX);

		}


		for(j=1;j<NY;j++)
		for(i=1;i<NX;i++)
		{
	    			E[i][j] = E[i][j]+U[i][j] * DT;
		}


	//U[0]=0;
	//U[1]=0;
	//U[NX-1]=0;
	//U[NX]=0;



	if(sources)
	{
		for(j=1;j<NY;j++)
		for(i=1;i<NX;i++)
			if(Flag[i][j] & C_SRC)
				E[i][j]=(sin(t)/10);
	}


  for(i=0;i<=NX;i++)
  for(j=0;j<=NX;j++)
	if(Flag[i][j] & C_BND)
		E[i][j]=U[i][j]=0.0f;

  for(i=0;i<=NX;i++)
  {

  	U[i][NY]=U[i][NY-1]=U[i][NY-2]=0;
	E[i][NY]=E[i][NY-1]=E[i][NY-2]=0;

	U[i][0]=U[i][1]=U[i][2]=0;
  	E[i][0]=E[i][1]=E[i][2]=0;
   
 }

  for(j=0;j<=NY;j++)
  {
	U[0][j]=U[1][j]=U[2][j]=0;
	E[0][j]=E[1][j]=E[2][j]=0;

	U[NX][j]=U[NX-1][j]=U[NX-2][j]=0;
	E[NX][j]=E[NX-1][j]=U[NX-2][j]=0;
  }

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
			
			if(Flag[i][j] & C_BND)
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

// Rendering 2d line

	point1[PX]=-1;
	point1[PY]=-1;

	for(t=0.0f;t<=1.0f;t+=0.01f)
	{
	 // (1-t)*x1+t*x2
	 point1[PX]=(1-t)*x1+t*x2;
	 point1[PY]=(1-t)*y1+t*y2;

	
	i=(int)(((point1[PX]+1)/2) * NX)+1;
	j=(int)(((point1[PY]+1)/2) * NY)+1;

//	Flag[i][j] |= C_VELHALF;

	Flag[i][j] |= C_BNDTEMP;


	}

}


void Grid::clearBndTemp(void)
{
	int i,j;
	
	for(i=0;i<NX+1;i++)
	for(j=0;j<NY+1;j++)
		Flag[i][j] &= ~C_BNDTEMP;
}



/*
float itosrceen - give a screen coordinates (-1;1) for point determined by cell number(i)
with ((i-0.5)/NX-0.5)*2
*/


float Grid::itoscreen(int i)
{
	return (((i-0.5)/(float)NX)-0.5)*2;
}

/*
float countE - give me an E value for triangle with algorithm

*/

float Grid::countE(int i, int j)
{
	float w,Fi;

// near bnd?
	if(
	(Flag[i-1][j] & (C_BND|C_BNDTEMP)) || (Flag[i][j-1] & (C_BND|C_BNDTEMP)) ||
	(Flag[i][j] & (C_BND|C_BNDTEMP)) || (Flag[i-1][j-1] & (C_BND|C_BNDTEMP))
	  )
		return (0);
	else
	   //	return ( E[i-1][j] + E[i][j] + E[i-1][j-1] +  E[i][j-1] )/(float)4*2;


//	I[i][j]=(I[i][j]+w*VEL)/2;


	   return ( E[i-1][j] + E[i][j] + E[i-1][j-1] +  E[i][j-1] )/(float)4*2;

   //	   return ( U[i-1][j] + U[i][j] + U[i-1][j-1] +  U[i][j-1] )/(float)4*2;



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
	if(Flag[i][j] & C_SRC)
		glColor3f(1,1,0.1);
	else
		glColor3f(0.9+countE(i,j)*20,0.8+countE(i,j)*18,0.9+countE(i,j)*19);
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
	glFogf(GL_FOG_DENSITY, 0.039f);
}
else
	glDisable(GL_FOG);



  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.,aspect, 1., 100.);

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
	glScalef(6.+(float)scale,6.+(float)scale,6.+(float)scale);

	glTranslatef(0,0,py);					   


#define BH 0.085

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
		glVertex3f(itoscreen(NX),itoscreen(NY),BH);
		glVertex3f(itoscreen(1),itoscreen(NY),BH);

		glVertex3f(itoscreen(1),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(1),BH);
		glVertex3f(itoscreen(1),itoscreen(1),BH);

		glVertex3f(itoscreen(1),itoscreen(1),0);
		glVertex3f(itoscreen(1),itoscreen(NY),0);
		glVertex3f(itoscreen(1),itoscreen(NY),BH);
		glVertex3f(itoscreen(1),itoscreen(1),BH);

		glVertex3f(itoscreen(NX),itoscreen(1),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),0);
		glVertex3f(itoscreen(NX),itoscreen(NY),BH);
		glVertex3f(itoscreen(NX),itoscreen(1),BH);

	glEnd();
						 


	int flatcol=1;

for(j=1;j<=NY-1;j+=1)
{

for(i=1;i<=NX-1;i+=1)
{

	if(Flag[i][j] & C_SRC)
	{

	glColor3f(1,1,0.1);

	glBegin(GL_QUADS);

		glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
		glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
		glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

	glEnd();
	} // no src
	else
  if(!(Flag[i][j] & (C_BND|C_BNDTEMP)))
  {

	if(LINE3DONOFF)
	{

		glLineWidth(1);

		glColor3f(1,1,1);

		glBegin(GL_LINE_STRIP);

			glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
			glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
			glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
			glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

		glEnd();

	}

	if(FLATONOFF)
	{

	flatcol=!flatcol;

	glBegin(GL_QUADS);

		glColor3f(flatcol,flatcol,flatcol);
		glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
		glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
		glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

	glEnd();


/*	glBegin(GL_TRIANGLES);

		glColor3f(1,1,1);
		glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
		glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));


		glColor3f(0,0,1);

		glColor3f(0,0,0);
		glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		glColor3f(0,0,0);
		glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
		glColor3f(0,0,0);
		glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

	glEnd();
*/

	}

	if(!(FLATONOFF|LINE3DONOFF))
	{
	glBegin(GL_QUADS);

		setEColour(i,j);
		glVertex3f(itoscreen(i),itoscreen(j),countE(i,j));
		setEColour(i+1,j);
		glVertex3f(itoscreen(i+1),itoscreen(j),countE(i+1,j));
		setEColour(i+1,j+1);
		glVertex3f(itoscreen(i+1),itoscreen(j+1),countE(i+1,j+1));
		setEColour(i,j+1);
		glVertex3f(itoscreen(i),itoscreen(j+1),countE(i,j+1));

	glEnd();


	}

  }	/* F[i][j] & C_BND */
  else
  {
// wall 1

		glColor3f(0.3,0.3,0.1);


	glBegin(GL_QUADS);

		glVertex3f(itoscreen(i),itoscreen(j),0);
		glVertex3f(itoscreen(i+1),itoscreen(j),0);
		glVertex3f(itoscreen(i+1),itoscreen(j),BH);
		glVertex3f(itoscreen(i),itoscreen(j),BH);



// wall 2


		glVertex3f(itoscreen(i),itoscreen(j+1),0);
		glVertex3f(itoscreen(i+1),itoscreen(j+1),0);
		glVertex3f(itoscreen(i+1),itoscreen(j+1),BH);
		glVertex3f(itoscreen(i),itoscreen(j+1),BH);


	
// wall 3
	if(!(Flag[i-1][j] & C_BND))
	{


			glVertex3f(itoscreen(i),itoscreen(j),0);
			glVertex3f(itoscreen(i),itoscreen(j+1),0);
			glVertex3f(itoscreen(i),itoscreen(j+1),BH);
			glVertex3f(itoscreen(i),itoscreen(j),BH);

	}

// wall 4
	if(!(Flag[i+1][j] & C_BND))
	{

			glVertex3f(itoscreen(i+1),itoscreen(j),0);
			glVertex3f(itoscreen(i+1),itoscreen(j+1),0);
			glVertex3f(itoscreen(i+1),itoscreen(j+1),BH);
			glVertex3f(itoscreen(i+1),itoscreen(j),BH);

	}

// and over

	glColor3f(0.6,0.5,0.4);
		glVertex3f(itoscreen(i),itoscreen(j),BH);
		glVertex3f(itoscreen(i+1),itoscreen(j),BH);
		glVertex3f(itoscreen(i+1),itoscreen(j+1),BH);
		glVertex3f(itoscreen(i),itoscreen(j+1),BH);
	glEnd();


  } 


}




}


/*

for(j=1;j<=NY-1;j+=1)
for(i=1;i<=NX-1;i+=1)
	if(I[i][j]<E[i][j]*E[i][j])
		I[i][j] = E[i][j];

	
	float point1X=-1;
	float point2X=-1;
	float point1Y=0;
	float point2Y=0;

	j=NY-4;


	glLineWidth(5);
	glColor3f(1.0,1.0,1.0);

	glBegin(GL_LINE_STRIP);     

	for(i=0;i<NX;i+=1)
 	{
		point1Y=I[i][j]*10;
		point2Y=I[i+1][j]*10;

		point1X+=(2/(float)NX);


		glVertex3f(point1X,1.0,0.1+point1Y);     
		glVertex3f(point1X+2/(float)NX,1.0,0.1+point2Y);     
 	}
	glEnd();      
*/



	glPopMatrix();



  glPopMatrix();





//if (dobigdump) {
//      glMatrixMode(GL_PROJECTION);
//      glLoadIdentity();
//      CreateTheProjection();                /* - Application Specific - */
//      for (i=0;i<4;i++) {
//         for (j=0;j<4;j++) {
//            fprintf(stderr,"Frame (%d,%d)\n",i,j);
//            ClearTheBuffers();              /* - Application Specific - */
//            glViewport(-i*width,-j*height,4*width,4*height);
//            glMatrixMode(GL_MODELVIEW);
//            glDrawBuffer(GL_BACK);
//            glLoadIdentity();
//            gluLookAt(vp.x,vp.y,vp.z,focus.x,focus.y,focus.z,up.x,up.y,up.z);
//            MakeLighting();
//            MakeMaterial();
//            DrawTheModel();                 /* - Application Specific - */
//            WindowDump();
//            glutSwapBuffers();
//         }
//      }
//      dobigdump = FALSE;
//   }



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










