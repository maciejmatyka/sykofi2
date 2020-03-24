
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "lbm.h"
#include "particles.h"

#define PC 2
int npar = 2*4096;			// num of particles
float par[NPARMAX*PC]; 


int modeinit = 1;           // 0-random, periodic
                            // 1-init at y=LY/2, x=LX-1

void parrandom(int i)
{
        if(modeinit == 1)
        {       
        	par[i*PC]     = rand()/double(RAND_MAX); // x
    		par[i*PC+1]	  = rand()/double(RAND_MAX); // y
        }
        else
        {
            par[i*PC]     = 1.0 - (2.0/float(LX))*(rand()/double(RAND_MAX));        // x
            par[i*PC+1]	  = 0.5 - (0.15)*(rand()/double(RAND_MAX)-0.5);    // y
        }
}
void initpar(void)
{
	for ( int k=0; k<npar; k++)
	   	parrandom(k);
}
float BilinearInterpolation(float x,float y,int x1,int x2,int y1,int y2,float f11,float f21,float f22,float f12)
{
	// @Numerical Recipes
	float t = (x-x1) / (x2 - x1);
	float u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}

int xytoij(int &i, int &j, float x, float y)
{
	i = int(floor(x * LX));	// 0 do L-1
	j = int(floor(y * LY));  // 0 do L-1
}
void movepar(float dt,int c)
{
	int i,j,k;
	int ip,jp;
	float x,y;
    
    float AAaspect = LX / LY;         // np. 2/1
    
	for ( k=0; k<npar; k++)
	{
		x = par[k*PC];
		y = par[k*PC+1];
		
        xytoij(i,j,x,y);          // zamieñ x,y -> i,j 
		while(F[ i ][ j ] == 1)   // dopóki cz¹steczka na przeszkodzie
		{
			parrandom(k);
			x = par[k*PC];	y = par[k*PC+1];
            xytoij(i,j,x,y);
		}		
		
		i = int(x * (LX-1) + LX ) % (LX);	// x=0 => i = 0, x=1 => L-2
		j = int(y * (LY-1) + LY ) % (LY);
		ip = int(x * (LX-1) + LX + 1 ) % (LX);
		jp = int(y * (LY-1)+ LY + 1 ) % (LY);
		
		x = x + BilinearInterpolation(x*(LX-1),y*(LY-1), i,ip, j,jp, U[c][i][j], U[c][ip][j], U[c][ip][jp], U[c][i][jp]) * dt;
		y = y + AAaspect * BilinearInterpolation(x*(LX-1),y*(LY-1), i,ip, j,jp, V[c][i][j], V[c][ip][j], V[c][ip][jp], V[c][i][jp]) * dt;
		
		if(modeinit==0)
		{
    	    while(x<0)  x = 1+x;
    	    while(y<0)  y = 1+y;
    	    while(x>1)  x = x-1;
    	    while(y>1)  y = y-1;
        }
        else
        {
            while(x<0 || y<0 || x>1 || y>1 || F[ i ][ j ] == 1)   // dopóki cz¹steczka na przeszkodzie
            {    
                parrandom(k);
                x = par[k*PC];	y = par[k*PC+1];
                xytoij(i,j,x,y);
            }
        }
            
		par[k*PC]	= x;
		par[k*PC+1]	= y;
	}
}
void drawpar(void)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	// VBO RENDER
	glPointSize(2);
	glColor4f(1.0,1.0,1.0,0.7f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,par);
	glDrawArrays(GL_POINTS,0,NPARMAX);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawpar(int mode)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	// VBO RENDER
	glPointSize(2);
  
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
	glVertexPointer(2,GL_FLOAT,0,par);
	glDrawArrays(GL_POINTS,0,npar);
	glDisableClientState(GL_VERTEX_ARRAY);
}
