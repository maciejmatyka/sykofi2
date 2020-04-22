

//
//
//  SIMPLER N-S Incompressible Fluid Solver
//  --------------------------------------
//
//  author: Maciej Matyka
//
//  contact: maq@panoramix.ift.uni.wroc.pl
//           http://panoramix.ift.uni.wroc.pl/~maq
//
//  PDF Paper about this method available on home page of the author.








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

void ApplyBoundaryConditions(double **, double **, double **);
void SolveUs(double **, double **, double **, double **, double, double, int, int, int, int, double);
void SolveVs(double **, double **, double **, double **, double, double, int, int, int, int, double);
void SolveUdWithoutP(double **, double **, double **, double, double, int, int, int, int, double);
void SolveVdWithoutP(double **, double **, double **, double, double, int, int, int, int, double);
void IterativePp(double **, double **, double **, double , double , int , int , int , int , double);
void CalculateP(double **, double **, double **, int , int , int , int , double);

void SolveU(double **, double **,  double **, double **, double, double, int, int, int, int, double);
void SolveV(double **, double **,  double **, double **, double, double, int, int, int, int, double);

void dump(char *, double **, int, int, int, int);
void dump2(char *file, double **u,double **v, int sx, int sy, int nx, int ny);
void dumpFlowStudio(char *file, double **u,double **v, int sx, int sy, int nx, int ny);


double SolveUV(double **u, double **v, double **us, double **vs, 
			   double **pp, double dx, double dy, 
			   int sx, int sy, int nx, int ny, double dt);


// definitions
// and
// constans

//#define KMAX 17850

//#define COVMAX 0.00000001




#define EPS_PP 0.001			// covergence value for pressure iteration (relaxation)
#define COVMAX 0.00015			// for Psi iteration proc.


#define NX 40
#define NY 40

//#define saveppm

#define DX (1.0f/(double)NX)
#define DY (1.0f/(double)NY)

#define DT 0.01f				// arbitrary choose

#define KMAX 4200


// physics

#define Ue 1.0f			// lid velocity

#define Re 400			// Reynolds number




// main program

int main(void)
{
	double **U=0; //[NX+1][NY]={0};
	double **V=0; //[NX][NY+1]={0};
	double **Pp=0;	// P^'
	double **Ps=0;	// P^*
	double **Us=0;  //U*
	double **Vs=0;  //V*
	double **Ud=0;  //U^
	double **Vd=0;  //V^
	double **P=0; //[NX][NY]={0};
	
	double cov;
	
	int savedframe;
	
	int i,j,k;
	
	
	
	AllocTableD(&P,NX,NY);
	AllocTableD(&Pp,NX,NY);
	AllocTableD(&Ps,NX,NY);
	
	AllocTableD(&U,NX-1,NY);
	AllocTableD(&Us,NX-1,NY);
	AllocTableD(&Ud,NX-1,NY);
	
	AllocTableD(&Vs,NX,NY-1);
	AllocTableD(&V,NX,NY-1);
	AllocTableD(&Vd,NX,NY-1);
	
	
	
	
	
#define scale 8
#define w (NX)*scale
#define h (NY)*scale
	
	V_OpenView(w,h);
	
	
	ApplyBoundaryConditions(Pp,U,V);	
	ApplyBoundaryConditions(Pp,Us,Vs);	
	
	k=1;
	


//	for(k=1;k<KMAX;k++)
	for(k=1;;k++)
	{
//		cout << k << "...";
		
		
		
		// step1
		// Revised - Us means U^^
		SolveUdWithoutP(Ud,U,V,DX,DY,1,1,NX-2,NY-1,DT);
		SolveVdWithoutP(Vd,V,U,DX,DY,1,1,NX-1,NY-2,DT);


		// step2
		IterativePp(P,Ud,Vd,DX,DY,1,1,NX-1,NY-1,DT);



	//	ApplyBoundaryConditions(P,Us,Vs);	

		// step3
		SolveUs(Us,U,Vd,P,DX,DY,1,1,NX-2,NY-1,DT);
		SolveVs(Vs,V,Ud,P,DX,DY,1,1,NX-1,NY-2,DT);


		// step4
		IterativePp(Pp,Us,Vs,DX,DY,1,1,NX-1,NY-1,DT);

		
		ApplyBoundaryConditions(Pp,Us,Vs);	

		cov = SolveUV(U,V,Us,Vs,Pp,DX,DY,1,1,NX-1,NY-1,DT);
		if(cov<COVMAX)
			break;


	for(i=0;i<NX;i++)
		for(j=0;j<NY-1;j++)
		Vs[i][j]=Vd[i][j]=V[i][j];
	for(i=0;i<NX-1;i++)
		for(j=0;j<NY;j++)
		Us[i][j]=Ud[i][j]=U[i][j];


#ifdef saveppm			
		if(!(k%100))
		{
			V_ClearRect(1,1,w-1,h-1,0);
			PlotStreamlinesSmear(NX,NY,NX,NY,DX,DY,U,V,P,scale);
			savedframe=V_Write(k);
		}
#endif
//		cout << ".. done...";
//		cout << "frame: " << savedframe << " cov: " << cov <<  endl;
			if(!(k%20))
		cout << k << " " << cov <<  endl;
		//k++;
		
	}
	
	
	V_ClearRect(1,1,w-1,h-1,0);
	PlotStreamlinesSmear(NX,NY,NX,NY,DX,DY,U,V,P,scale);
	savedframe=V_Write(k);
	
/*	V_ClearRect(1,1,w-1,h-1,0);
	PlotStreamlinesRandom(NX,NY,NX,NY,DX,DY,U,V,P,scale);
	savedframe=V_Write(k);
	
	
	//checkonly
	V_ClearRect(1,1,w-1,h-1);
	PlotThreeFieldsSmooth(NX,NY,NX,NY,DX,DY,U,V,P,scale);
	PlotVelocities(NX,NY,NX,NY,DX,DY,U,V,scale);
	savedframe=V_Write(k);
	
*/	
	
	return 0;
}




