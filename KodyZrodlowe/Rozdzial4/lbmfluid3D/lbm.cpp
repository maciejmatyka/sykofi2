
// LBM at tau = 1.0;
// by Maciej Matyka 2014-2019
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
#include "lbm.h"

float df[2][W][H][D][q];   

float UX[ W ][ H ][ D ], UY[ W ][ H ][ D ], UZ[ W ][ H ][ D ], R[ W ][ H ][ D ]; 
int F[ W ][ H ][ D ];

const int ex[q] = {0,-1 , 0, 0,-1,-1,-1,-1, 1, 0, 0, 1, 1, 1, 1};
const int ey[q] = {0, 0 ,-1, 0,-1,-1, 1, 1, 0, 1, 0, 1, 1,-1,-1};
const int ez[q] = {0, 0 , 0,-1,-1, 1,-1, 1, 0, 0, 1, 1,-1, 1,-1};
const int inv[q] = {0,8,9,10,11,12,13,14,1,2,3,4,5,6,7};
const float z=2.0/9.0, l=1.0/72.0, s=1.0/9.0;
const float w[q]={z,s,s,s,l,l,l,l,s,s,s,l,l,l,l};
float fx = -0.0000065, fy = 0, fz=0;
float tau = 1;            // vis = (2tau-1)/6 approx 0.167

void initlbm()
{

   for(int i=0; i < W ; i++)   
   for(int j=0; j < H ; j++)   	   	
   for(int m=0; m < D ; m++)   	   	
   {
		for(int k=0; k<q; k++)
			df[0][i][j][m][k] = df[1][i][j][m][k] = w[k];
   }

   for(int i=0; i < W ; i++)   
   for(int j=0; j < H ; j++)   	   	
   for(int m=0; m < D ; m++)   	   	
   {
		UX[i][j][m] = UY[i][j][m] = UZ[i][j][m] = 0.0;
	 	R[i][j][m] = 1;
		F[i][j][m] = 0;
		if((j==0 || j==H-1) || (m==0 || m==D-1)) F[i][j][m] = 1;  // sciana gorna i dolna
		
   }

    /*    int x0=W*(0.75);
        int y0=H*(0.5);
        int z0=D*(0.5);
            
        for(int i=0; i < W ; i++)   
        for(int j=0; j < H ; j++)   	   	
        for(int m=0; m < D ; m++)   	   	
        {
            if(  (i-x0)*(i-x0)+(j-y0)*(j-y0)+(m-z0)*(m-z0)<(H/10)*(H/10) )
		      F[i][j][m] = 1;                   // przeszkoda kwadratowa H/10 x H/10
        }		  */

   
   for(int PRZESZ=0; PRZESZ<120; PRZESZ++)
   {
        int x0=W*(rand()/(float)RAND_MAX);
        int y0=H*(rand()/(float)RAND_MAX);
        int z0=D*(rand()/(float)RAND_MAX);
            
        for(int i=0; i < W ; i++)   
        for(int j=0; j < H ; j++)   	   	
        for(int m=0; m < D ; m++)   	   	
        {
            if(  (i-x0)*(i-x0)+(j-y0)*(j-y0)+(m-z0)*(m-z0)<(H/10)*(H/10) )
		      F[i][j][m] = 1;                   // przeszkoda kwadratowa H/10 x H/10
        }		  
   }
}


// A4 - wartosci makroskopowe
void macro(int c)
{
	for(int i=0 ; i < W; i++)
	for(int j=0 ; j < H; j++)
	for(int m=0 ; m < D; m++)  
	if(F[ i ][ j ][ m ] == 0)
	{
       	float rho=0,ux=0,uy=0,uz=0;
   	   	for(int k=0; k<q; k++)			// calculate density and velocity
		{
			rho = rho 	+ df[c][ i ][ j ][ m ][ k ];
			ux =  ux 	+ df[c][ i ][ j ][ m ][ k ] * ex[ k ];
			uy =  uy 	+ df[c][ i ][ j ][ m ][ k ] * ey[ k ];
			uz =  uz 	+ df[c][ i ][ j ][ m ][ k ] * ez[ k ];
		}
		ux /= rho;
		uy /= rho;	 
		uz /= rho;	 
     
		UX[ i ][ j ][ m ] = ux;
		UY[ i ][ j ][ m ] = uy;
		UZ[ i ][ j ][ m ] = uz;
		R[ i ][ j ][ m ] = rho;
    }
 }


