/* 

przyklad2.c


Rozwiazanie rownania rozpadu promieniotworczego 
w postaci bezwymiarowej
metoda RKIV.




autor: Maciej Matyka

*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double k=0.1,u=1,dt=10.0,t=0;


double f(double u)
{
	return(-(k*u));
}


int main(void)
{
	double k1,k2,k3,k4;

  	printf("t       RKIV       Analitycznie\n\n\n");

	for(t=0;t<86;t+=dt)
	{
		printf("%f       %f        %f \n",t,u,exp(-k*t));
		

   		k1=f(u)*dt;
		k2=f(u+0.5*k1)*dt;
		k3=f(u+0.5*k2)*dt;
		k4=f(u+k3)*dt;
	    u+=(1/(float)6)*(k1+2*k2+2*k3+k4);
	}

	return(0);
}