double SolveUV(double **u, double **v, double **us, double **vs, 
			   double **pp, double dx, double dy, 
			   int sx, int sy, int nx, int ny, double dt)
{
	
	
	
	int i,j;	
	double cov;
	
	cov=0;
	double	uvold;
	
	for(i=sx;i<nx;i++)
		for(j=sy;j<ny-1;j++)
		{
			uvold=v[i][j];
			v[i][j] = vs[i][j] - (dt/dy) * (pp[i][j+1] - pp[i][j]);
			vs[i][j]=v[i][j];
			cov=max(cov,fabs(v[i][j]-uvold));
		}
		
		
		for(i=sx;i<nx-1;i++)
			for(j=sy;j<ny;j++)
			{		
				uvold=u[i][j];
				u[i][j] = us[i][j] - (dt/dx) * (pp[i+1][j] - pp[i][j]);
				us[i][j]=u[i][j];
				cov=max(cov,fabs(u[i][j]-uvold));
			}
			
			
			return cov;
}






void IterativePp(double **pp, double **us, double **vs, double dx, double dy, int sx, int sy, int nx, int ny, double dt)
{
	double a,b,c,d;
	int i,j,k;
	double covmax;			// maximum change on a grid
	double last_pp;
	
	a=2*( dt/pow(dx,2) + dt/pow(dy,2) );
	b=-dt/pow(dx,2);
	c=-dt/pow(dy,2);
	
	
	// @todo: maybe test the covergence...
	
	//for(k=0;k<KMAX;k++)				// iteration steps
	do{
		covmax=0;
		
		covmax=0;
		for(j=ny-1;j>=sy;j--)
			for(i=sx;i<nx;i++)
			{
				d=(1.0f/dx)*(us[i][j] - us[i-1][j]) + (1.0f/dy)*( vs[i][j] - vs[i][j-1]);
				last_pp=pp[i][j];
				pp[i][j] = -(1.0f/a)*( b*pp[i+1][j] + b*pp[i-1][j] + c*pp[i][j+1] + c*pp[i][j-1] + d);
				covmax=max(fabs(pp[i][j] - last_pp),covmax);
			}
			
			
	}while(covmax>EPS_PP);
	
	
}

void SolveVdWithoutP(double **vs, double **v, double **u, double dx, double dy, 
			 int sx, int sy, int nx, int ny, double dt)
{
	int i,j;		
	double Bs;	//B*
	double Ud;	// u^-
	double Udd;	// u^{--}
	
	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++)
		{
			//!!
			Udd = 0.5 * (u[i-1][j] + u[i-1][j+1]);
			Ud  = 0.5 * (u[i][j] + u[i][j+1]);
			
			Bs = -( (v[i+1][j]*Ud - v[i-1][j]*Udd)/(2.0f*dx) + (pow(v[i][j+1],2) - pow(v[i][j-1],2))/(2.0f*dy)  );
			Bs = Bs + (1.0f/Re) * ( (v[i+1][j] - 2.0f*v[i][j] + v[i-1][j])/pow(dx,2) + 
				(v[i][j+1] - 2.0f*v[i][j] + v[i][j-1])/pow(dy,2) ); 
			
			vs[i][j] = v[i][j] + dt*Bs;// - (1.0f/dy) * (ps[i][j+1] - ps[i][j]));
		}
}
void SolveVs(double **vs, double **v, double **u, double **ps, double dx, double dy, 
			 int sx, int sy, int nx, int ny, double dt)
{
	int i,j;		
	double Bs;	//B*
	double Ud;	// u^-
	double Udd;	// u^{--}
	
	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++)
		{
			//!!
			Udd = 0.5 * (u[i-1][j] + u[i-1][j+1]);
			Ud  = 0.5 * (u[i][j] + u[i][j+1]);
			
			Bs = -( (v[i+1][j]*Ud - v[i-1][j]*Udd)/(2.0f*dx) + (pow(v[i][j+1],2) - pow(v[i][j-1],2))/(2.0f*dy)  );
			Bs = Bs + (1.0f/Re) * ( (v[i+1][j] - 2.0f*v[i][j] + v[i-1][j])/pow(dx,2) + 
				(v[i][j+1] - 2.0f*v[i][j] + v[i][j-1])/pow(dy,2) ); 
			
			vs[i][j] = vs[i][j] + dt*(Bs - (1.0f/dy) * (ps[i][j+1] - ps[i][j]));
		}
}




