
/*

Badanie ca³kowitej energii oscylatora harmonicznego bez t³umienia
metod¹ MidPoint (punktu œrodkowego).

Napisane na potrzeby ksi¹¿ki "Symulacje Komputerowe w Fizyce"
autor: Maciej Matyka, grudzieñ 2001 r.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double k=0.5;
double x=14,x2=14,dx=0,dx2=0,v=0,v2=0,dv=0,dv2=0;

double t=0,dt=0.015,g=0;


double Ee,Em;

double f(double xk)
{
	return(-(k)*xk+g);
}


int main(void)
{
	double k1,k2;
	
	double T=0,V=0;


	for(t=0;t<10;t+=dt)
	{

	// metoda midpoint

//		Em = 0.5*k*(x)*(x)-g*x;



		k1=f(x)*dt;
		k2=f(x+0.5*k1)*dt;
	    v+=k2;

	    dx=v*dt;
	    x=x+dx;

		T=(v*v)/2+g*x;
		V=(k*x*x)/2;



		printf("%f      %f      %f   %f\n",t,T,V,T+V);


		Em = Em+(v*v)/2; 

  	//metoda Eulera

		Ee=0.5*k*x2*x2-g*x2;

	    dv2=f(x2)*dt;
	    v2+=dv2;
	    dx2=v2*dt;
	    x2=x2+dx2;

		Ee = Ee+(v2*v2)/2; 
		



									  
	}

		return(0);
}  

