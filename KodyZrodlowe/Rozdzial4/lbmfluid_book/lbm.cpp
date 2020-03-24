/**
 * Symulacje Komputerowe w Fizyce, wydanie 2 - rozszerzone
 * Metoda LBM - Implmentacja w C
 * Autor: Maciej Matyka, 2018-12-22
 **/

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#include "lbm.h"

// A1
float df[2][LX][LY][9];   

const int ex[9] = {0,  1,0,-1, 0,  1,-1,-1, 1};
const int ey[9] = {0,  0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9]={4.0/9.0,  1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,   1.0/36.0,1.0/36.0,1.0/36.0,1.0/36.0};

float U[LX][LY], V[LX][LY], R[LX][LY];
int F[LX][LY];

float fx = 0.000001;
float tau = 0.68;            // vis = (2tau-1)/6 approx 0.167


// A2
void initlbm(void)
{
    // inicjalizacja df
	for(int i=0; i < LX ; i++)	
	for(int j=0; j < LY ; j++)	
	for(int k=0; k< 9; k++)	
		df [0][ i ][ j ][k] = df [1][ i ][ j ][k] = w[k];
	
	// czyœæ flagi (ustaw oznaczenia na siatce na 0 - "wszystkie z p³ynem")
	for(int i=0; i < LX; i++)
	for(int j=0; j < LY; j++)
		F[ i ][ j ] = 0;
   
   // ustaw górn¹ i doln¹ powierzchniê jako œciana
  //  for(int i=0; i < LX; i++)
//	   F[ i ][ 0 ] = F[ i ][ LY-1 ] = 1;
   
   // wrysuj ko³o w œrodku uk³adu
	float R = LY/5;                                         // rozmiar
    for(int i=0; i < LX; i++)
    for(int j=0; j < LY; j++)
	 if( (i-LX/2)*(i-LX/2)+(j-LY/2)*(j-LY/2) < R*R )
	     F[ i ][ j ] = 1;   

 // wrysuj prostok¹t w œrodku uk³adu
/*	float R = LY/5;                                         // rozmiar
    for(int i=0; i < LX; i++)
    for(int j=0; j < LY; j++)
	 if( abs(i-LX/2)< R && abs(j-LY/2)< R )
	     F[ i ][ j ] = 1;   	     
*/	     
}



// A4 - wartosci makroskopowe
void macro(int c)
{
	for(int i=0 ; i < LX; i++)
	for(int j=0 ; j < LY; j++)
	if(F[ i ][ j ] == 0)
	{
       	float rho=0,ux=0,uy=0;
   	   	for(int k=0; k<9; k++)			// calculate density and velocity
		{
			rho = rho 	+ df[c][ i ][ j ][ k ];
			ux =  ux 	+ df[c][ i ][ j ][ k ] * ex[ k ];
			uy =  uy 	+ df[c][ i ][ j ][ k ] * ey[ k ];
		}
		ux /= rho;
		uy /= rho;	 
     
		U[ i ][ j ] = ux;
		V[ i ][ j ] = uy;
		R[ i ][ j ] = rho;
    }
 }


// A5 - oblicz "f z tyldą" - krok kolizji
 void collision(int c)
 {
     float ux, uy, rho;
 	for(int i=0 ; i < LX ; i++)
	for(int j=0 ; j < LY ; j++)
	if(F[ i ][ j ] == 0)
 	{	   
       	ux = U[ i ][ j ];
       	uy = V[ i ][ j ];
       	rho = R[ i ][ j ];
		
	    // siła zewnętrzna
 		ux = ux + fx * tau / rho;		// (wzór 4.54)

      	float feq;	   
      	for(int k=0; k< 9; k++)
	    {
         	 // wzór 4.52		
	       feq =  w[ k ] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[ k ] * ux + ey[ k ]*uy) 
				+ (9.0f/2.0f) * (ex[ k ] * ux + ey[ k ]*uy) * (ex[ k ] * ux + ey[ k ]*uy));
		  // "f z tyld¹" zapamiętujemy w miejscu aktualnej funkcji rozkładu
	       df [c][ i ][ j ][ k ] =  df [c][ i ][ j ][ k ] - (1/tau)* (df[ c ][ i ][ j ][ k ] - feq);
	    }  	// pętla po kierunkach
	} // pętla po komórkach sieci
 } 

// A6 - krok transportu
void transport(int c)
{
  for(int i=0 ; i < LX ; i++)
  for(int j=0 ; j < LY ; j++)
  if(F[ i ][ j ] == 0)			// tylko z węzła z płynem
  {
    for(int k=0; k< 9; k++)
    {
        int ip = ( i+ex[ k ] + LX ) % (LX);
        int jp = ( j+ey[ k ] ); 			// int yp = ( j+ey[ k ] + LY ) % (LY);
         
  		if( F[ip][jp] == 1 )		// docelowy węzeł jest brzegowy?	
	        df[1-c][ i ][ j ][ inv[ k ] ] = df[c][ i ][ j ][ k ];		// tak, wykonaj odbicie
		else
  	        df[1-c][ ip ][ jp ][ k ] = df[c][ i ][ j ][ k ];		// nie, normalny transport
  	 } // pętla po kierunkach
  	} 
 }



// A3 - krok LBM
void lbm(void)
{
	static int c = 1;
	c = 1-c;                       // wybierz na której siatce pracujemy (1,0,1,0,1,0...)
    macro(c);
    collision(c);
    transport(c);
}


float tortuosity(void)
{
 /*	// Skrypt Palabos
 	T calc_tortuositypal(MultiBlockLattice2D<T, DESCRIPTOR> &lat)
	 {
		   Box2D crossection(0,0,0,L-1);
		   plint xComponent = 0;
		   //T q = computeSum(*computeVelocityComponent(lat, crossection, xComponent ) );
		   Box2D pm(MAR,MAR+L-1,0,L-1);
		   T absvelsum = computeSum(*computeVelocityNorm(lat, pm) );
		   T xvelsum = computeSum(*computeVelocityComponent(lat, pm, xComponent ));
		   //T t = absvelsum / ( q * L ) ;
		   T t = absvelsum / xvelsum;
		   return t;
	 }
   */

   // calc absvelsum
   float absvelsum = 0, xvelsum = 0;
   
   for(int i=0 ; i < LX ; i++)
   for(int j=0 ; j < LY ; j++)
   if(F[ i ][ j ] == 0)
   {
		absvelsum += sqrt(U[ i ][ j ]*U[ i ][ j ] + V[ i ][ j ]*V[ i ][ j ]);
	    xvelsum += U[ i ][ j ];
   }		    
   
   float t=0;
   if(xvelsum) t = absvelsum / xvelsum;
	
   return t;   
}
