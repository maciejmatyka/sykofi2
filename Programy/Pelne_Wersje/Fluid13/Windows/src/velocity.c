#include <stdio.h>
#include <stdlib.h>


#include "bndflags.h"
#include "dimalloc.h"


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: void	CalculateVelocity(double **Un1, double **Vn1,int **F, double **R, double **P, int nx, int ny, double dx, double dy, double gx, double gy, double vis, double **U, double **V)
	  it:       Calculate velocity field.
		args:     blah
		return:   none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void CalculateVelocity(double **Un1, double **Vn1, int **F, double **R,
		       double **P, int nx, int ny, double dx, double dy,
		       double gx, double gy, double vis, double **U,
		       double **V, double dt)
{
    int i, j;
    double Sum1, Sum2;




    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    Un1[i][j] = U[i][j];
	    Vn1[i][j] = V[i][j];
	}			/* end for i */
    }				/* end for j */




    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if ((F[i][j] & C_FULL) || (F[i][j] & C_SUR)) {


		Sum2 = Sum1 = 0;

/* U */

		if (F[i + 1][j] & C_EMP)
		    Un1[i][j] = U[i][j] + gx * dt;
		else {
		    if (F[i + 1][j] & C_BND)
			Un1[i][j] = U[i][j];
		    else {
			if (U[i][j + 1] == 0.0) {
			    if (
				(F[i + 1][j + 1] & C_NOSLP
				 && F[i + 1][j + 1] & C_COR)
				|| (F[i][j + 1] & C_NOSLP
				    && F[i][j + 1] & C_COR)) U[i][j + 1] =
				    -1.0*U[i][j];
			    else {
				if (((F[i][j + 1] & C_EMP)
				     && (F[i + 1][j + 1] & C_EMP))
				    ||
				    ((F
				      [i + 1][j + 1] & C_FRSLP
				      && F[i + 1][j + 1] & C_COR)
				     || (F[i][j + 1] & C_FRSLP
					 && F[i][j + 1] & C_COR)))
				    U[i][j + 1] = U[i][j];
			    }
			} else {
			    if ((F[i][j + 1] & C_EMP)
				&& (F[i + 1][j + 1] & C_EMP))
				U[i][j + 1] = U[i][j];
			}


			if (U[i][j - 1] == 0.0) {
			    if (
				(F[i + 1][j - 1] & C_NOSLP
				 && F[i + 1][j - 1] & C_COR)
				|| (F[i][j - 1] & C_NOSLP
				    && F[i][j - 1] & C_COR)) U[i][j - 1] =
				    -1.0*U[i][j];

			    if (((F[i][j - 1] & C_EMP)
				 && (F[i + 1][j - 1] & C_EMP))
				||
				((F
				  [i + 1][j - 1] & C_FRSLP
				  && F[i + 1][j - 1] & C_COR)
				 || (F[i][j - 1] & C_FRSLP
				     && F[i][j - 1] & C_COR))) U[i][j -
								    1] =
				    U[i][j];
			} else {
			    if ((F[i][j - 1] & C_EMP)
				&& (F[i + 1][j - 1] & C_EMP))
				U[i][j - 1] = U[i][j];
			}


//#define nine_lapl




#ifndef	nine_lapl

			// sum 1 - laplasjan u
			Sum1 =
			    vis *
			    ((U[i + 1][j] + U[i - 1][j] - 2 * U[i][j]) /
			     (dx * dx) + (U[i][j + 1] + U[i][j - 1] -
					  2 * U[i][j]) / (dy * dy)) +
			    (P[i][j] - P[i+1][j]) / dx + gx;

#else

/// Nine points method for laplace operator

/// Epsilon for nine points method

#define eps	 (0.33333)

			Sum1 =
			    vis *(
				(
				
				(1-eps) * (U[i+1][j]+U[i-1][j]+U[i][j-1]+U[i][j+1]-4*U[i][j]) 
				+ eps*(U[i+1][j+1]+U[i+1][j-1]+U[i-1][j-1]+U[i-1][j+1]-4*U[i][j])

				
				)/(dy * dy))+(P[i][j] - P[i+1][j]) / dx + gx;
#endif
			
			Sum2 = 0.0;

			if (!(F[i][j] & C_URON))
			    Sum2 =
				-((U[i][j + 1] + U[i][j]) / 2) *
				((V[i][j] + V[i + 1][j]) / 2);

			if (!(F[i][j - 1] & C_URON))
			    Sum2 +=
				((U[i][j] + U[i][j - 1]) / 2) *
				((V[i][j - 1] + V[i + 1][j - 1]) / 2);


			Un1[i][j] = U[i][j] + dt * (Sum1 + Sum2 / dy +
					    (((U[i - 1][j] + U[i][j]) / 2) * ((U[i - 1][j] + U[i][j]) / 2) -
				        ((U[i + 1][j] +U[i][j]) / 2) * ((U[i + 1][j] +	U[i][j]) / 2)) /  dx);

			}
		}

/* V */

		Sum2 = Sum1 = 0;

		if (F[i][j + 1] & C_EMP)
		    Vn1[i][j] = V[i][j] + gy * dt;
		else {
		    if (F[i][j + 1] & C_BND)
			Vn1[i][j] = V[i][j];
		    else {
			if (V[i + 1][j] == 0.0) {
			    if (
				(F[i + 1][j + 1] & C_NOSLP
				 && F[i + 1][j + 1] & C_COR)
				|| (F[i + 1][j] & C_NOSLP
				    && F[i + 1][j] & C_COR)) V[i + 1][j] =
				    -1.0*V[i][j];
			    else {
				if (((F[i + 1][j] & C_EMP)
				     && (F[i + 1][j + 1] & C_EMP))
				    ||
				    ((F
				      [i + 1][j + 1] & C_FRSLP
				      && F[i + 1][j + 1] & C_COR))
				    || (F[i + 1][j] & C_FRSLP
					&& F[i + 1][j] & C_COR))
				    V[i + 1][j] = V[i][j];
			    }
			} else {
			    if ((F[i + 1][j] & C_EMP)
				&& (F[i + 1][j + 1] & C_EMP))
				V[i + 1][j] = V[i][j];
			}


			if (V[i - 1][j] == 0.0) {
			    if (
				(F[i - 1][j + 1] & C_NOSLP
				 && F[i - 1][j + 1] & C_COR)
				|| (F[i - 1][j] & C_NOSLP
				    && F[i - 1][j] & C_COR)) V[i - 1][j] =
				    -1.0*V[i][j];
			    else {
				if (((F[i - 1][j] & C_EMP)
				     && (F[i - 1][j + 1] & C_EMP))
				    ||
				    ((F
				      [i - 1][j + 1] & C_FRSLP
				      && F[i - 1][j + 1] & C_COR))
				    || (F[i - 1][j] & C_FRSLP
					&& F[i - 1][j] & C_COR))
				    V[i - 1][j] = V[i][j];
			    }
			} else {
			    if ((F[i - 1][j] & C_EMP)
				&& (F[i - 1][j + 1] & C_EMP))
				V[i - 1][j] = V[i][j];
			}


			// sum 1 - laplasjan v

			
#ifndef nine_lapl			
/// MAC
			Sum1 =
			    vis *
			    ((V[i][j + 1] + V[i][j - 1] - 2 * V[i][j]) /
			     (dy * dy) + (V[i + 1][j] + V[i - 1][j] -
					  2 * V[i][j]) / (dx * dx)) +
			    (P[i][j] - P[i][j + 1]) / dy + gy;

#else
/// Nine points method for laplace operator

			Sum1 =
			    vis *(
				(
				
					(1-eps) * (V[i+1][j]+V[i-1][j]+V[i][j-1]+V[i][j+1]-4*V[i][j])
					+ eps*(V[i+1][j+1]+V[i+1][j-1]+V[i-1][j-1]+V[i-1][j+1]-4*V[i][j])
				
				)/(dy * dy) )+(P[i][j] - P[i][j+1]) / dy + gy;
#endif

			Sum2 = 0.0;

			if (!(F[i][j] & C_URON))
			    Sum2 =
				-(((U[i][j] + U[i][j + 1]) / 2) *
				  ((V[i][j] + V[i + 1][j]) / 2));

			if (!(F[i - 1][j] & C_URON))
			    Sum2 +=
				((U[i - 1][j] + U[i - 1][j + 1]) / 2) *
				((V[i - 1][j] + V[i][j]) / 2);

			Vn1[i][j] = V[i][j] + dt * (Sum1 + Sum2 / dx +
						    (((V
						       [i][j - 1] +
						       V[i][j]) / 2) *
						     ((V
						       [i][j - 1] +
						       V[i][j]) / 2) -
						     ((V
						       [i][j + 1] +
						       V[i][j]) / 2) *
						     ((V
						       [i][j + 1] +
						       V[i][j]) / 2)) /
						    dy);

		    }
		}

	    }			/* cell contains no fluid */
	}			/* end for i */
    }				/* end for j */




    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if (F[i][j] & C_SUR) {

		/* U */
		if (F[i - 1][j] & C_EMP)
		    Un1[i - 1][j] = Un1[i][j];
		else {
		    if (F[i + 1][j] & C_EMP)
			Un1[i][j] = Un1[i - 1][j];
		}

		/* V */

		if (F[i][j - 1] & C_EMP)
		    Vn1[i][j - 1] = Vn1[i][j];
		else {
		    if (F[i][j + 1] & C_EMP)
			Vn1[i][j] = Vn1[i][j - 1];
		}

	    } else {
		if (F[i][j] & C_BND) {
		    if (F[i + 1][j] & C_OB)
			Un1[i][j] = U[i][j];

		    if (F[i][j + 1] & C_OB)
			Vn1[i][j] = V[i][j];
		}
	    }

	}			/* end for i */
    }				/* end for j */



