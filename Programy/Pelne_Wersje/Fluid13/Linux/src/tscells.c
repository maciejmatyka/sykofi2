

/*
 timestep procedure
                      tscells.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "particle.h"
#include "bndflags.h"
#include "tscells.h"



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: double SumParticlesU(struct particle *firstparticle, i,j);
		function: double SumParticlesV(struct particle *firstparticle, i,j);

	  it:				Sum velocities (u,v) of particles in Cell i,j. Divine by total
							number of particles in that cell.
		args:			^^^^^
		return:		(double) UK(VK)/NK
    todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

double SumParticlesU(struct particle *firstparticle, int i, int j)
{
    struct particle *wrkpart;
    int N;			/* total number of particles in cell i,j */
    double UK;			/* u velocities of particles in cell i,j */
    int pi,pj;


    N = 0;
    UK = 0.0;

    for (wrkpart = firstparticle; wrkpart != NULL; wrkpart = wrkpart->next) {

	pi = wrkpart->i;
	pj = wrkpart->j;

	if ((pi == i) && (pj == j))
	/*    && (wrkpart->flag != P_AVAIL)) */
	    {
	    N += 1;
	    UK += (wrkpart->uk);
	}
    }

    return (UK / N);
}

double SumParticlesV(struct particle *firstparticle, int i, int j)
{
    struct particle *wrkpart;
    int N;			/* total number of particles in cell i,j */
    double VK;			/* u velocities of particles in cell i,j */
    int pi,pj;


    N = 0;
    VK = 0.0;

    for (wrkpart = firstparticle; wrkpart != NULL; wrkpart = wrkpart->next) {

	pi = wrkpart->i;
	pj = wrkpart->j;

	if ((pi == i) && (pj == j))
	/*    && (wrkpart->flag != P_AVAIL))*/
	     {
	    N += 1;
	    VK += (wrkpart->vk);
	}
    }

    return (VK / N);
}

int SumParticles(struct particle *firstparticle, int i, int j)
{
    struct particle *wrkpart;
    int N;			/* total number of particles in cell i,j */
    int pi,pj;

    N = 0;

    for (wrkpart = firstparticle; wrkpart != NULL; wrkpart = wrkpart->next)
    {
	pi = wrkpart->i;
	pj = wrkpart->j;
    
	if ((pi == i) && (pj == j))
	/*    && (wrkpart->flag != P_AVAIL))*/
	{
	    N += 1;
		printf("%d,%d\n",pi,pj);
	}
    }

    return N;
}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: ReflagTo(int **F, int i, int j, int flag);
	  it:       Remark cell between C_EMP, C_FULL and C_SUR. Need, becouse
							lot of datas is inside Flag table.
		args:     intuicyjnie oczywiste
		return:		none
    todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
void ReflagTo(int **F, int i, int j, int flag)
{

    F[i][j] &= ~C_FULL;
    F[i][j] &= ~C_EMP;
    F[i][j] &= ~C_SUR;
    F[i][j] |= flag;

}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: int AnyParticles(int i, int j, struct particle *firstparticle);
	  it:       Check cell for particles.
		args:			i,j - coords. of cell. struct particle *
		return:   TRUE or FALSE (cell contain particles or not)
    todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
