/** LBM MRT Implementation, Maciej Matyka
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;
#include "lbm.h"
const int Q=9;
const int ex[Q] = {0,1,0,-1, 0, 1,-1,-1, 1};
const int ey[Q] = {0,0,1, 0,-1, 1, 1,-1,-1};
const int inv[Q] = {0,3,4,1,2,7,8,5,6};
const double w[Q]={4.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/36.0,1.0/36.0,1.0/36.0,1.0/36.0};

float U0 = 0.2;		// 0.51, 0.11
float goalRe = 20000; 
//const int relax = 0;		// 0-BGK
const int relax = 1;		// 1-MRT
const int MODE_ZOUHE = 0; // top lid acceleration mode
const int MODE_EQUILIBRIUM = 1;
const int MODE_MOMENTUM = 2;
//const int boundarymode = MODE_ZOUHE; 	// 1-Zou/He
const int boundarymode = MODE_EQUILIBRIUM;     // feq
//const int boundarymode = MODE_MOMENTUM;

double df[2][L*L][Q];

double m[Q];   // df in moment space for MRT model (C. Rettinger 2013)
double dm[Q];  // 
double deltaf[Q];
double meq[Q];

double M[Q*Q] = 
{
    1,1,1,1,1,1,1,1,1,
   -4,-1,-1,-1,-1,2,2,2,2,
   4,-2,-2,-2,-2,1,1,1,1,
   0,1,0,-1,0,1,-1,-1,1,
   0,-2,0,2,0,1,-1,-1,1,
   0,0,1,0,-1,1,1,-1,-1,
   0,0,-2,0,2,1,1,-1,-1,
   0,1,-1,1,-1,0,0,0,0,
   0,0,0,0,0,1,-1,1,-1 
}; // M matrix for MRT

double M_inv[Q*Q] =
{
 	  0.11111,  -0.11111,   0.11111,   0.00000,  -0.00000,   0.00000,  -0.00000,   0.00000,   0.00000,
      0.11111,  -0.02778,  -0.05556,   0.16667,  -0.16667,   0.00000,   0.00000,   0.25000,   0.00000,
      0.11111,  -0.02778,  -0.05556,   0.00000,   0.00000,   0.16667,  -0.16667,  -0.25000,   0.00000,
      0.11111,  -0.02778,  -0.05556,  -0.16667,   0.16667,   0.00000,   0.00000,   0.25000,   0.00000,
      0.11111,  -0.02778,  -0.05556,   0.00000,   0.00000,  -0.16667,   0.16667,  -0.25000,   0.00000,
      0.11111,   0.05556,   0.02778,   0.16667,   0.08333,   0.16667,   0.08333,   0.00000,   0.25000,
      0.11111,   0.05556,   0.02778,  -0.16667,  -0.08333,   0.16667,   0.08333,   0.00000,  -0.25000,
      0.11111,   0.05556,   0.02778,  -0.16667,  -0.08333,  -0.16667,  -0.08333,   0.00000,   0.25000,
      0.11111,   0.05556,   0.02778,   0.16667,   0.08333,  -0.16667,  -0.08333,   0.00000,  -0.25000
};



double U[L*L], V[L*L], R[L*L];
int FLAG[L*L];
double fx = 0.00;//14;
double fy = 0.00;
//double omega = 1.95;//89;//9999;//1.28;
// vis = (2tau-1)/6
// Re = U0 * L0 / vis
// tau = 3*U0*L0/Re+0.5

double tau = 3*U0*L/goalRe + 0.5;//0.515;//1.0/omega;
double omega = 1.0 / tau;
double S[Q]; 

float volumeflux(void)
{
  // oblicz strumień objętości (z pzekroju wzdłuż osi y), równanie 4.65

  float Q = 0;          // objętościowe natężenie przepływu (4.64)
  float dy = 1.0 / L;  // H/LY
  float dA = dy;        // długość w 2D
  float A = L;        // pole powierzchni przekroju (długość w 2D)

  for(int j=0; j<L; j++)
  for(int i=0; i<L; i++)
  {
    Q += U[i+j*L] * dA;
  }

  float q = Q / A;
  return q;
}

float volumeflux2d(void)
{
  // oblicz strumień objętości (z pzekroju wzdłuż osi y), równanie 4.65

  float Q = 0;          // objętościowe natężenie przepływu (4.64)
  float dy = 1.0 / L;  // H/LY
  float dA = dy;        // długość w 2D
  float A = L;         // pole powierzchni przekroju (długość w 2D)
  int i=L/2;           // pozycja przekroju
  for(int j=0; j<L; j++)
  {
    Q += U[i+j*L] * dA;
  }

  float q = Q / A;
  return q;
}


void savevdf(int v)
{
   char workbuf[80];
   sprintf(workbuf,"vdf%d.dat",v);

   ofstream file(workbuf);

   for(int x=0 ; x < L ; x++)
   for(int y=0 ; y < L ; y++)
   if(FLAG[ x+y*L ] == 0)
   {
    file << U[ x+y*L ] << " " << V[ x+y*L ] << " " << sqrt(U[ x+y*L ]*U[ x+y*L ] + V[ x+y*L ]*V[ x+y*L ]) << endl;
   }

   file.close();
}


float ReynoldsNumber()
{
   	float visc = 0.333*(tau-0.5);
	return U0 * L / visc;
}


void updateMRTS()
{
 // International Journal of Materials, Mechanics and Manufacturing, Vol. 2, No. 4, November 2014   
   S[0] = S[3] = S[5] = 1;
   S[1] = S[2] = 1.4;
   S[4] = S[6] = 1.2;
   S[7] = S[8] = omega;
}


void updateRe(float _re)
{
   goalRe = _re;
   tau = 3*U0*L/goalRe + 0.5;
   omega = 1.0 / tau;
   updateMRTS();
   cout << "omega: " << omega << endl;
}



void initlbm(void)
{
   
   // init S for MRT (values from Zhaoli Guo, LBM Book)
/*   S[0]=S[3]=S[5]=0;   
   S[7]=S[8]=1.0/tau;
   S[4]=S[6]=8*(2-S[7])/(8-S[7]);
   S[1]=1.6;
   S[2]=1.8;
  */

   // International Journal of Materials, Mechanics and Manufacturing, Vol. 2, No. 4, November 2014
   updateMRTS();