/* calculate velocities near boundary walls */


    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if (F[i][j] & C_OB) {
		if (!(F[i][j] & C_EMP)) {
		    if (F[i + 1][j] & C_BND) {
			if (!(F[i + 1][j] & C_OUT))
			    Un1[i][j] = U[i][j];
			else
			    Un1[i][j] =
				Un1[i - 1][j] - (dx / dy) * (Vn1[i][j] -
							     Vn1[i][j -
								    1]);
		    }

		    if (F[i - 1][j] & C_BND) {
			if (!(F[i - 1][j] & C_OUT))
			    Un1[i - 1][j] = U[i - 1][j];
			else
			    Un1[i - 1][j] =
				Un1[i][j] - (dx / dy) * (Vn1[i][j] -
							 Vn1[i][j - 1]);

		    }

		    if (F[i][j + 1] & C_BND) {
			if (!(F[i][j + 1] & C_OUT))
			    Vn1[i][j] = V[i][j];
			else
			    Vn1[i][j] =
				Vn1[i][j - 1] - (dy / dx) * (Un1[i][j] -
							     Un1[i -
								 1][j]);

		    }

		    if (F[i][j - 1] & C_BND) {
			if (!(F[i][j - 1] & C_OUT))
			    Vn1[i][j - 1] = V[i][j - 1];
			else
			    Vn1[i][j - 1] =
				Vn1[i][j] - (dy / dx) * (Un1[i][j] -
							 Un1[i - 1][j]);
		    }

		}		/* no empty obstacles */
	    }			/* only OBSTACLE cells */
	}			/* end for i */
    }				/* end for j */


    /* copy V[n+1]->V[n] and U[n+1]->U[n] */



    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    U[i][j] = Un1[i][j];
	    V[i][j] = Vn1[i][j];
	}			/* end for i */
    }				/* end for j */

}
