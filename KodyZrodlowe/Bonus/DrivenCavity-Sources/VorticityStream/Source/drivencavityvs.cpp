

//
//
//  VORTICITY-STREAM N-S Incompressible Fluid Solver
//  -------------------------------------------------
//
//  author: Maciej Matyka
//
//  contact: maq@panoramix.ift.uni.wroc.pl
//           http://panoramix.ift.uni.wroc.pl/~maq
//
//  PDF Paper about this method available on home page of the author.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "view.h"
#include "visual.h"
#include "minmax.h"
using namespace std;

// fuctions

void AllocTableD(double ***, int nx, int ny);
void SolveVorticityTransport(double **ksi,double **ksitemp, double **u,double **v,
							 double dx,double dy,double dt, int sx, int sy, int nx, int ny);
void IteratePsi(double **psi, double **ksi, double dx, double dy,
				int sx, int sy, int nx, int ny);
double SolveUV(double **psi, double **u, double **v, double dx, double dy,
			 int sx, int sy , int nx, int ny);
void BoundaryConditions(double **psi, double **ksi, double **ksitemp, double dx, double dy,
					 int sx, int sy , int nx, int ny);

// constans

#define NX 40
#define NY 40

//#define saveppm

#define DX (1.0f/(double)NX)
#define DY (1.0f/(double)NY)

#define DT 0.003f				// arbitrary choose

#define KMAX 4200
#define COVMAX 0.00005			// for Psi iteration proc.


#define EPS_PP 0.000001			// covergence value for pressure iteration (relaxation)
 
//#define saveppm

// physics

#define Re 400			// Reynolds number

// main program

int main(void)
{
	double **Psi,**Ksi,**KsiTemp,**U,**V;
	int k;
	int i,j;
	double cov;

	AllocTableD(&Psi,NX,NY);
	AllocTableD(&Ksi,NX,NY);
	AllocTableD(&KsiTemp,NX,NY);
	AllocTableD(&U,NX,NY);
	AllocTableD(&V,NX,NY);

#define scale 10
#define w (NX)*scale
#define h (NY)*scale

		V_OpenView(w,h);


	
//	for(k=1;k<KMAX;k++)
//	for(k=1;k<KMAX;k++)
	for(k=1;;k++)
	{

		SolveVorticityTransport(Ksi,KsiTemp,U,V,DX,DY,DT,1,1,NX-1,NY-1);
		IteratePsi(Psi,Ksi,DX,DY,1,1,NX-1,NY-1);
		BoundaryConditions(Psi,Ksi,KsiTemp,DX,DY,0,0,NX,NY);
		cov=SolveUV(Psi,U,V,DX,DY,1,1,NX-1,NY-1);
		
		if(k>5)			// needed because of 0 cov at the beginning
		if(cov<COVMAX)
			break;

#ifdef saveppm
		if(!(k%100))
		{
			V_ClearRect(1,1,w-1,h-1,0);
	//		PlotVelocities(NX,NY,NX,NY,DX,DY,U,V,scale);
			PlotStreamlinesSmear(NX,NY,NX,NY,DX,DY,U,V,V,scale);
			V_Write(k);

	//		cout << k/10 << endl;
		}
#endif

	if(!(k%2))
		cout << k << " " << cov <<  endl;
	}


			V_ClearRect(1,1,w-1,h-1,0);
			PlotStreamlinesSmear(NX,NY,NX,NY,DX,DY,U,V,V,scale);
			V_Write(k);

			
//0			
			V_ClearRect(1,1,w-1,h-1,0);
			PlotThreeFieldsSmooth(NX,NY,NX,NY,DX,DY,U,V,Psi,scale);
			V_Write(k);
//1
			V_ClearRect(1,1,w-1,h-1,0);
			PlotVelocities(NX,NY,NX,NY,DX,DY,U,V,scale);
			V_Write(k);
//2
			V_ClearRect(1,1,w-1,h-1,0);
			PlotThreeFieldsSmooth(NX,NY,NX,NY,DX,DY,U,V,Psi,scale);
			PlotVelocities(NX,NY,NX,NY,DX,DY,U,V,scale);
			V_Write(k);
//3	
			V_ClearRect(1,1,w-1,h-1,0);
			PlotOneSmoothField(NX,NY,NX,NY,DX,DY,Psi,scale);
			V_Write(k);
//4
			V_ClearRect(1,1,w-1,h-1,0);
			PlotOneSmoothField(NX,NY,NX,NY,DX,DY,Ksi,scale);
			V_Write(k);
//5
			V_ClearRect(1,1,w-1,h-1,0);
			PlotOneSmoothField(NX,NY,NX,NY,DX,DY,U,scale);
			V_Write(k);
//6
			V_ClearRect(1,1,w-1,h-1,0);
			PlotOneSmoothField(NX,NY,NX,NY,DX,DY,V,scale);
			V_Write(k);


	cout << "#  x    y    z" << endl << endl;

	for(i=0;i<NX;i++)
	{
	for(j=NY-1;j>=0;j--)
		{
//			cout << i*DX << " " << j*DY << " "<< Psi[i][j] << " " << endl;
			cout << Psi[i][j] << " ";
		}
		cout << endl;
	}


	return 0;
}