/*   	S[0] = S[3] = S[5] = 1;
   	S[1] = S[2] = 1.4;
	S[4] = S[6] = 1.2;
    S[7] = S[8] = omega;
*/	 
   
   for(int j=0; j < L*L ; j++)	
   		FLAG[j]=0;

	for(int j=0; j < L*L ; j++)	
	for(int k=0; k< Q; k++)	
		df [0][ j ][k] = df [1][ j ][k] = w[k];//*k/19.0;
      
   // driven cavity
   for(int i=0; i<L; i++)
   {
	  //FLAG[0+i*L] = FLAG[L-1+i*L] = FLAG[i+0*L] = 1;
	  //FLAG[i+(L-1)*L] = 2;
	  
	  // boundaries
	  FLAG[0+i*L] = FLAG[L-1+i*L] = FLAG[i+0*L] = FLAG[i+(L-1)*L] = 1;
   }
   //FLAG[0+(L-1)*L] = FLAG[L-1+(L-1)*L] = 2;

   // moving bnd
   for(int i=0; i<L; i++)
	 FLAG[i+(L-1)*L] = 2;
   
   
   
	
   /*for(int i=0; i < 10; i++)
   for(int j=0; j < 10; j++)
   for(int k=0; k< Q; k++)
   if(k!=1)
   {			
	   df [0][ j ][k] *=0.9;
	   df [1][ j ][k] *=0.9;
   }*/
   
/*	const int R=12;
	for(int i=(L-R)/2; i < (L-R)/2 + R; i++)
	for(int j=(L-R)/2; j < (L-R)/2 + R ; j++)
		FLAG[i+j*L] = 1;
 */
   
/*	const int R2=14;
	for(int i=(L-R2)/4; i < (L-R2)/4 + R; i++)
	for(int j=(L-R2)/3; j < (L-R2)/3 + R ; j++)
		FLAG[i+j*L] = 1;*/
}


