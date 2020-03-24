


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>



#include "fluid.h"
#include "glucik.h"




// fluid surface tables


double U[NX+1]={0};
double S[NX+1]={0};
double B[NX+1]={0};

double D[NX+1]={0};



double DT=0.5;	
double G=1.0;//1.0;





void CalculateSurface(void)
{
	double Vs;
	int i;
	double r;
    

   	for(i=0;i<NX;i++)
	{
		D[i] = S[i] - B[i]; 

   		if(D[i] < 0)
		{
			D[i] = 0;
   		}
	}



	for(i=3;i<NX-3;i++)
	{
		S[i] =  ((D[i-1]+D[i])/(2*DX))*U[i-1];
		S[i] -= ((D[i]+D[i+1])/(2*DX))*U[i];
		S[i] *= DT;
	}

	for(i=3;i<NX-3;i++)
		U[i] = (-G * (S[i+1]-S[i])/DX)*DT;



}




void DrawFluidSurface(void)
{
	int i;

	glLineWidth(DX);

	glBegin(GL_LINES);
	for(i=0;i<NX;i++)
	{


// bottom
		glColor3f(0.4f, 0.25, 0.15);
		glVertex2f((double)(2*i)/(double)NX-1,0);
		glVertex2f((double)(2*i)/(double)NX-1,B[i]);

// fluid
		glColor3f(0.4f, 0.25, 0.85);
		glVertex2f((double)(2*i)/(double)NX-1,B[i]);
		glVertex2f((double)(2*i)/(double)NX-1,S[i]);


	}							  
	glEnd();


}






void InitSurface(void)
{
	int i;

// define bottom


 //	B[0]=B[1]=B[2]=B[NX]=B[NX-1]=B[NX-2]=0.9;

	for(i=0;i<NX;i++)
	{
	   	B[i] = 0.54-2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;

	  //	B[i] = (0.25 + cos((double)i/130)/10);
	}							  


// define surface
	for(i=0;i<NX;i++)
	{
		S[i] = (0.53 + cos((double)i/50)/10);
		if(S[i]<B[i])
			S[i]=B[i];
	}	

	for(i=0;i<NX/2;i++)
	{
		S[i]=B[i];
	}

   
}