int AnyParticles(int i, int j, struct particle *firstparticle)
{
    struct particle *wrkpart;

    for (wrkpart = firstparticle; wrkpart != NULL; wrkpart = wrkpart->next) {
	if ((wrkpart->i == i) && (wrkpart->j == j)
	    && (wrkpart->flag != P_AVAIL))
	    return 1;
    }

    return 0;
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: void ReflagCells(int** F, double **P, double **U, double **V, int nx, int ny, struct particle *firstparticle)
	  it:				Reflag cells, use particle struct stream.
		args:
		return:		none
    todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void ReflagCells(double **Vn1, int **F, double **P, double **U, double **V,
		 int nx, int ny, struct particle *firstparticle)
{
    int i, j;
    double UKnk, VKnk;		/* UK - sum of u vel. particles in cell
				   nk - number of particles in cell
				   UKnk - UK divined by nk ( UK / nk) */

    double PSum;		/* Sum of pressure in cells */
    int N;			/* Number of cells take use in pressure sum calculation */

    struct particle *wrkpart;



    
/*    N=SumParticles(firstparticle,25,25);
 
 
    printf("particles = %d\n",N);
*/

    for (j = 1; j <= ny; j++)
	for (i = 1; i <= nx; i++)
	    Vn1[i][j] = 0.0;
	
	
    for (wrkpart = firstparticle; wrkpart != NULL; wrkpart = wrkpart->next) 
    {
/*	if (F[wrkpart->i][wrkpart->j] & C_BND)
	    wrkpart->flag = P_AVAIL;
*/
	i = wrkpart->i;
	j = wrkpart->j;

	if(i<nx && j<ny && i> 1 && j> 1)
		Vn1[i][j] = 2.0;	/* mark particle cells */

/*	printf("partin[%d][%d]\n",i,j);*/
    }






    for (j = 1; j <= ny; j++) {

	for (i = 1; i <= nx; i++) {
	    if (F[i][j] & C_SUR) {
		if (Vn1[i][j] == 0) {	/* ! any particles ? */

		    ReflagTo(F, i, j, C_EMP);

		    P[i][j] = 0.0;

		    if (F[i + 1][j] & C_EMP)
			U[i][j] = 0.0;

		    if (F[i - 1][j] & C_EMP)
			U[i - 1][j] = 0.0;

		    if (F[i][j + 1] & C_EMP)
			V[i][j] = 0.0;

		    if (F[i][j - 1] & C_EMP)
			V[i][j - 1] = 0.0;
		}
	    } else {		/* (F[i][j] !& C_SUR) */

		if (F[i][j] & C_EMP) {

		    P[i][j] = 0.0;

		    if (Vn1[i][j] != 0) {	/* any particles ? */
			ReflagTo(F, i, j, C_SUR);

			/* sum u and v values of particles in cell i,j */

			UKnk = SumParticlesU(firstparticle, i, j);
			VKnk = SumParticlesV(firstparticle, i, j);

			if ((F[i + 1][j] & C_EMP) || (F[i + 1][j] & C_OUT))
			    U[i][j] = UKnk;

			if ((F[i - 1][j] & C_EMP) || (F[i - 1][j] & C_OUT))
			    U[i - 1][j] = UKnk;

			if ((F[i][j + 1] & C_EMP) || (F[i][j + 1] & C_OUT))
			    V[i][j] = VKnk;

			if ((F[i][j - 1] & C_EMP) || (F[i][j - 1] & C_OUT))
			    V[i][j - 1] = VKnk;
		    }
		}
	    }

	}			/* end for i */
    }				/* end for j */




    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if (Vn1[i][j] != 0) {	/* any particles ? */
		if (F[i][j] & C_FULL) {
		    if ((F[i + 1][j] & C_EMP) || (F[i - 1][j] & C_EMP)
			|| (F[i][j + 1] & C_EMP) || (F[i][j - 1] & C_EMP)) {
/*						printf("sur!");*/
			ReflagTo(F, i, j, C_SUR);
			P[i][j] = 0;
		    }
		} else {
		    P[i][j] = 0;

		    if (F[i][j] & C_SUR) {
			PSum = 0;
			N = 0;

			if (!(F[i + 1][j] & C_EMP)) {
			    if (F[i + 1][j] & C_FULL) {
				PSum += P[i + 1][j];
				N += 1;
			    }
			
			
			if (!(F[i - 1][j] & C_EMP)) {
			    if (F[i - 1][j] & C_FULL) {
			        PSum += P[i - 1][j];
			        N += 1;
				}
			
			
			if (!(F[i][j + 1] & C_EMP)) {
			    if (F[i][j + 1] & C_FULL) {
				PSum += P[i][j + 1];
				N += 1;
				}
			
			
			if (!(F[i][j - 1] & C_EMP)) {
			    if (F[i][j - 1] & C_FULL) {
			        PSum += P[i][j - 1];
			        N += 1;
				}
			
			
			
				ReflagTo(F, i, j, C_FULL);

			if(PSum != 0)
				P[i][j] = (PSum / N);
			else
				P[i][j] = 0.0;


			}}}}
			
			
		    }
		}


	    }
	    /* no particles */
	}			/* end for i */
    }				/* end for j */



}

/* Surface conditions taken from any deutsch book - easy to expand for 3d */

