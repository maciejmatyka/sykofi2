


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

double H[NX+1]={0};



double DT=1.0;	
double G=5;//1.0;


double volume,volumeold,volumeadd;


void CalculateSurface(void)
{
	double as;
	int i;
	int n;

    

 
   	for(i=1;i<NX-1;i++)
	{
   	   	if(S[i] < B[i])
 	 		S[i] = B[i];

		H[i] = S[i] - B[i];

    }



	for(i=1;i<NX-1;i++)
	{
		as  = H[i]   * (S[i-1]-2*S[i]+S[i+1]);
		as += H[i-1] * (S[i-1]-S[i]);
		as += H[i+1] * (S[i+1]-S[i]);
		as /= 2*DX2;
		as *= G;

		VS[i] = VS[i] + as*DT;

	}							  
   



	for(i=1;i<NX-1;i++)
	{
 		S[i] = S[i] + VS[i] * DT;
						
	}





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
	   	B[i] = 0.1+2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;
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


   	  // 	S[i] = 0.2+2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;

	 	S[NX-i] = 0.35-0.056+(cos((double)i/20))/18;

	}	


	for(i=0;i<NX;i++)
		if(S[i]<B[i])
			S[i]=B[i];

 
   
}





/* ok second

 void InitSurface(void)
{
	int i;

// define bottom


 	for(i=0;i<NX/10;i++)
	{
//	   	B[i] = 0.5-2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;
//		B[i] = 0.18+sin((double)i/20)/10;

		B[i] = 0.4;
		B[NX-i] = 0.4;

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


	 	S[NX-i] = 0.55-0.056+(cos((double)i/20))/18;

	}	


	for(i=0;i<NX;i++)
		if(S[i]<B[i])
			S[i]=B[i];

   
}

*/




/* ok, first
void InitSurface(void)
{
	int i;

// define bottom


 	for(i=0;i<NX;i++)
	{
//	   	B[i] = 0.1+2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;
		B[i] = 0.1+(sin((double)i/14))/3;
	}							  



// define surface

	for(i=0;i<NX;i++)
	{
		S[i] = 0.35+0.056-(cos((double)i/10))/18;


//   	   	S[i] = 0.2+2*(((double)i/(double)NX)-0.5)*(((double)i/(double)NX)-0.5);      //sin((double)i/10)/6;

   //	 	S[NX-i] = 0.20+0.056+(cos((double)i/14))/2;

	}	


	for(i=0;i<NX;i++)
		if(S[i]<B[i])
			S[i]=B[i];

   
}

*/