void lbm(void)
{
   static int c = 0;
   c = 1-c;

   // update moving bnd
   
   if(boundarymode==MODE_EQUILIBRIUM)
	 {
	   for(int x=0 ; x < L ; x++)
	   for(int y=0 ; y < L ; y++)
	   if(FLAG[ x+y*L ] == 2)
	   {
		  float ux=U0;
		  float uy=0;
		  float rho=1;	

		  for(int i=0; i<Q; i++)
		  {   
		 	double feq =  w[i] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[i] * ux + ey[i]*uy) 
						+ (9.0f/2.0f) * (ex[i] * ux + ey[i]*uy) * (ex[i] * ux + ey[i]*uy));
			df[c][x+y*L][i] = df[1-c][x+y*L][i] = feq;
		  }	  
	   }
	 }
   
 

   for(int x=0 ; x < L ; x++)
   for(int y=0 ; y < L ; y++)
   if(FLAG[ x+y*L ] == 0 || FLAG[ x+y*L ] == 2)
   {
        double rho=0,ux=0,uy=0;
        for(int i=0; i<Q; i++)          // calculate density and velocity
        {
            rho = rho   + df[c][ x+y*L ][i];
            ux =  ux    + df[c][ x+y*L ][i] * ex[i];
            uy =  uy    + df[c][ x+y*L ][i] * ey[i];
        }
       
        ux /= rho;
        uy /= rho;     
        U[ x+y*L ] = ux;//+0.5*fx;
        V[ x+y*L ] = uy;//+0.5*fy;
        R[ x+y*L ] = rho;
   }

   int idx;
   for(int x=0 ; x < L ; x++)
   for(int y=0 ; y < L ; y++)
   {
   	  idx = x+y*L;
	  if(FLAG[ idx ] == 0 || FLAG[ idx ] == 2)
	  {		 
		 
	   if(relax==1)		// MRT
	   {
			  
		 for(int j=0; j<Q; j++)
		   m[j] = meq[j] = 0;
		 
		 // step 1: f -> m
		 for(int j=0; j<Q; j++)
	  	 {
			for(int i=0; i<Q; i++)		// M (j-th row) x df(elements)
         		m[j] += M[i + j*Q] * df[c][ idx ][ i ];		 				
		 }

		 double rho = m[0];
		 double jx = m[3];
		 double jy = m[5];
		 meq[0] = m[0];
		 meq[3] = m[3];
		 meq[5] = m[5];		   
		   
		 meq[1] = -2.0*rho + 3.0*(jx*jx + jy*jy); // Rettinger 2013, p. 8
		 meq[2] = rho - 3.0*(jx*jx + jy*jy);
		 meq[4] = -jx;
		 meq[6] = -jy;
		 meq[7] = jx*jx - jy*jy;
		 meq[8] = jx*jy;
		 
		 // step 3: collision
		 for(int k=0; k<Q; k++)
			 dm[k] = S[k] * (m[k] - meq[k]);

		 // step 4:
		 // get back to vector space
		 // multiply M_inv * dm
		 for(int i=0; i<Q; i++) deltaf[i] = 0;
		 
		 for(int j=0; j<Q; j++)
		 {
			for(int i=0; i<Q; i++)      // M (j-th row) x df(elements)
				deltaf[j] += M_inv[i + j*Q] * dm[i];
		 }
		 
		 

		 // step 5: collision step in velocity space
	  	 for(int i=0; i< Q; i++) 
		    	df[c][idx][i] -= deltaf[ i ];		  		  
		 

		 // add force term (p 14, eq. 3.3 Rettinger 2013)
//		 for(int k=0; k<Q; k++)
//		 {
//		 	double fkx = 3*w[k]*R[idx] * (ex[k] * fx + ey[k] * fy);
//			df[c][idx][k] += fkx;
//		 }
       }  // relax MRT
		 

	  if(relax==0)  // BGK
	  {
			  
	   	// compute f_tilde (collision step)
		// BGK
	   double ux = U[idx];
	   double uy = V[idx];
	   double rho = R[idx];
	   double feq;	   
	   for(int i=0; i< 9; i++)
	   {		
	      feq =  w[i] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[i] * ux + ey[i]*uy) 
				+ (9.0f/2.0f) * (ex[i] * ux + ey[i]*uy) * (ex[i] * ux + ey[i]*uy));
	      df [c][x+y*L][i] =  df [c][x+y*L][i] - (1/tau)* (df[c][x+y*L][i] - feq);
	   }  
	  }
		 		 
   	} //FLAG
   }//xy

									     
									   

