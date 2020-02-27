/**
 * Symulacje Komputerowe w Fizyce, wydanie 2 - rozszerzone
 * Metoda LBM - Implmentacja w C
 * Autor: Maciej Matyka, 2018-12-22
 **/

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
using namespace std;

#include "lbm.h"
void importppm(string fname, int _nx, int _ny);

// A1
float df[2][LX][LY][9];   

const int ex[9] = {0,  1,0,-1, 0,  1,-1,-1, 1};
const int ey[9] = {0,  0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9]={4.0/9.0,  1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,   1.0/36.0,1.0/36.0,1.0/36.0,1.0/36.0};

float U[LX][LY], V[LX][LY], R[LX][LY];
int F[LX][LY];

float fx = 0;//0.025;//0.0000025;
float fy = 0;//-0.025;

float tau = 1.0;            // 
float vis = (2*tau-1)/6.0;	// viscosity D2Q9

float RADIUS = LY/5;			// promien kola



void initbnd(int m)
{
	// czysc flagi (ustaw oznaczenia na siatce na 0 - "wszystkie z plynem")
	for(int i=0; i < LX; i++)
	for(int j=0; j < LY; j++)
		F[ i ][ j ] = 0;
        
// ustaw gorna i dolna powierzchnie jako sciane
//    for(int i=0; i < LX; i++)
//	   F[ i ][ 0 ] = F[ i ][ LY-1 ] = 1;
        
    // m=0 -> no bnd's
    /*if(m==0)
    {
        for(int i=0; i < LX; i++)
        for(int j=0; j < LY; j++)
    	    F[i][j] = 0;    
    }*/
    // circle
    if(m==1)
    {
        for(int i=0; i < LX; i++)
         for(int j=0; j < LY; j++)
    	 if( (i-LX/2)*(i-LX/2)+(j-LY/2)*(j-LY/2) < RADIUS*RADIUS )
    	     F[i][j] = 1;   
    }
    
    // quad
    if(m==2)
    {
       float R = LY/5;                                         // rozmiar
        for(int i=0; i < LX; i++)
        for(int j=0; j < LY; j++)
	   if( abs(i-LX/2)< R && abs(j-LY/2)< R )
	         F[i][j] = 1;  
    }

    if(m==3)
    {
    	importppm("honda_300x75.ppm",LX,LY);
    	for(int i=0; i < LX; i++)          //- gÃ³rna i dolna Å“ciana
           	   F[ i ][ 0 ] = F[ i ][ LY-1 ] = 1;
    	
    }
}



extern float rho0;
// A2
void initlbm(void)
{
    // inicjalizacja df
    	for(int i=0; i < LX ; i++)	
    	for(int j=0; j < LY ; j++)	
    	{

    		//df [0][ i ][ j ][k] = df [1][ i ][ j ][k] = w[k] * 500 * (j<LY/2) + 80 * (j>=LY/2);  
    		float r,u,v;
    		//if(i>LX/2) R[i][j] = 80.; else R[i][j] = 300.+ rand()/(float)RAND_MAX;
    	/*if(j>LY/2) 
    				R[i][j] = rho0/4.0; 
    			else 
    				R[i][j] = rho0+ (rand()/(float)RAND_MAX);*/
	   		R[i][j] = rho0 + rand()/(float)RAND_MAX;
    		U[i][j] = 0;
    		V[i][j] = 0;

    		r = R[i][j];
    		u = U[i][j];
    		v = V[i][j];

	    	for(int k=0; k<9; k++)	
	    	{
	    		df [0][ i ][ j ][k] = df [1][ i ][ j ][k] =  
	    			w[k] * r * (1.0f - (3.0f/2.0f) * (u*u + v*v) 
	    			+ 3.0f * (ex[k]*u + ey[k]*v) 
					+ (9.0f/2.0f) * (ex[k]*u + ey[k]*v) * (ex[k]*u + ey[k]*v));
	    	}
	    }
    		
   // wrysuj kolo w srodku ukladu
    //initbnd(0);	                                         // rozmiar
}


// A4 - wartosci makroskopowe
void macro(int c)
{
	for(int x=0 ; x < LX ; x++)
	for(int y=0 ; y < LY ; y++)
	if(F[ x ][ y ] == 0)
	{
        float rho=0,ux=0,uy=0;
    	for(int i=0; i<9; i++)			// calculate density and velocity
		{
			rho = rho 	+ df[c][ x ][ y ][i];
			ux =  ux 	+ df[c][ x ][ y ][i] * ex[i];
			uy =  uy 	+ df[c][ x ][ y ][i] * ey[i];
		}
		ux /= rho;
		uy /= rho;	 
     
     	U[ x ][ y ] = ux;        // force
		V[ x ][ y ] = uy;
		R[ x ][ y ] = rho;
    }
    else
    {
    	U[ x ][ y ] = 0;
		V[ x ][ y ] = 0;
		R[ x ][ y ] = 0;
    }

}