void SolveUs(double **us, double **u, double **v, double **ps, double dx, double dy, 
			 int sx, int sy, int nx, int ny, double dt)
{
	int i,j;
	
	// divide by rho at the end!
	
	double As;	//A*
	double Vd;	// v^-
	double Vdd;	// v^{--}
	
	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++)
		{
			
			Vd  = 0.5 * (v[i][j] + v[i+1][j]);
			Vdd = 0.5 * (v[i][j-1] + v[i+1][j-1]);
			As  = -( ( u[i][j+1]*Vd - u[i][j-1]*Vdd)/(2.0f*dy) + (pow(u[i+1][j],2) - pow(u[i-1][j],2))/(2.0f*dx) );
			As = As + (1.0f/Re)*( (u[i+1][j] - 2.0f*u[i][j] + u[i-1][j])/pow(dx,2) + (u[i][j+1] - 2.0f*u[i][j] + u[i][j-1])/pow(dy,2));
			
			us[i][j] = us[i][j] + dt*(As - (1.0f/dx)*(ps[i+1][j] - ps[i][j]));
		}
}
void SolveUdWithoutP(double **us, double **u, double **v, double dx, double dy, 
			 int sx, int sy, int nx, int ny, double dt)
{
	int i,j;
	
	// divide by rho at the end!
	
	double As;	//A*
	double Vd;	// v^-
	double Vdd;	// v^{--}
	
	for(j=sy;j<ny;j++)
		for(i=sx;i<nx;i++)
		{
			
			Vd  = 0.5 * (v[i][j] + v[i+1][j]);
			Vdd = 0.5 * (v[i][j-1] + v[i+1][j-1]);
			As  = -( ( u[i][j+1]*Vd - u[i][j-1]*Vdd)/(2.0f*dy) + (pow(u[i+1][j],2) - pow(u[i-1][j],2))/(2.0f*dx) );
			As = As + (1.0f/Re)*( (u[i+1][j] - 2.0f*u[i][j] + u[i-1][j])/pow(dx,2) + (u[i][j+1] - 2.0f*u[i][j] + u[i][j-1])/pow(dy,2));
			
			us[i][j] = u[i][j] + dt*As;//  - (1.0f/dx)*(ps[i+1][j] - ps[i][j]));
		}
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



// maybe use fillout procedure?
void ApplyBoundaryConditions(double **p, double **u, double **v)
{
	int i,j;
	double a,b;
	
	
	// p
	
	
	//-----------------------------------------
	
	for(i=0;i<=NX-1;i++){
		p[i][0]=p[i][1];
		p[i][NY-1]=p[i][NY-2];
	}
	
	for(j=0;j<=NY-1;j++){
		p[0][j]=p[1][j];
		p[NX-1][j]=p[NX-2][j];
	}
	p[0][0]=p[1][1];
	p[0][NY-1]=p[1][NY-2];
	p[NX-1][NY-1]=p[NX-2][NY-2];
	p[NX-1][0]=p[NX-2][1];
	
	//-----------------------------------------
	
	// v
	
	
#define LINEARCOR -(2.0f/3.0f)
	
	for(i=0;i<=NX-2;i++){
		v[i][0]=LINEARCOR * v[i][1];
		v[i][NY-2]=LINEARCOR * v[i][NY-3];
	}
	
	for(j=0;j<=NY-2;j++){					// 1 left
		v[0][j] = 0;
		v[NX-1][j] = 0;
	}
	
	//-----------------------------------------
	
	//u
	for(j=0;j<=NY-2;j++){
		u[0][j] = LINEARCOR * u[1][j];
		u[NX-2][j] = LINEARCOR * u[NX-3][j];
	}
	
	
	for(i=0;i<=NX-2;i++){
		u[i][0]=Ue;
		u[i][NY-1]=0;
		
	}
	//-----------------------------------------

}