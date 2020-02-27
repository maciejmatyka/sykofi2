
// LBM at tau = 1.0;
// by Maciej Matyka 2014-2019
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
#include "lbm.h"
#define W LX
#define H LY
#define D LZ

float U[ 2 ][ W ][ H ][ D ], V[ 2 ][ W ][ H ][ D ], X[ 2 ][ W ][ H ][ D ], R[ 2 ][ W ][ H ][ D ]; 
int F[ W ][ H ][ D ];

const int ex[q] = {0,-1 , 0, 0,-1,-1,-1,-1, 1, 0, 0, 1, 1, 1, 1};
const int ey[q] = {0, 0 ,-1, 0,-1,-1, 1, 1, 0, 1, 0, 1, 1,-1,-1};
const int ez[q] = {0, 0 , 0,-1,-1, 1,-1, 1, 0, 0, 1, 1,-1, 1,-1};
const int inv[q] = {0,8,9,10,11,12,13,14,1,2,3,4,5,6,7};
const float z=2.0/9.0, l=1.0/72.0, s=1.0/9.0;
const float w[q]={z,s,s,s,l,l,l,l,s,s,s,l,l,l,l};
float fx = -0.000065, fy = 0, fz=0;

void initlbm()
{
   for(int i=0; i < W ; i++)   
   for(int j=0; j < H ; j++)   	   	
   for(int m=0; m < D ; m++)   	   	
   {
		U[ 0 ][i][j][m] = V[ 0 ][i][j][m] = X[ 0 ][i][j][m] = U[ 1 ][i][j][m] = V[ 1 ][i][j][m] = X[ 1 ][i][j][m] = 0.0;
	 	R[ 0 ][i][j][m] = R[ 1 ][i][j][m] = 1;
		F[i][j][m] = 0;
		if((j==0 || j==H-1) || (m==0 || m==D-1)) F[i][j][m] = 1;  // sciana gorna i dolna
		
   }
   
   for(int PRZESZ=0; PRZESZ<20; PRZESZ++)
   {
        int x0=W*(rand()/(float)RAND_MAX);
        int y0=H*(rand()/(float)RAND_MAX);
        int z0=D*(rand()/(float)RAND_MAX);
            
        for(int i=0; i < W ; i++)   
        for(int j=0; j < H ; j++)   	   	
        for(int m=0; m < D ; m++)   	   	
        {
            if(  (i-x0)*(i-x0)+(j-y0)*(j-y0)+(m-z0)*(m-z0)<(H/5)*(H/5) )
		      F[i][j][m] = 1;                   // przeszkoda kwadratowa H/10 x H/10
        }		  
   }
}

void LBMTAU1(int c)
{
    float f, r, u, v, x;

	for(int i=0 ; i < W ; i++)		
	for(int j=0 ; j < H ; j++)
    for(int m=0 ; m < D ; m++)  
	if(F[ i ][j][m] == 0 )
	{	
		   R[ c ][i][j][m] = U[ c ][i][j][m] = V[ c ][i][j][m] = X[ c ][i][j][m] = 0;

		   for(int k=0; k<q; k++)
		   {	
			   int ip = ( i+ex[k] + W ) % W;
			   int jp = ( j+ey[k] + H ) % H;
			   int mp = ( m+ez[k] + D ) % D;
			   int ik = inv[ k ];
  	
          	   if(F[ ip ][ jp ][ mp ] == 0)
		      {
				r = R[ 1-c ][ip][jp][ mp ];     
                u =  (U[ 1-c ][ip][jp][ mp ] + fx) / r;   
                v = (V[ 1-c ][ip][jp][ mp ] + fy) / r;
                x = (X[ 1-c ][ip][jp][ mp ] + fz) / r; 
				f = w[ik]*r*(1.- (u*u+v*v+x*x)*3./2.+(ex[ik]*u+ey[ik]*v+ez[ik]*x)*3.+pow(ex[ik]*u+ey[ik]*v+ez[ik]*x,2)*9./2.);
	          } else
	   			   f = w[ik] * R[1-c][i][j][m];

	      	    R[ c ][ i ][ j ][m] += f;
		   	    U[ c ][ i ][ j ][m] += ex[ik]*f;
		   	    V[ c ][ i ][ j ][m] += ey[ik]*f;	   
		   	    X[ c ][ i ][ j ][m] += ez[ik]*f;	   
	  	   }	   
	}
}