void CalculateSurface(double **Un1, double **Vn1, int** F, double **P, double **D, double **U, double **V, int nx, int ny, double gx, double gy, double dt, double dx, double dy)
{
	int i,j;


/*	return;*/


/*	gx=0;
	gy=0;
*/
	for(j=1;j<=ny;j++)
		for(i=1;i<=nx;i++)
			F[i][j] &= ~S_NSWE;


       if ((F[i][j] & C_EMP) && F[i+1][j] & C_EMP)
          U[i][j] = 0.0;

       if ((F[i][j] & C_EMP) && F[i][j+1] & C_EMP)
          V[i][j] = 0.0;


	for(j=1;j<=ny;j++)
  {
		for(i=1;i<=nx;i++)
		{

/*						D[i][j] = (U[i][j] - U[i-1][j])/dx + (V[i][j] - V[i][j-1])/dy;*/

				if(F[i][j] & C_SUR)
				{

					F[i][j] &= ~S_NSWE;

          if (F[i-1][j] & C_EMP)
             F[i][j] = F[i][j] | S_W;
          if (F[i+1][j] & C_EMP)
             F[i][j] = F[i][j] | S_E;
          if (F[i][j-1] & C_EMP)
             F[i][j] = F[i][j] | S_S;
          if (F[i][j+1] & C_EMP)
             F[i][j] = F[i][j] | S_N;



switch (F[i][j] & S_NSWE)
{

case S_N   :{ V[i][j] = V[i][j-1]-dy/dx*(U[i][j]-U[i-1][j]); if (F[i-1][j+1] & C_EMP)
U[i-1][j+1] = U[i-1][j]-dy/dx*(V[i][j]-V[i-1][j]);
} break; case S_S   :{ V[i][j-1] = V[i][j]+dy/dx*(U[i][j]-U[i-1][j]); if (F[i-1][j-1] & C_EMP)
U[i-1][j-1] =
U[i-1][j]+dy/dx*(V[i][j-1]-V[i-1][j-1]);
} break; case S_E   :{ U[i][j] = U[i-1][j]-dx/dy*(V[i][j]-V[i][j-1]); if (F[i+1][j-1] & C_EMP)
V[i+1][j-1] =
V[i][j-1]-dx/dy*(U[i][j]-U[i][j-1]);
} break;
case S_W :{ U[i-1][j] = U[i][j]+dx/dy*(V[i][j]-V[i][j-1]); if (F[i-1][j-1] & C_EMP)
V[i-1][j-1] =
V[i][j-1]+dx/dy*(U[i-1][j]-U[i-1][j-1]);
} break;
case S_NE  :{ U[i][j]     = U[i-1][j];
V[i][j]     = V[i][j-1];
if (F[i-1][j+1] & C_EMP)
U[i-1][j+1] =
U[i-1][j]-dy/dx*(V[i][j]-V[i-1][j]);
if (F[i+1][j+1] & C_EMP)
{
U[i][j+1]   = U[i][j];
V[i+1][j]   = V[i][j];
}
if (F[i+1][j-1] & C_EMP)
V[i+1][j-1] =
V[i][j-1]-dx/dy*(U[i][j]-U[i][j-1]);
} break;
case S_NW  :{ U[i-1][j]   = U[i][j];
V[i][j]     = V[i][j-1];
if (F[i-1][j+1] & C_EMP)
{
U[i-1][j+1] = U[i-1][j];
V[i-1][j]   = V[i][j];
}
if (F[i-1][j-1] & C_EMP)
V[i-1][j-1] =
V[i][j-1]+dx/dy*(U[i-1][j]-U[i-1][j-1]);
} break;
case S_SW  :{ U[i-1][j]   = U[i][j];
V[i][j-1]   = V[i][j];
if (F[i-1][j-1] & C_EMP)
{
U[i-1][j-1] = U[i-1][j];
V[i-1][j-1] = V[i][j-1];
}
} break;
case S_SE  :{ U[i][j]     = U[i-1][j];
V[i][j-1]   = V[i][j];
if (F[i-1][j-1] & C_EMP)
U[i-1][j-1] =
U[i-1][j]+dy/dx*(V[i][j-1]-V[i-1][j-1]); if (F[i+1][j-1] & C_EMP)
{
U[i][j-1]   = U[i][j];
V[i+1][j-1] = V[i][j-1];
}
} break;
case S_WE  :{ U[i][j]     += dt*gx;
U[i-1][j]   += dt*gx;
if (F[i-1][j-1] & C_EMP)
V[i-1][j-1]  =
V[i][j-1]+dx/dy*(U[i-1][j]-U[i-1][j-1]); if (F[i+1][j-1] & C_EMP)
V[i+1][j-1]  =
V[i][j-1]-dx/dy*(U[i][j]-U[i][j-1]);
} break;
case S_NS  :{ V[i][j]     += dt*gy;
V[i][j-1]   += dt*gy;
if (F[i-1][j+1] & C_EMP)
U[i-1][j+1]  =
U[i-1][j]-dy/dx*(V[i][j]-V[i-1][j]);
if (F[i-1][j-1] & C_EMP)
U[i-1][j-1]  =
U[i-1][j]+dy/dx*(V[i][j-1]-V[i-1][j-1]);
} break;

case S_NWE :{ V[i][j]      = V[i][j-1]-dy/dx*(U[i][j]-U[i-1][j]);
U[i][j]     += dt*gx;
U[i-1][j]   += dt*gx;
if (F[i-1][j-1] & C_EMP)
V[i-1][j-1]  =
V[i][j-1]+dx/dy*(U[i-1][j]-U[i-1][j-1]); if (F[i+1][j-1] & C_EMP)
V[i+1][j-1]  =
V[i][j-1]-dx/dy*(U[i][j]-U[i][j-1]);
if (F[i-1][j+1] & C_EMP)
{
V[i-1][j]    = V[i][j];
U[i-1][j+1]  = U[i-1][j];
}
if (F[i+1][j+1] & C_EMP)
{
V[i+1][j]    = V[i][j];
U[i][j+1]    = U[i][j];
}
} break;
case S_NSW :{ U[i-1][j]    = U[i][j]+dx/dy*(V[i][j]-V[i][j-1]);
V[i][j]     += dt*gy;
V[i][j-1]   += dt*gy;
if (F[i-1][j-1] & C_EMP)
{
V[i-1][j-1]  = V[i][j-1];
U[i-1][j-1]  = U[i-1][j];
}
if (F[i-1][j+1] & C_EMP)
{
V[i-1][j]    = V[i][j];
U[i-1][j+1]  = U[i-1][j];
}
} break;
case S_SWE :{ V[i][j-1]    = V[i][j]+dy/dx*(U[i][j]-U[i-1][j]);
U[i][j]     += dt*gx;
U[i-1][j]   += dt*gx;
if (F[i-1][j-1] & C_EMP)
{
U[i-1][j-1]  = U[i-1][j];
V[i-1][j-1]  = V[i][j-1];
}
if (F[i+1][j-1] & C_EMP)
{
U[i][j-1]    = U[i][j];
V[i+1][j-1]  = V[i][j-1];
}
} break;
case S_NSE :{ U[i][j]      = U[i-1][j]-dx/dy*(V[i][j]-V[i][j-1]);
V[i][j]     += dt*gy;
V[i-1][j]   += dt*gy;
if (F[i-1][j+1] & C_EMP)
U[i-1][j+1]  =
U[i-1][j]-dy/dx*(V[i][j]-V[i-1][j]);
if (F[i-1][j-1] & C_EMP)
U[i-1][j-1]  =
U[i-1][j]+dy/dx*(V[i][j-1]-V[i-1][j-1]); if (F[i+1][j-1] & C_EMP)
{
U[i][j-1]    = U[i][j];
V[i+1][j-1]  = V[i][j-1];
}
if (F[i+1][j+1] & C_EMP)
{
U[i][j+1]    = U[i][j];
V[i+1][j]    = V[i][j];
}
} break;
case S_NSWE:{ U[i][j]     += dt*gx;
U[i-1][j]   += dt*gx;
V[i][j]     += dt*gy;
V[i][j-1]   += dt*gy;
if (F[i-1][j+1] & C_EMP)
{
U[i-1][j+1]  = U[i-1][j];
V[i-1][j]    = V[i][j];
}
if (F[i+1][j+1] & C_EMP)
{
U[i][j+1]    = U[i][j];
V[i+1][j]    = V[i][j];
}
if (F[i-1][j-1] & C_EMP)
{
U[i-1][j-1]  = U[i-1][j];
V[i-1][j-1]  = V[i][j-1];
}
if (F[i+1][j-1] & C_EMP)
{
U[i][j-1]    = U[i][j];
V[i+1][j-1]  = V[i][j-1];
}
} break;
default     : break;
}


/*          printf("U[%d][%d]=%f , V[%d][%d]=%f \n",i,j,U[i][j],i,j,V[i][j]);*/

				}

	   }	/* end for i */
	}		/* end for j */






}



