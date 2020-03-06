
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "lbm.h"
#include "particles.h"

#define PC 2            // liczba wymiarów
double par[NPARMAX*PC]; 
int npar;
int PSIZ=2;

void parrandom(int i)
{
    	par[i*PC]     = W*(rand()/double(RAND_MAX)); // x
		par[i*PC+1]	  = H*(rand()/double(RAND_MAX)); // y
}

void initpar(void)
{
	for (int k=0; k<npar; k++)
		parrandom(k);
}

int xytoij(int &i, int &j, float x, float y)
{
	i = int(floor(x * L0));	// 0 do L-1
	j = int(floor(y * L0));  // 0 do L-1
}

// Przesuñ cz¹steczkê
void movepar(double dt)
{    
	int i,j,k,ip,jp;
	double x,y,xp,yp;

	for ( k=0; k<npar; k++)
	{
		x = par[k*PC];	
        y = par[k*PC+1];
        xytoij(i,j,x,y);          // zamieñ x,y -> i,j 

		while(F[i][j] == 1)   // dopoki czasteczka na przeszkodzie
		{
			parrandom(k);
			x = par[k*PC];	
			y = par[k*PC+1];
            xytoij(i,j,x,y);
		}
     
        x = x + U[i][j] * dt;        
        y = y + V[i][j] * dt;

        // warunki periodyczne
		while( x<0 ) x = W+x;	 
		while( y<0 ) y = H+y;
		while( x>W ) x = x-W;	  
		while( y>H ) y = y-H;
		
		par[k*PC] = x;	
		par[k*PC+1]	= y;
	}
}


double BilinearInterpolation(double x,double y,float x1,float x2,float y1,float y2,double f11,double f21,double f22,double f12)
{
	// @Numerical Recipes
	double t = (x-x1) / (x2 - x1);
	double u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}

void moveparinterpolate(double dt)
{
	int i,j,k;
	int ip,jp;
	double x,y;
	for ( k=0; k<npar; k++)
	{
		x = par[k*PC];
		y = par[k*PC+1];
		xytoij(i,j,x,y);          // zamieñ x,y -> i,j 
		
		while(F[i][j] == 1)
		{
			parrandom(k);
			x = par[k*PC];	
			y = par[k*PC+1];
			xytoij(i,j,x,y);          // zamieñ x,y -> i,j 
		}

		double temp;
   		float fracx = x * L0 - i;
   		float fracy = y * L0 - j;

        if(fracx>=0.5) 	ip = i+1; //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
    		else 		ip = i-1;
        
		if(fracy>=0.5) 	
            jp = j+1; //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
		else 		
            jp = j-1;

        ip = int(ip + LX ) % (LX);		// i=0 => 0, i=-1 => L-1
        jp = int(jp + LY ) % (LY);		// i=L-1 => L-1, L => 0
        
   		if(i>ip  ) {int k=i; i=ip; ip=k; }
   		if(j>jp  ) {int k=j; j=jp; jp=k; }

        float x1,x2,y1,y2;

        x1 = i+0.5;       x2 = ip+0.5;
        y1 = j+0.5;       y2 = jp+0.5;

        if(y * L0 < 0.5)
        {
            y1 = 0.5;
            y2 = -0.5;
        }
        if(x * L0 < 0.5)
        {
            x1 = -0.5;
            x2 = 0.5;
        }
        
      	x = x + BilinearInterpolation(x*L0, y*L0, x1, x2, y1, y2, U[i][j],U[ip][j],U[ip][jp],U[i][jp]) * dt;
  	   	y = y + BilinearInterpolation(x*L0, y*L0, x1, x2, y1, y2, V[i][j],V[ip][j],V[ip][jp],V[i][jp]) * dt;

		while(x<0) 	x = W+x;
		while(y<0)	y = H+y;
		while(x>W)	x = x-W;
		while(y>H)	y = y-H;

		par[k*PC]	= x;
		par[k*PC+1]	= y;
	}
}

void drawpar(int mode)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	// VBO RENDER
	glPointSize(PSIZ);
  
    float a=0.7;
    if(mode==0)
	 		a=mnoznik_alpha*0.125;
   	//glEnable(GL_BLEND);
	glColor4f(1.0,1.0,1.0,a);
   
   /*	glBegin(GL_POINTS);
    for ( int k=0; k<npar; k++)
	 {
		glVertex2f(par[k*PC], par[k*PC+1]);
	 }
	glEnd();   */
		
   
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_DOUBLE,0,par);
	glDrawArrays(GL_POINTS,0,npar);
	glDisableClientState(GL_VERTEX_ARRAY);
}
