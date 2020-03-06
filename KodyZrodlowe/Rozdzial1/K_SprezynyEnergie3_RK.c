
/*

Badanie ca³kowitej energii oscylatora harmonicznego bez t³umienia
metod¹ Runge-Kutta IV.

Napisane na potrzeby ksi¹¿ki "Symulacje Komputerowe w Fizyce"
autor: Maciej Matyka, grudzieñ 2001 r.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double k=0.9;
double c=0.4;

double x=54,x2=54,dx=0,dx2=0,v=0,v2=0,dv=0,dv2=0;

double t=0,dt=3.5,g=0; 
double m=3.0;


double Ee,Em;

double f(double xk)
{
	return(-(k/m)*xk+g);
}


int main(void)
{
	double k1,k2,k3,k4;
	
	for(t=0;t<1140;t+=dt)
	{

	// metoda rkIV

		k1=f(x)*dt;
		k2=f(x+0.5*k1)*dt;
		k3=f(x+0.5*k2)*dt;
		k4=f(x+k3)*dt;
	    v+=(1/6)*(k1+2*k2+2*k3+k4);

	    dx=v*dt;
	    x=x+dx;

  	// metoda Eulera


	    dv2=f(x2)*dt;
	    v2+=dv2;
	    dx2=v2*dt;
	    x2=x2+dx2;

if(t>1090)
		  printf("%f      %f     %f\n",t,((m*v*v)+(k*x*x))/2,((m*v2*v2)+(k*x2*x2))/2);

		
	

	}

		return(0);
}  

