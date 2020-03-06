
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bndflags.h"
#include "view.h"

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: void CalculateDij(int **F, int nx, int ny);
	  it:				Prepare table for next pressure calc. functions.
		args:     blah
		return:   none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void CalculateDij(int **F, double **D, double **U, double **V, double dx,
		  double dy, int nx, int ny)
{
    int i, j;
/*	int res;*/
     /**/
/*	res=1;*/
	for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if (!(F[i][j] & C_BND))
		if (!(F[i][j] & C_EMP))
		    D[i][j] = (U[i][j] - U[i-1][j]) / dx
			+ (V[i][j] - V[i][j-1]) / dy;

/*				if( fabs(D[i][j]) > (3.5 * 0.001))*/
/*        {*/
/*						printf("D[%d][%d] too large.\n",i,j);*/
/*						res=0;*/
/*						break;*/
/*				}*/

	}			/* end for i */
    }				/* end for j */

/*  return res;*/

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void	PressureSource(int **F, double **R, int nx, int ny, double dx, double dy,);
	  it:
							Calculate Rij - init for pressure calculation.
		args:			see...
		return: 	none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

/* Rij */

void PressureSource(int **F, double **U, double **V, double **D,
		    double **R, int nx, int ny, double dx, double dy,
		    double vis, double dt)
{
    double SumR, Sum1, Sum2, Sum3, Sum4, Sum5;
    int i, j;

    SumR = 0;


    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {


	    if (F[i][j] & C_FULL) {

		Sum1 = Sum2 = Sum3 = Sum4 = Sum5 = 0;


		if (F[i + 1][j] & C_COR) {
		    if (F[i + 1][j + 1] & C_OB)
			Sum2 -= U[i][j + 1] / dx;
		    else
			Sum2 -= U[i][j - 1] / dx;
		}

		if (F[i - 1][j] & C_COR) {
		    if (F[i - 1][j + 1] & C_OB)
			Sum2 += U[i - 1][j + 1] / dx;
		    else
			Sum2 += U[i - 1][j - 1] / dx;
		}

		if (F[i][j + 1] & C_COR) {
		    if (F[i + 1][j + 1] & C_OB)
			Sum1 -= V[i + 1][j] / dy;
		    else
			Sum1 -= V[i - 1][j] / dy;
		}

		if (F[i][j - 1] & C_COR) {
		    if (F[i + 1][j - 1] & C_OB)
			Sum1 += V[i + 1][j - 1] / dy;
		    else
			Sum1 += V[i - 1][j - 1] / dy;
		}





		if (!(F[i][j] & C_OB)) {

		    if (F[i + 1][j - 1] & C_COR) {
			if (F[i + 1][j - 1] & C_FRSLP) {
			    Sum2 += U[i][j] / dx;
			    Sum1 -= V[i][j - 1] / dy;
			} else {
			    Sum2 -= U[i][j] / dx;
			    Sum1 += V[i][j - 1] / dy;
			}
		    }


		    if (F[i - 1][j - 1] & C_COR) {
			if (F[i - 1][j - 1] & C_FRSLP) {
			    Sum2 -= U[i - 1][j] / dx;
			    Sum1 -= V[i][j - 1] / dy;
			} else {
			    Sum2 += U[i - 1][j] / dx;
			    Sum1 += V[i][j - 1] / dy;
			}
		    }


		    if (F[i + 1][j + 1] & C_COR) {
			if (F[i + 1][j + 1] & C_FRSLP) {
			    Sum2 += U[i][j] / dx;
			    Sum1 += V[i][j] / dy;
			} else {
			    Sum2 -= U[i][j] / dx;
			    Sum1 -= V[i][j] / dy;
			}
		    }


		    if (F[i - 1][j + 1] & C_COR) {
			if (F[i - 1][j + 1] & C_FRSLP) {
			    Sum2 -= U[i - 1][j] / dx;
			    Sum1 += V[i][j] / dy;
			} else {
			    Sum2 += U[i - 1][j] / dx;
			    Sum1 -= V[i][j] / dy;
			}
		    }
		}

		/* i,j ~C_OB */
		if (!(F[i + 1][j] & C_BND)) {
		    Sum1 += (D[i + 1][j] - D[i][j]);
		    Sum3 +=
			(((U[i][j] + U[i + 1][j]) / 2) *
			 ((U[i][j] + U[i + 1][j]) / 2) -
			 ((U[i - 1][j] + U[i][j]) / 2) *
			 ((U[i - 1][j] + U[i][j]) / 2));
		}

		if (!(F[i - 1][j] & C_BND)) {
		    Sum1 += (D[i - 1][j] - D[i][j]);
		    Sum3 +=
			(((U[i - 1][j] + U[i - 2][j]) / 2) *
			 ((U[i - 1][j] + U[i - 2][j]) / 2) -
			 ((U[i - 1][j] + U[i][j]) / 2) *
			 ((U[i - 1][j] + U[i][j]) / 2));
		}

		Sum1 /= (dx * dx);
		Sum3 /= (dx * dx);


		if (!(F[i][j + 1] & C_BND)) {
		    Sum2 += (D[i][j + 1] - D[i][j]);
		    Sum4 +=
			(((V[i][j] + V[i][j + 1]) / 2) *
			 ((V[i][j] + V[i][j + 1]) / 2) -
			 ((V[i][j - 1] + V[i][j]) / 2) *
			 ((V[i][j - 1] + V[i][j]) / 2));
		}

		if (!(F[i][j - 1] & C_BND)) {
		    Sum2 += (D[i][j - 1] - D[i][j]);
		    Sum4 +=
			(((V[i][j - 1] + V[i][j - 2]) / 2) *
			 ((V[i][j - 1] + V[i][j - 2]) / 2) -
			 ((V[i][j - 1] + V[i][j]) / 2) *
			 ((V[i][j - 1] + V[i][j]) / 2));
		}

		Sum2 /= (dy * dy);
		Sum4 /= (dy * dy);


		Sum1 = (-1.0*vis) * (Sum1 + Sum2);
		Sum2 = Sum3 + Sum4;

		if (!(F[i][j] & C_URON))
		    Sum5 =
			((U[i][j] + U[i][j + 1]) *
			 (V[i][j] + V[i + 1][j]));
		if (!(F[i - 1][j - 1] & C_URON))
		    Sum5 +=
			((U[i - 1][j] + U[i - 1][j - 1]) *
			 (V[i][j - 1] + V[i - 1][j - 1]));
		if (!(F[i][j - 1] & C_URON))
		    Sum5 -=
			((U[i][j] + U[i][j - 1]) *
			 (V[i][j - 1] + V[i + 1][j - 1]));
		if (!(F[i - 1][j] & C_URON))
		    Sum5 -=
			((U[i - 1][j] + U[i - 1][j + 1]) *
			 (V[i - 1][j] + V[i][j]));

		R[i][j] =
		    ((Sum1 + Sum2 + Sum5 / (2 * dx * dy)) -
		     (D[i][j] / dt));
		SumR += R[i][j];


	    }
	    /* Cell not C_FULL */
	}			/* end for i */
    }				/* end for j */


}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
							void	CalculatePressure(int **F, double **R, double **P, int nx, int ny, double dx, double dy, double gx, double gy, double vis,  double **U, double **V);
	  it:
							Calculate Pressure for every cell.
		args:			see...
		return: 	none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