void BoundaryConditions(double **psi, double **ksi, double **ksitemp, double dx, double dy,
					 int sx, int sy , int nx, int ny)
{
	int i,j;

	double dx2,dy2;

	dx2=pow(dx,2);
	dy2=pow(dy,2);


#define alpha 0.2

	for(i=sx;i<nx;i++)
	{
		psi[i][0]=0;
		psi[i][ny-1]=0;


		ksitemp[i][0] = 2*(psi[i][0] - psi[i][1] + dy)/dy2;
		ksitemp[i][ny-1] = 2*(psi[i][ny-1] - psi[i][ny-2])/dy2;


		ksi[i][0] = alpha * ksi[i][0] + (1-alpha)*ksitemp[i][0];
		ksi[i][ny-1] = alpha * ksi[i][ny-1] + (1-alpha)*ksitemp[i][ny-1];

	}



	for(j=sy;j<ny;j++)
	{
		psi[0][j]=0;
		psi[nx-1][j]=0;


		ksitemp[0][j] = 2*(psi[0][j] - psi[1][j])/dx2;
		ksitemp[nx-1][j] = 2*(psi[nx-1][j] - psi[nx-2][j])/dx2;


		ksi[nx-1][j] = alpha * ksi[nx-1][j] + (1-alpha)*ksitemp[nx-1][j];
		ksi[0][j] = alpha * ksi[0][j] + (1-alpha)*ksitemp[0][j];

	}

}

double SolveUV(double **psi, double **u, double **v, double dx, double dy,
			 int sx, int sy , int nx, int ny)
{
	int i,j;

	double cov;
	
	cov=0;
	double	uvold;

		for(j=sy;j<ny;j++)
			for(i=sx;i<nx;i++){

				uvold=u[i][j];				
				u[i][j] = (psi[i][j+1] - psi[i][j-1])/(2*dy);
//			cov=max(cov,fabs(u[i][j])/fabs(uvold));
				cov=max(cov,fabs(u[i][j]-uvold));

				
				uvold=v[i][j];
				v[i][j] =-(psi[i+1][j] - psi[i-1][j])/(2*dx);
//			cov=max(cov,fabs(v[i][j])/fabs(uvold));
				cov=max(cov,fabs(v[i][j]-uvold));

			}


	return cov;
}




void IteratePsi(double **psi, double **ksi, double dx, double dy,
				int sx, int sy, int nx, int ny)
{
	int i,j;
	double cov;
	double dx2,dy2;
	double C;
	double psi_bak;

	dx2=pow(dx,2);
	dy2=pow(dy,2);

	C=(dx2*dy2) / (2*dx2 + 2*dy2);



	do{
		cov=0;

		for(j=sy;j<ny;j++)
			for(i=sx;i<nx;i++){

			psi_bak=psi[i][j];
		
			
			psi[i][j] =	C*( 
							(psi[i-1][j] + psi[i+1][j])/dx2 + (psi[i][j-1] + psi[i][j+1])/dy2 
							+ ksi[i][j]
							);
			
			cov=max(fabs(psi[i][j] - psi_bak),cov);
		}


	}while(cov>EPS_PP);


}




void SolveVorticityTransport(double **ksi,double **ksitemp, double **u,double **v,
							 double dx,double dy,double dt, int sx, int sy, int nx, int ny)
{
	double a,b;
	double dx2,dy2;
	int i,j;

	dx2=pow(dx,2);
	dy2=pow(dy,2);


	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++){

			a=		u[i][j] * (ksi[i+1][j] - ksi[i-1][j]) / (2*dx) 
				+	v[i][j] * (ksi[i][j+1] - ksi[i][j-1]) / (2*dy);

			b=	(1.0f/Re)*
				(	(ksi[i+1][j] - 2*ksi[i][j] + ksi[i-1][j]) / dx2	
				+	(ksi[i][j+1] - 2*ksi[i][j] + ksi[i][j-1]) / dy2 );

			ksitemp[i][j] = ksi[i][j] + dt * (b - a);
		}


	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++)
			ksi[i][j] = ksitemp[i][j];
}




void AllocTableD(double ***table, int nx, int ny)
{
	int i;
	int j;

	(*table) = new double*[nx];
	for(j=0;j<nx;j++)
		(*table)[j]=new double[ny];

	for(i=0;i<nx;i++)
	for(j=0;j<ny;j++)
		(*table)[i][j] = 0;

}


