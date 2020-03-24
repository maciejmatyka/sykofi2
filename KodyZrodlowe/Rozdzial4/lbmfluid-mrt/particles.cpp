
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "lbm.h"
#include "particles.h"

#define PC 2
double par[NPARMAX*PC]; 
int npar;

void initpar(void)
{
	for ( int k=0; k<npar; k++)
	{
		par[k*PC]	= rand()/double(RAND_MAX);
		par[k*PC+1]	= rand()/double(RAND_MAX);
	}
}
float xytoij(int &i, int &j, float x, float y)
{
	i = int(floor(x * L));	// 0 do L-1
	j = int(floor(y * L));  // 0 do L-1
}
void parrandom(int i)
{
    	par[i*PC]     = (rand()/double(RAND_MAX)); // x
		par[i*PC+1]	  = (rand()/double(RAND_MAX)); // y
}
// init with probability dependent on local
// velocity (to keep more particles where flow is faster)
float ALPHAPROB = 5000.1;
void initparprobability(void)
{
	// velocity sum
	float usum=0;
	for(int i=0; i<L; i++)
	for(int j=0; j<L; j++)
		usum = usum + sqrt(U[i+j*L]*U[i+j*L]+V[i+j*L]*V[i+j*L]);

	
	float r,u;
	int i,j;
	for (int k=0; k<npar; k++)
	{
		do
		{
			parrandom(k);
			float x = par[k*PC];
			float y = par[k*PC+1];

			xytoij(i, j, x, y);
			r = rand()/float(RAND_MAX);
			u = sqrt( U[i+j*L]*U[i+j*L]+V[i+j*L]*V[i+j*L] );
			//cout << "particle: " << k << " " << x << " " << y << " " << r << " " << ALPHA*(u/usum) << endl << ;
			// << "\x1b[0A";

		} while( u==0 || r > ALPHAPROB*(u/usum));
	}
}

void initparprobabilityINVERSE(void)
{
	// velocity sum
	float usum=0;
	for(int i=0; i<L; i++)
	for(int j=0; j<L; j++)
	{
		float u = sqrt(U[i+j*L]*U[i+j*L]+V[i+j*L]*V[i+j*L]);
		if(u)
			usum = usum + 1.0/u;
	}

	float ALPHA = 1002.1;
	float r,u;
	int i,j;
	for (int k=0; k<npar; k++)
	{
		do
		{
			parrandom(k);
			float x = par[k*PC];
			float y = par[k*PC+1];

			xytoij(i, j, x, y);
			r = rand()/float(RAND_MAX);
			u = sqrt( U[i+j*L]*U[i+j*L]+V[i+j*L]*V[i+j*L] );
			if(u)
				u = 1.0/u;
			//cout << "particle: " << k << " " << x << " " << y << " " << r << " " << ALPHA*(u/usum) << endl << ;
			// << "\x1b[0A";

		} while( u==0 || r > (ALPHA*(u/usum)));
	}
}

double BilinearInterpolation(double x,double y,int x1,int x2,int y1,int y2,double f11,double f21,double f22,double f12)
{
	// @Numerical Recipes
	double t = (x-x1) / (x2 - x1);
	double u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}
void movepar(double dt)
{
	int i,j,k;
	int ip,jp;
	double x,y;
	for ( k=0; k<npar; k++)
	{
		x = par[k*PC];
		y = par[k*PC+1];
		i = int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
		j = int(y * (L-1) + L ) % (L);
		ip = int(x * (L-1) + L + 1 ) % (L);
		jp = int(y * (L-1)+ L + 1 ) % (L);
		x = x + BilinearInterpolation(x*(L-1),y*(L-1), i,ip, j,jp, U[ i+j*L ],U[ip+j*L],U[ip+jp*L],U[i+jp*L]) * dt;
		y = y + BilinearInterpolation(x*(L-1),y*(L-1), i,ip, j,jp, V[ i+j*L ],V[ip+j*L],V[ip+jp*L],V[i+jp*L]) * dt;
		if(x<0) x = 1+x;
		if(y<0)	y = 1+y;
		if(x>1)	x = x-1;
		if(y>1)	y = y-1;
		par[k*PC]	= x;
		par[k*PC+1]	= y;
	}
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

		i = int(floor(x * (L))); //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
		j = int(floor(y * (L))); //int(y * (L-1) + L ) % (L);
		
		while(FLAG[i+j*L] == 1)
		{
			x = par[k*PC]	= rand()/double(RAND_MAX);
			y = par[k*PC+1]	= rand()/double(RAND_MAX);
			i = int(floor(x * (L))); //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
			j = int(floor(y * (L))); //int(y * (L-1) + L ) % (L);
		}

		double temp;
   		float fracx = x * L - i;
   		float fracy = y * L - j;

        if(fracx>=0.5) 	ip = i+1; //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
    		else 		ip = i-1;
        
		if(fracy>=0.5) 	
            jp = j+1; //int(x * (L-1) + L ) % (L);	// x=0 => i = 0, x=1 => L-2
		else 		
            jp = j-1;

        ip = int(ip + L ) % (L);		// i=0 => 0, i=-1 => L-1
        jp = int(jp + L ) % (L);		// i=L-1 => L-1, L => 0
        
   		if(i>ip  ) {int k=i; i=ip; ip=k; }
   		if(j>jp  ) {int k=j; j=jp; jp=k; }

        float x1,x2,y1,y2;

        x1 = i+0.5;       x2 = ip+0.5;
        y1 = j+0.5;       y2 = jp+0.5;

        if(y * L < 0.5)
        {
            y1 = 0.5;
            y2 = -0.5;
        }
        if(x * L < 0.5)
        {
            x1 = -0.5;
            x2 = 0.5;
        }
        
      	x = x + BilinearInterpolation(x*L, y*L, x1, x2, y1, y2, U[ i+j*L ],U[ip+j*L],U[ip+jp*L],U[i+jp*L]) * dt;
  	   	y = y + BilinearInterpolation(x*L, y*L, x1, x2, y1, y2, V[ i+j*L ],V[ip+j*L],V[ip+jp*L],V[i+jp*L]) * dt;
  // 		x = x + BilinearInterpolation(x*(L-1),y*(L-1), i,ip, j,jp, U[ i+j*L ],U[ip+j*L],U[ip+jp*L],U[i+jp*L]) * dt;
	//	y = y + BilinearInterpolation(x*(L-1),y*(L-1), i,ip, j,jp, V[ i+j*L ],V[ip+j*L],V[ip+jp*L],V[i+jp*L]) * dt;
   
		while(x<0) 	x = 1+x;
		while(y<0)	y = 1+y;
		while(x>1)	x = x-1;
		while(y>1)	y = y-1;

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
	glVertexPointer(2,GL_DOUBLE,0,par);
	glDrawArrays(GL_POINTS,0,npar);
	glDisableClientState(GL_VERTEX_ARRAY);
}