int CalculatePressure(int **F, double **R, double **P, int nx, int ny,
		      double dx, double dy, double gx, double gy,
		      double vis, double **U, double **V)
{
    int i, j;
	/*, iterations;	,iterationsCON; */
    int itmax;			/* max iterations num */
    double CONmax, CONij;
    double Z;
    double POld;
    double h, L;
/*	char blah[100];*/

/*
    gy=-gy;
*/
    itmax = 0;

    h = 10;			/* max height of fluid */
    L = 10;			/* max lenght of fluid */

/*	iterationsCON = 0;*/
    CONmax = 0.0;

    Z = 2 * (1 / (dx * dx) + 1 / (dy * dy));


/*	CursDown();*/

    do {
/*		iterationsCON = 0;*/
	CONmax = 0.0;

/*		for(iterations=0;iterations<10;iterations++)*/
/*		{*/

 	for (i = 1; i <= nx; i++) {
   for (j = 1; j <= ny; j++) {

		if (F[i][j] & C_FULL || F[i][j] & C_SUR) {
		    if (F[i][j] & C_OB) {


			/* SKIPPED: Calculations for out cell event */


			if (F[i][j + 1] & C_BND) {
			    if ((F[i][j + 1] & C_NOSLP)
				|| (F[i][j + 1] & C_IN)) {
				P[i][j + 1] =
				    P[i][j] + gy * dy +
				    ((2 * vis) / dy) * (V[i][j - 1] -
							V[i][j]);

				if (F[i][j + 1] & C_INCOR)
				    P[i][j + 1] -=
					((dy * vis) / (dx * dx)) *
					(V[i + 1][j] - V[i - 1][j]);
			    } else {
				if (F[i][j + 1] & C_FRSLP)
				    P[i][j + 1] = P[i][j] + gy * dy;
			    }
			}

			if (F[i][j - 1] & C_BND) {
			    if ((F[i][j - 1] & C_NOSLP)
				|| (F[i][j - 1] & C_IN)) {
				P[i][j - 1] =
				    P[i][j] - gy * dy -
				    ((2 * vis) / dy) * (V[i][j] -
							V[i][j - 1]);

				if (F[i][j - 1] & C_INCOR)
				    P[i][j - 1] +=
					((dy * vis) / (dx * dx)) *
					(V[i + 1][j - 1] -
					 V[i - 1][j - 1]);
			    } else {
				if (F[i][j - 1] & C_FRSLP)
				    P[i][j - 1] = P[i][j] - gy * dy;
			    }
			}

			if (F[i + 1][j] & C_BND) {
			    if ((F[i + 1][j] & C_NOSLP)
				|| (F[i + 1][j] & C_IN)) {
				P[i + 1][j] =
				    P[i][j] + gx * dx +
				    ((2 * vis) / dx) * (U[i - 1][j] -
							U[i][j]);

				if (F[i + 1][j] & C_INCOR)
				    P[i + 1][j] -=
					((dx * vis) / (dy * dy)) *
					(U[i + 1][j + 1] -
					 U[i + 1][j - 1]);
			    } else {
				if (F[i + 1][j] & C_FRSLP)
				    P[i + 1][j] = P[i][j] + gx * dx;
			    }
			}

			if (F[i-1][j] & C_BND) {
			    if ((F[i-1][j] & C_NOSLP)
				|| (F[i-1][j] & C_IN)) {
				P[i - 1][j] =
				    P[i][j] - gx * dx -
				    ((2 * vis) / dx) * (U[i][j] -
							U[i - 1][j]);

				if (F[i-1][j] & C_INCOR)
				    P[i-1][j] +=
					((dx * vis) / (dy * dy)) *
					(U[i-1][j+1] -
					 U[i-1][j-1]);
			    } else {
				if (F[i-1][j] & C_FRSLP)
				    P[i-1][j] = P[i][j] - gx * dx;
			    }
			}

		    }

		    /* M: no obstacle cell */
		    POld = P[i][j];	/* for CONij calculations */

		    P[i][j] =
			((P[i + 1][j] + P[i - 1][j]) / (Z * dx * dx));
		    P[i][j] +=
			((P[i][j + 1] + P[i][j - 1]) / (Z * dy * dy));
		    P[i][j] += (R[i][j] / Z);

/*						iterationsCON++;*/
/*						if(iterationsCON == 10)*/
/*						{*/
/*	  					iterationsCON=0;*/

/*

   ((V[i][j] + V[i][j-1])/2)*((V[i][j] + V[i][j-1])/2)+((U[i][j] + U[i-1][j])/2)*((U[i][j] + U[i-1][j])/2)


*/


/*	            CONij = (fabs(P[i][j] - POld)/(fabs(P[i][j])+fabs(POld)+*/
/*											 U[i][j]*U[i][j] + V[i][j]*V[i][j] +fabs(gy*h)+fabs(gx*L)));*/

		    CONij = P[i][j] - POld;
		    CONij = fabs(CONij);

		    CONij /= (fabs(P[i][j]) +
			      fabs(POld) +
			      ((U[i][j] + U[i - 1][j]) / 2) *
			      ((U[i][j] + U[i - 1][j]) / 2) +
			      ((V[i][j] + V[i][j - 1]) / 2) *
			      ((V[i][j] + V[i][j - 1]) / 2) +
			      fabs(gy) * h + fabs(gx) * L);


		    if (CONij > CONmax)
			CONmax = CONij;

/*						}*/


		}		/* O: no full cell */
	    }			/* end for i */
	}			/* end for j */
/*	 }*/
/* end for iterations*/

/*			CursUp();*/

/*		if (V_CheckExitTrue())
			return(1); */

/*		printf("CONmax=%f\n",CONmax);*/
/*		printf("[1A");*/
	/* cursor up */

/*	itmax++;*/
/*	if(itmax > 150)*/
/*		return(1);*/
	/* problem */

/*    printf("%f\n",CONmax);
*/
    } while (CONmax > 0.0002);




    return (0);

}
