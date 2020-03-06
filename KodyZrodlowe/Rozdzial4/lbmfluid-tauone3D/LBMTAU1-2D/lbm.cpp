
// LBM at tau = 1.0;
// by Maciej Matyka 2014-2019

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
#include "lbm.h"

float U[ 2 ][ LX ][ LY ], V[ 2 ][ LX ][ LY ], R[ 2 ][ LX ][ LY ]; 
int F[ LX ][ LY ];
const int ex[9] = {0, 1,0,-1, 0,  1,-1,-1, 1}, ey[9] = {0, 0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9] = {4./9., 1./9., 1./9., 1./9., 1./9., 1./36., 1./36., 1./36., 1./36.};
float fx = -0.000065, fy = 0;

float ReynoldsNumber(int c)
{
	// oblicz u srednie
	float u=0;
	int s=0;	
	for(int j=0; j < LY; j++)
	{
		if(F[2][j]==0)		// plyn
		{
			s++;
			u = u + U[c][LX/2][j];
		}
	}
	u /= s;			// u srednie

   	float visc = 0.333*(1.0-0.5);
	return fabs(u) * L0 / visc;
}

void initlbm()
{
   for(int i=0; i < LX ; i++)   
   for(int j=0; j < LY ; j++)   	   	
   {
		U[ 0 ][i][j] = V[ 0 ][i][j] = U[ 1 ][i][j] = V[ 1 ][i][j] = 0.0;
	 	R[ 0 ][i][j] = R[ 1 ][i][j] = 1;
		F[i][j] = 0;
		if(j==0 || j==LY-1) F[i][j] = 1;  // sciana gorna i dolna
		if(abs(i-(LX-LX/5))<LY/10 && abs(j-LY/2)<L0) 
		  F[i][j] = 1;                   // przeszkoda kwadratowa LY/10 x LY/10
   }
}

void LBMTAU1(int c)
{
    float f, r, u, v;

	for(int i=0 ; i < LX ; i++)		
	for(int j=0 ; j < LY ; j++)
	if(F[ i ][j] == 0 )
	{	
		R[ c ][i][j] = U[ c ][i][j] = V[ c ][i][j] = 0;

		for(int k=0; k<9; k++)
		{	
			int ip = ( i+ex[k] + LX ) % LX;
			int jp = ( j+ey[k] + LY ) % LY;
	   		int ik = inv[ k ];
  	
          	if(F[ ip ][ jp ] == 0)
		    {
				r = R[ 1-c ][ip][jp];     
				u = (U[ 1-c ][ip][jp] + fx) / r;   
				v = (V[ 1-c ][ip][jp] + fy) / r; 
				
				f = w[ik]*r* (1.- (u*u+v*v)*3./2.+(ex[ik]*u+ey[ik]*v)*3.+pow(ex[ik]*u+ey[ik]*v,2)*9./2.); 
	        } else
	   			f = w[ik] * R[1-c][i][j];

	      	R[ c ][ i ][ j ] += f;
		   	U[ c ][ i ][ j ] += ex[ik]*f;
		   	V[ c ][ i ][ j ] += ey[ik]*f;	   
	  	}	   
	}
}
