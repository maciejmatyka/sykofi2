


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>



#include "fluid.h"
#include "glucik.h"




// fluid surface tables


double VS[NX+1]={0};
double S[NX+1]={0};
double B[NX+1]={0};

double D[NX+1]={0};



double DT=0.5;	
double G=10.0;//1.0;


void CalculateSurface(void)
{
	double vs;
	int i;
    

 
   	for(i=3;i<NX-3;i++)
	{
		D[i] = S[i] - B[i]; 

   		if(D[i] < 0)
			D[i]=0;
	}



	for(i=3;i<NX-3;i++)
	{
		vs = (D[i-1] + D[i]) * (S[i] - S[i-1]);
		vs = vs - (D[i] + D[i+1]) * (S[i+1] - S[i]);
		vs = vs / (2.0f * DX2);
		vs = vs * (-G);

		VS[i] = VS[i] + vs*DT;
	}							  
   

	for(i=3;i<NX-3;i++)
	{
		S[i] = S[i] + VS[i] * DT;
	}




	S[2]=S[3];
	S[NX-3] = S[NX-4];


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
//		B[i] = 0.18+sin((double)i/20)/10;
	}							  



// define surface

	for(i=0;i<NX;i++)
	{
		S[i] = 0.35;
	}


	for(i=0;i<NX;i++)
	{
//		S[i] = 0.35+0.056-(cos((double)i/10))/18;


   //	   	S[i] = 0.2+2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;


	 	S[NX-i] = 0.35-0.056+(cos((double)i/20))/18;

	}	


	for(i=0;i<NX;i++)
		if(S[i]<B[i])
			S[i]=B[i];

   
}

