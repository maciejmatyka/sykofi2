
#include <iostream>
using namespace std;
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
int npar = NPARMAX;
int PSIZ=2;
int modeinit = 0;

void parrandom(int i)
{
    	//par[i*PC]     = W*(rand()/double(RAND_MAX)); // x
		//par[i*PC+1]	  = H*(rand()/double(RAND_MAX)); // y

	    if(modeinit == 1)
        {       
        	par[i*PC]     = W*(rand()/double(RAND_MAX)); // x
    		par[i*PC+1]	  = H*(rand()/double(RAND_MAX)); // y
        }
        else
        {
        	if(rand()/float(RAND_MAX) < 0.25)
        		par[i*PC+1]	  = H*(0.45 - (0.005)*(rand()/double(RAND_MAX)-0.5));			// y
        	else
        		par[i*PC+1]	  = H*(0.55 - (0.005)*(rand()/double(RAND_MAX)-0.5));			// y

			par[i*PC]     = W*(1.0 - (2.0/float(LX))*(rand()/double(RAND_MAX)));    // x

        }
}
void parrandomALL(int i)
{
       	par[i*PC]     = W*(rand()/double(RAND_MAX)); // x
   		par[i*PC+1]	  = H*(rand()/double(RAND_MAX)); // y
}

void addpar(int n)
{
	if(npar+n < NPARMAX)
	{
		for(int i = npar; i<npar+n; i++)
		{
			parrandom(i);
		}
		npar = npar + n;
	}

}


void initpar(void)
{
	for (int k=0; k<npar; k++)
		parrandom(k);
}
void initparALL(void)
{
	for (int k=0; k<npar; k++)
		parrandomALL(k);
}
float xytoij(int &i, int &j, float x, float y)
{
	i = int(floor(x * L0));		// 0 do L-1
	j = int(floor(y * L0));  	// 0 do L-1
}

// init with probability dependent on local
// velocity (to keep more particles where flow is faster)
float ALPHAPROB = 5000.1;
void initparprobability(int c)
{
	// velocity sum
	float usum=0;
	for(int i=0; i<LX; i++)
	for(int j=0; j<LY; j++)
		usum = usum + sqrt(U[c][i][j]*U[c][i][j]+V[c][i][j]*V[c][i][j]);

	
	float r,u;
	int i,j;
	for (int k=0; k<npar; k++)
	{
		cout << "particle: " << k << endl << "\x1b[1A";
		do
		{
			parrandomALL(k);
			float x = par[k*PC];
			float y = par[k*PC+1];

			xytoij(i, j, x, y);
			r = rand()/float(RAND_MAX);
			u = sqrt( U[c][i][j]*U[c][i][j]+V[c][i][j]*V[c][i][j] );
			//cout << "particle: " << k << " " << x << " " << y << " " << r << " " << ALPHA*(u/usum) << endl << ;
			// << "\x1b[0A";

		} while( u==0 || r > ALPHAPROB*(u/usum));
	}
}

void initparprobabilityINVERSE(int c)
{
	// velocity sum
	float usum=0;
	for(int i=0; i<LX; i++)
	for(int j=0; j<LY; j++)
	{
		float u = sqrt(U[c][i][j]*U[c][i][j]+V[c][i][j]*V[c][i][j]);
		if(u)
			usum = usum + 1.0/u;
	}

	float ALPHA = 1002.1;
	float r,u;
	int i,j;
	for (int k=0; k<npar; k++)
	{
		cout << "particle: " << k << endl << "\x1b[1A";
		do
		{
			parrandomALL(k);
			float x = par[k*PC];
			float y = par[k*PC+1];

			xytoij(i, j, x, y);
			r = rand()/float(RAND_MAX);
			u = sqrt( U[c][i][j]*U[c][i][j]+V[c][i][j]*V[c][i][j] );
			if(u)
				u = 1.0/u;
			//cout << "particle: " << k << " " << x << " " << y << " " << r << " " << ALPHA*(u/usum) << endl << ;
			// << "\x1b[0A";

		} while( u==0 || r > (ALPHA*(u/usum)));
	}
}

// Przesuñ cz¹steczkê
void movepar(double dt, int c)
{    
	int i,j,k,ip,jp;
	double x,y,xp,yp;

	for ( k=0; k<npar; k++)
	{
		x = par[k*PC];	
        y = par[k*PC+1];
        xytoij(i,j,x,y);          // zamieñ x,y -> i,j 

		//if(  x<0 || y<0 || x>=W || y>=H ) parrandom(k);

		while(F[i][j] == 1 || x<0 || y<0 || x>=W || y>=H )   // dopoki czasteczka na przeszkodzie
		{
			parrandom(k);
			x = par[k*PC];	
			y = par[k*PC+1];
            xytoij(i,j,x,y);
		}
     
        x = x + U[c][i][j] * dt;        
        y = y + V[c][i][j] * dt;

        // warunki periodyczne
		/*while( x<0 ) x = W+x;	 
		while( y<0 ) y = H+y;
		while( x>W ) x = x-W;	  
		while( y>H ) y = y-H;*/
		
		

		par[k*PC] = x;	
		par[k*PC+1]	= y;
		if(  x<0 || y<0 || x>=W || y>=H ) parrandom(k);
	}
}


double BilinearInterpolation(double x,double y,float x1,float x2,float y1,float y2,double f11,double f21,double f22,double f12)
{
	// @Numerical Recipes
	double t = (x-x1) / (x2 - x1);
	double u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}

void moveparinterpolate(double dt, int c)
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
        
      	x = x + BilinearInterpolation(x*L0, y*L0, x1, x2, y1, y2, U[c][i][j],U[c][ip][j],U[c][ip][jp],U[c][i][jp]) * dt;
  	   	y = y + BilinearInterpolation(x*L0, y*L0, x1, x2, y1, y2, V[c][i][j],V[c][ip][j],V[c][ip][jp],V[c][i][jp]) * dt;

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
		
//    glScalef(0.3,0.3,1.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_DOUBLE,0,par);
	glDrawArrays(GL_POINTS,0,npar);
	glDisableClientState(GL_VERTEX_ARRAY);
}