// M1 - do opisu oddzia³ywañ dla przep³ywu wielofazowego
float psi0 = 4.0;
float rho0 = 200.;
float G = -160.0;//-120;

// M2 - funkcja oddziaływania
float psi(int i, int j)
{
	return psi0 * exp(-rho0 / R[ i ][ j ]);
}

// A5 - oblicz "f z tylda" - krok kolizji
void collision(int c)
{
    float ux,uy,rho;
	for(int i=0 ; i < LX ; i++)
	for(int j=0 ; j < LY ; j++)
	if(F[ i ][ j ] == 0)       // (model BGK bez relaksacji na komorkach brzegowych: Zou, He, 1997)
	{	   
        ux = U[ i ][ j ];
        uy = V[ i ][ j ];
        rho = R[ i ][ j ];

		float forcex, forcey;		    

 	    // M3 - siła oddziaływania dla faz o różnej gęstości
		float multifx=0;
		float multify=0;

	    for(int k=0; k<9; k++)			// petla po kierunkach bez 0, wzor (4.67)
	   	{
	    	int xp = ( i+ex[ k ] + LX ) % (LX);     // warunki periodyczne 0-1+LX->LX-1, LX-1+1->0
	       	int yp = ( j+ey[ k ] + LY ) % (LY);
	    	multifx = multifx + w[ k ] * psi(xp, yp) * ex[ k ];	// wzor (4.67)
	    	multify = multify + w[ k ] * psi(xp, yp) * ey[ k ];
		}
		multifx = multifx * (-G*psi(i,j));	// wzor (4.67)
		multify = multify * (-G*psi(i,j));

	    forcex = fx + multifx;
	    forcey = fy + multify;

         // sila zewnetrzna             
       	ux = ux + forcex * tau / rho;		// wzor (4.54)                 ----->II, https://doi.org/10.1016/j.ijheatmasstransfer.2009.11.014
       	uy = uy + forcey * tau / rho;		// wzor (4.54)                 ----->II, https://doi.org/10.1016/j.ijheatmasstransfer.2009.11.014
        
        // Forcing thread 2: http://www.palabos.org/forum/read.php?3,1075,4792
        // Kuzmin.pdf
        // Fi=-3 omega_i rho c_i * F / c^2
        float feq;	   
        for(int k=0; k< 9; k++)
	    {
          // rownanie 4.52		
	       feq =  w[k] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[k] * ux + ey[k]*uy) 
				+ (9.0f/2.0f) * (ex[k] * ux + ey[k]*uy) * (ex[k] * ux + ey[k]*uy));
		  // "f z tylda" zapamietujemy w miejscu aktualnej funkcji rozkladu
		  
		   	//float Fi=3*w[i]*rho*(ex[i]*forcex + ey[i]*forcey);       // fy=0  // A.A. Mohamad, A. Kuzmin / International Journal of Heat and Mass Transfer 53 (2010) 990â€“996
			float Fk = 0;	    
			//Fi /= (rho);   
		  
	       df [c][ i ][ j ][k] =  df [c][ i ][ j ][k] - (1/tau)* (df[c][ i ][ j ][k] - feq) + Fk;	       
	    }  

	    
	}
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
        int xp = ( i+ex[ k ] + LX ) % (LX);
        int yp = ( j+ey[ k ] + LY ) % (LY);
         
  		if( F[xp][yp] == 1 )		// docelowy węzeł jest brzegowy?	
	        df[1-c][ i ][ j ][ inv[ k ] ] = df[c][ i ][ j ][ k ];		// tak, wykonaj odbicie
		else
  	        df[1-c][ xp ][ yp ][ k ] = df[c][ i ][ j ][ k ];		// nie, normalny transport
  	 } // pętla po kierunkach
  } 
}

// A3 - krok LBM
void lbm(void)
{
	static int c = 1;
	c = 1-c;                       // wybierz na ktÃƒÂ³rej siatce pracujemy (1,0,1,0,1,0...)
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
   
   for(int x=0 ; x < LX ; x++)
   for(int y=0 ; y < LY ; y++)
   if(F[ x ][ y ] == 0)
   {
		absvelsum += sqrt(U[ x ][ y ]*U[ x ][ y ] + V[ x ][ y ]*V[ x ][ y ]);
	    xvelsum += U[ x ][ y ];
   }		    
   
   float t=0;
   if(xvelsum) t = absvelsum / xvelsum;
	
   return t;   
}