// A5 - oblicz "f z tyldą" - krok kolizji
 void collision(int c)
 {
    float ux, uy, uz, rho;
 	for(int i=0 ; i < W ; i++)
	for(int j=0 ; j < H ; j++)
	for(int m=0 ; m < D ; m++)
	if(F[ i ][ j ][ m ] == 0)
 	{	   
       	ux = UX[ i ][ j ][ m ];
       	uy = UY[ i ][ j ][ m ];
       	uz = UZ[ i ][ j ][ m ];
       	rho = R[ i ][ j ][ m ];
		
	    // siła zewnętrzna
 		ux = ux + fx * tau / rho;		// (wzór 4.54)

      	float feq;	   
      	for(int k=0; k< q; k++)
	    {
         	 // wzór 4.52		
	       feq =  w[ k ] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy+ uz*uz) + 3.0f * (ex[ k ] * ux + ey[ k ]*uy + ez[ k ]*uz) 
				+ (9.0f/2.0f) * (ex[ k ] * ux + ey[ k ]*uy+ ez[ k ]*uz) * (ex[ k ] * ux + ey[ k ]*uy+ ez[ k ]*uz));
		  // "f z tyld¹" zapamiętujemy w miejscu aktualnej funkcji rozkładu
	       df [c][ i ][ j ][ m ][ k ] =  df [c][ i ][ j ][ m ][ k ] - (1/tau)* (df[ c ][ i ][ j ][ m ][ k ] - feq);
	    }  	// pętla po kierunkach
	} // pętla po komórkach sieci
 } 

// A6 - krok transportu
void transport(int c)
{
  for(int i=0 ; i < W ; i++)
  for(int j=0 ; j < H ; j++)
  for(int m=0 ; m < D ; m++)
  if(F[ i ][ j ][ m ] == 0)			// tylko z węzła z płynem
  {
    for(int k=0; k< q; k++)
    {
        int ip = ( i+ex[ k ] + W ) % (W);
        int jp = ( j+ey[ k ] ); 			// int yp = ( j+ey[ k ] + H ) % (H);
        int mp = ( m+ez[ k ] ); 	
         
  		if( F[ip][jp][mp] == 1 )		// docelowy węzeł jest brzegowy?	
	        df[1-c][ i ][ j ][ m ][ inv[ k ] ] = df[c][ i ][ j ][ m ][ k ];		// tak, wykonaj odbicie
		else
  	        df[1-c][ ip ][ jp ][ mp ][ k ] = df[c][ i ][ j ][ m ][ k ];		// nie, normalny transport
  	 } // pętla po kierunkach
  	} 
 }

void lbm(int c)
{
	macro(c);
	collision(c);
	transport(c);

}

/*void LBMTAU1(int c)
{
    float f, r, u, v, x;

	for(int i=0 ; i < W ; i++)		
	for(int j=0 ; j < H ; j++)
    for(int m=0 ; m < D ; m++)  
	if(F[ i ][j][m] == 0 )
	{	
		   R[ c ][i][j][m] = UX[ c ][i][j][m] = UY[ c ][i][j][m] = UZ[ c ][i][j][m] = 0;

		   for(int k=0; k<q; k++)
		   {	
			   int ip = ( i+ex[k] + W ) % W;
			   int jp = ( j+ey[k] + H ) % H;
			   int mp = ( m+ez[k] + D ) % D;
			   int ik = inv[ k ];
  	
          	   if(F[ ip ][ jp ][ mp ] == 0)
		      {
				r = R[ 1-c ][ip][jp][ mp ];     
                u =  (UX[ 1-c ][ip][jp][ mp ] + fx) / r;   
                v = (UY[ 1-c ][ip][jp][ mp ] + fy) / r;
                x = (UZ[ 1-c ][ip][jp][ mp ] + fz) / r; 
				f = w[ik]*r*(1.- (u*u+v*v+x*x)*3./2.+(ex[ik]*u+ey[ik]*v+ez[ik]*x)*3.+pow(ex[ik]*u+ey[ik]*v+ez[ik]*x,2)*9./2.);
	          } else
	   			   f = w[ik] * R[1-c][i][j][m];

	      	    R[ c ][ i ][ j ][m] += f;
		   	    UX[ c ][ i ][ j ][m] += ex[ik]*f;
		   	    UY[ c ][ i ][ j ][m] += ey[ik]*f;	   
		   	    UZ[ c ][ i ][ j ][m] += ez[ik]*f;	   
	  	   }	   
	}
}*/