//   cout << U[5*5] << endl;

   
   
   
   // streaming
  for(int x=0 ; x < L ; x++)
  for(int y=0 ; y < L ; y++)
  if(FLAG[ x+y*L ] == 0 || (FLAG[ x+y*L ] == 2 && boundarymode != MODE_MOMENTUM))
  {
     for(int i=0; i<Q; i++)
	 {
		//int xp = x+ex[i];// ( x+ex[i] + L ) % (L);
		//int yp = y+ey[i];//( y+ey[i] + L ) % (L);
		
		int xp = ( x+ex[i] + L ) % (L);
		int yp = ( y+ey[i] + L ) % (L);
		
		if((xp>=0 && yp>=0 && xp<L && yp<L) || ( FLAG[xp+yp*L] == 2 && boundarymode != MODE_MOMENTUM))
		  {
			 
			if( FLAG[xp+yp*L] == 1 )
				df[1-c][ x+y*L ][inv[i]] = df[c][ x+y*L ][i];     
			else if( FLAG[xp+yp*L] == 0 || ( FLAG[xp+yp*L] == 2 && boundarymode != MODE_MOMENTUM))
				df[1-c][ xp+yp*L][i] = df[c][ x+y*L ][i];                 
			else if ( FLAG[xp+yp*L] == 2 && boundarymode == MODE_MOMENTUM)
			   {
				  //f α (i, N y − 1) = f α (i, N y − 1) + 6ρw α e αy U p , where f α (i, N y − 1)
				  // http://arxiv.org/abs/1202.6351, page 20, bottom (Nicolas Delbosc, email)
				  // 
				  df[1-c][ x+y*L][ inv[i] ] = df[c][ x+y*L ][i] + 6*w[inv[i]]*R[x+y*L]*ex[inv[i]]*U0;
			   }
			 
		  }
		
	 }
  }


  if(boundarymode == MODE_ZOUHE)
  {

		
	 
 	float ux=U0;//0.001;//U0;
 	float uy = 0;
 	float rho = 1;
   
 	    for(int y=0 ; y < L ; y++)
		 for(int x=0 ; x < L ; x++)
	     if(FLAG[ x+y*L ] == 2)
		 {
				rho = (1.0/(1.0-uy))* (df[1-c][x+y*L][0] + df[1-c][x+y*L][1] + df[1-c][x+y*L][3] + 2*(df[1-c][x+y*L][6]+df[1-c][x+y*L][2]+df[1-c][x+y*L][5]));			
		        df[1-c][x+y*L][4] = df[1-c][x+y*L][2] + (2.0/3.0) * rho * uy;
		        df[1-c][x+y*L][8] = df[1-c][x+y*L][6] - (1.0/2.0) * (df[1-c][x+y*L][1] - df[1-c][x+y*L][3]) + (1.0/6.0) * rho * uy + (1.0/2.0) * rho * ux;
		        df[1-c][x+y*L][7] = df[1-c][x+y*L][5] + (1.0/2.0) * (df[1-c][x+y*L][1] - df[1-c][x+y*L][3]) + (1.0/6.0) * rho * uy - (1.0/2.0) * rho * ux;
		 }
	 
  }
   
	   
   
}

   
/*	static int c = 1;
	c = 1-c;
	for(int x=0 ; x < L ; x++)
	for(int y=0 ; y < L ; y++)
	if(FLAG[ x+y*L ] == 0)
	{
		double rho=0,ux=0,uy=0;
		for(int i=0; i<9; i++)			// calculate density and velocity
		{
			rho = rho 	+ df[c][ x+y*L ][i];
			ux =  ux 	+ df[c][ x+y*L ][i] * ex[i];
			uy =  uy 	+ df[c][ x+y*L ][i] * ey[i];
		}
	   
		ux /= rho;
		uy /= rho;	   
		U[ x+y*L ] = ux;
		V[ x+y*L ] = uy;
		R[ x+y*L ] = rho;
	   
		ux = ux + fx;

	   	// compute f_tilde (collision step)
	   double feq;	   
	   for(int i=0; i< 9; i++)
	   {		
	      feq =  w[i] * rho * (1.0f - (3.0f/2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[i] * ux + ey[i]*uy) 
				+ (9.0f/2.0f) * (ex[i] * ux + ey[i]*uy) * (ex[i] * ux + ey[i]*uy));
	      df [c][x+y*L][i] =  df [c][x+y*L][i] - (1/tau)* (df[c][x+y*L][i] - feq);
	   }  
	}

  for(int x=0 ; x < L ; x++)
  for(int y=0 ; y < L ; y++)
  if(FLAG[ x+y*L ] == 0)
  {
    
    for(int i=0; i< 9; i++)
    {
      int xp = ( x+ex[i] + L ) % (L);
      int yp = ( y+ey[i] + L ) % (L);
      // streaming
      if( FLAG[xp+yp*L] == 1 )
      df[1-c][ x+y*L ][inv[i]] = df[c][ x+y*L ][i];		
	else
      df[1-c][ xp+yp*L][i] = df[c][ x+y*L ][i];					
    }
    
  } */