void importppm(string fname, int _nx, int _ny)
{
	int i,j;

	ifstream ppmfile(fname.c_str()); // open the file
	
	if(!ppmfile)
	{
		cout << "Cannot open the file: " << fname << endl;
		exit(1);
	}

	#define line fname  // don't waste one string

	// move the header
	getline (ppmfile,line);			// P3
	getline (ppmfile,line);			// NX, NY
	getline (ppmfile,line);			// # Created by IrfanView 
	getline (ppmfile,line);			// depth
		
	for(j=0; j < _ny; j++)				// read data
	for(i=0; i < _nx; i++)
	{
		ppmfile >> line;						// R
		if( atoi(line.c_str()) == 255)
			F[i][LY-j] = 0;
		else
			F[i][LY-j] = 1;
		ppmfile >> line; ppmfile >> line;		// G,B
	}

	// close file
	ppmfile.close();
}

void exportvtk(void)
{
/*
1. # vtk DataFile Version 2.0
2. Komentarz - nasze pole prÃ„â„¢dkoÃ…â€ºci policzone metodÃ„â€¦ LBM
3. ASCII
4. DATASET STRUCTURED_POINTS
5. DIMENSIONS 4 2 1
6. ORIGIN 0 0 0
7. SPACING 1 1 1
8. POINT_DATA 8
10. VECTORS PolePredkosci double
11. 1.0 0.0 0.0*/

	ofstream file("velocity.vtk");
	file << "# vtk DataFile Version 2.0\nLBM, Symulacje Komputerowe w Fizyce 2, Maciej Matyka 2019\n";
	file << "ASCII\nDATASET STRUCTURED_POINTS\n";
	file << "DIMENSIONS " << LX << " " << LY << " 1\n";
	file << "ORIGIN 0 0 0\nSPACING 1 1 1\n";
	file << "POINT_DATA " << LX*LY*1 << "\n";
	file << "VECTORS PolePredkosci double\n";

	for(int j=0; j<LY; j++)
	for(int i=0; i<LX; i++)
	{
		file << U[i][j] << " " << V[i][j] << " 0.0" << endl; 
	}

	file.close();

	// zapisz gestosc-1 (do cisnienia)
	ofstream file2("density.vtk");
	file2 << "# vtk DataFile Version 2.0\nLBM, Symulacje Komputerowe w Fizyce 2, Maciej Matyka 2019\n";
	file2 << "ASCII\nDATASET STRUCTURED_POINTS\n";
	file2 << "DIMENSIONS " << LX << " " << LY << " 1\n";
	file2 << "ORIGIN 0 0 0\nSPACING 1 1 1\n";
	file2 << "POINT_DATA " << LX*LY*1 << "\n";
	file2 << "SCALARS Gestosc double\n";
	file2 << "LOOKUP_TABLE default\n";
	for(int j=0; j<LY; j++)
	for(int i=0; i<LX; i++)
	{

		if(!F[i][j])
			file2 << ((R[i][j]-1.0)*10000.0) << endl; 
		else
			file2 << 0 << endl;
	}
	file2.close();
}
void exportprofile(void)
{
	ofstream file("poiseuille.dat");
	file << "#y u" << endl;
	for(int j=0; j<LY; j++)
		file << j << " " << U[LX/2][j] << endl; 
	file.close();


    const int NUM = 10;     // liczba profili prÃªdkoÅ“ci
    for(int i=0; i<LX; i+=LX/NUM)
    {
        char workbuf[80];
        sprintf(workbuf, "poiseuille_i=%d.dat", i);
    	ofstream file2(workbuf);
    	file2 << "#y u" << endl;
    	for(int j=0; j<LY; j++)
    	if(F[i][j] == 0)
    		file2 << j << " " << U[ i ][j] << endl; 
//    	else
 //       	file << j << " " << -1000 << endl; 
    	file2.close();
    }


    // export velocity field

 	ofstream file3("velocity.dat");
   	file3 << "#x y vx vy" << endl; 
    for(int i=0; i<LX; i++)
    for(int j=0; j<LY; j++)
    	if(F[i][j] == 0)
    		file3 << i << " " << j << " " << U[i][j] << " " << V[i][j] << endl; 
   	file3.close();

}

// Oblicz liczbe Reynoldsa Re = uL/mu na podstawie:
// - sredniej predkosci wzdluz przekroju
// - lepkosci kinematycznej
// - wielkosci ciala (np. srednicy)
float Reynolds(void)
{
	// oblicz LENGTH (wysokosc w polowie ukladu)
	float LENGTH = 0;
	for(int j=LY-2; j >= 0 && F[LX/2][j] != 1; j--)
	{
		LENGTH += 1;
	}
	LENGTH = LY - LENGTH;		//	 BIERZEMY WYSOKOÅšÄ† AUTA
	//cout << "LEN: " << LENGTH << "ENDL ";

	// oblicz u srednie
	float u=0;
	int s=0;	
	for(int j=0; j < LY; j++)
	{
		if(F[2][j]==0)		// plyn
		{
			s++;
			u = u + U[2][j];
		}
	}
	u /= s;			// u srednie
	//float LENGTH = RADIUS;

	return u*LENGTH/vis;
}
