


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>



#include "fluid.h"
#include "glucik.h"




// fluid surface tables


double U[NX+1]={0};
double SBAK[NX+1]={0};
double UBAK[NX+1]={0};
double S[NX+1]={0};
double B[NX+1]={0};

double D[NX+1]={0};



double DT=0.01;	
double G=10.0;//1.0;


#define epsilon 0.001



void CalculateSurface(void)
{
   //	double u;
	int i;



    
   	for(i=0;i<NX;i++)
	{
		D[i] = S[i] - B[i]; 

   		if(D[i] < 0)
			D[i]=0;
	}

// trzymaj
   	for(i=3;i<NX-3;i++)
		UBAK[i] = (U[i+1]+U[i-1])/2 + G*(DT/(2*DX))*(S[i+1]-S[i-1]);

// tu tez
   	for(i=3;i<NX-3;i++)
		SBAK[i] = (S[i+1]+S[i-1])/2 - D[i]*(DT/(2*DX))*(U[i+1]-U[i-1]);



// wpisz
   	for(i=3;i<NX-3;i++)
		U[i] = UBAK[i];

   	for(i=3;i<NX-3;i++)
		S[i] = SBAK[i];



	for(i=3;i<NX-3;i++)
	{
	  	if(S[i] < B[i])
	   		S[i] = B[i] - epsilon;
	}

  //	S[2]=S[3];
  //	S[NX-3] = S[NX-4];


}




void DrawFluidSurface(void)
{
	int i;


	glLineWidth(DX);

	glBegin(GL_LINES);
	for(i=0;i<NX;i++)
	{


// fluid
		glColor3f(0.4f, 0.25, 0.85);
		glVertex2f((double)(2*i)/(double)NX-1,B[i]);
		glVertex2f((double)(2*i)/(double)NX-1,S[i]);

// bottom
		glColor3f(0.4f, 0.25, 0.15);
		glVertex2f((double)(2*i)/(double)NX-1,0);
		glVertex2f((double)(2*i)/(double)NX-1,B[i]);


	}							  
	glEnd();




	glLineWidth(11);
	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-1,0);
	glVertex2f(-1,1);
	glVertex2f(1,1);
	glVertex2f(1,0);
	glVertex2f(-1,0);
	glEnd();



}






void InitSurface(void)
{
	int i;

// define bottom


 	for(i=0;i<NX;i++)
	{
//	   	B[i] = 0.5-2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;

		B[i] = 0.3+sin((double)i/22)/12;

	}							  


	for(i=0;i<NX;i++)
	{
		S[i] = B[i]+0.07;
	}	


// define surface

	for(i=NX-NX/3;i<NX;i++)
//	for(i=NX/2;i<NX/4;i++)
	{
   //		S[i] = 0.84-(cos((double)i/25)/10);
	//	if(S[i]<B[i])
   //			S[i]=B[i];
	}	

   
}

