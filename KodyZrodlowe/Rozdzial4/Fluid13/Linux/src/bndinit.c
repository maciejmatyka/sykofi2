
#include <stdio.h>
#include <stdlib.h>


#include "bndflags.h"
#include "bndinit.h"



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
						void BundaryCellsToPoints(int BCells[], int BPoints[], int BNum, int width, int height, double dx, double dy);
	  it:
			      Convert Boundary cell datas to output points type of datas.
		args:
			      BCells - my table of cells typed to be boundary.
      			BPoints - table of points typed to be boundary.
		return:	none
		todo:		none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void BundaryCellsToPoints(int BCells[], int BPoints[], int BNum, int width,
			  int height, double dx, double dy)
{
    int i;

    for (i = 0; i < BNum; i++) {

	BPoints[i * 2 + i] = BCells[i * 2 + i] * dx - 1;
	BPoints[i * 2 + i + 1] = BCells[i * 2 + i + 1] * dy - 1;
	BPoints[i * 2 + i + 2] = BCells[i * 2 + i + 2];

	if (BPoints[i * 2 + i] > (width - dx - 1))
	    BPoints[i * 2 + i] -= dx;

	if (BPoints[i * 2 + i + 1] > (height - dy - 1))
	    BPoints[i * 2 + i + 1] -= dy;


    }

}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
							int CellPosX(int dx, int i);
	  it:       Return center positions for i Cell by alg:
														cx=dx-dx/2;
		args:
							dx - dimension on i direction
							i  - number of cell
		return:   x center position of cell
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

int CellPosX(double dx, int i)
{
    return ((int) (i * dx - dx / 2));
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
							int CellPosY(double dy, int j);
 	  it:       Return center positions for j Cell by alg:
														cy=dy-dy/2;
		args:
							dy - dimension on j direction
							j  - number of cell
		return:   y center position of cell
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

int CellPosY(double dy, int j)
{
    return ((int) (j * dy - dy / 2));
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
							void CellSetup15(int** F, int BPoints[], int BNum, int width, int height, int nx, int ny, double dx, double dy);
	  it:
							define a boundary conditions from boundary points list
		args:
							int** F - table of cell flags
							BPoints[] - table of boundary points (xm,ym,typem,...)
		return:		none
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void CellSetup15(int **F, int BPoints[], int BNum, int width, int height,
		 int nx, int ny, double dx, double dy)
{
    int i, j, xm, ym, k, typem, xm1, ym1, typem1, xm2, ym2;	/*,typem2; */

    int m = 0;			/* offset for boundary points in table */

    i = 0;
    j = 0;
    typem = 0;

    for (k = 0; k < BNum - 1; k++) {	/* BNum - 4:  real number of boundary points */
	xm = BPoints[m];
	ym = BPoints[m + 1];
		typem=BPoints[m+2];

	xm1 = BPoints[m + 3];
	ym1 = BPoints[m + 4];
		typem1=BPoints[m+5];

	xm2 = BPoints[m + 6];
	ym2 = BPoints[m + 7];
/*		typem2=BPoints[m+8];*/


/*	printf("noslp=%d, frslp=%d\n",C_NOSLP,C_FRSLP);*/
/*	printf("typem=%d, typem1=%d\n",typem,typem1);*/

/*	typem = (C_FRSLP);*/
/*	typem1 = (C_FRSLP);*/
/*		typem2=C_FRSLP;*/

   /*_____________________________
   """""""""""""""""""""""""""""""
		Case 1
		Boundary line. xm > xm+1
   _______________________________
   """""""""""""""""""""""""""""*/


	if (xm > xm1) {
	    i = (int) (xm / (int) dx + 1.999);
	    j = (int) (ym / (int) dy + 2.001);

	    while (!(CellPosX(dx, i) <= xm1)) {
		F[i][j] |= C_BND | typem;
		F[i][j - 1] |= C_OB;
		if (!(F[i][j] & C_OUT))
		    F[i][j - 1] |= C_URON;
		i -= 1;
	    }

	    if (ym2 < ym1) {
/*						F[i][j]	|=	C_EMPBND;*//* later... */
		if ((typem & C_FRSLP) || (typem1 & C_FRSLP))
		    F[i][j] |= C_FRSLP;
	    }
	    if (ym2 > ym1) {
		F[i + 1][j] |= C_COR;
		F[i][j - 1] |= C_URON;
	    }
/* R */
	    m += 3;
	}


   /*_____________________________
   """""""""""""""""""""""""""""""
		Case 2
		Boundary line. xm < xm+1
   _______________________________
   """""""""""""""""""""""""""""*/



	if (xm < xm1) {
	    i = (int) (xm / (int) dx + 2.001);
	    j = (int) (ym / (int) dy + 1.999);

	    while (!(CellPosX(dx, i) >= xm1)) {
		F[i][j] |= C_BND | typem;
		F[i][j + 1] |= C_OB;
		if (!(F[i][j] & C_OUT))
		    F[i][j] |= C_URON;
		i += 1;
	    }

	    if (ym2 > ym1) {
/*						F[i][j]	|=	C_EMPBND;*//* later... */
		if ((typem & C_FRSLP) || (typem1 & C_FRSLP))
		    F[i][j] |= C_FRSLP;
	    }
	    if (ym2 < ym1)
		F[i - 1][j] |= C_COR;
/* R */
	    m += 3;
	}




   /*_____________________________
   """""""""""""""""""""""""""""""
		Case 3
		Boundary line. ym < ym+1
   _______________________________
   """""""""""""""""""""""""""""*/

	if (ym < ym1) {
	    i = (int) (xm / (int) dx + 2.001);
	    j = (int) (ym / (int) dy + 2.001);

	    while (!(CellPosY(dy, j) >= ym1)) {
		F[i][j] |= C_BND | typem;
		F[i - 1][j] |= C_OB;

		if (!(F[i][j] & C_OUT))
		    F[i - 1][j] |= C_URON;
		j += 1;
	    }

	    if (xm2 < xm1) {
/*						F[i][j]	|=	C_EMPBND;*//* later... */
		if ((typem & C_FRSLP) || (typem1 & C_FRSLP))
		    F[i][j] |= C_FRSLP;
	    }
	    if (xm2 > xm1)
		F[i][j - 1] |= C_COR;
/* R */
	    m += 3;
	}


   /*_____________________________
   """""""""""""""""""""""""""""""
		Case 4
		Boundary line. ym > ym+1
   _______________________________
   """""""""""""""""""""""""""""*/

	if (ym > ym1) {
	    i = (int) (xm / (int) dx + 1.999);
	    j = (int) (ym / (int) dy + 1.999);

	    while (!(CellPosY(dy, j) <= ym1)) {
		F[i][j] |= C_BND | typem;
		F[i + 1][j] |= C_OB;

		if (!(F[i][j] & C_OUT))
		    F[i][j] |= C_URON;
		j -= 1;
	    }

/* last boundary? *//* not needed - 3 last boundaries are (0,0,0) defined */

	    if (xm2 > xm1 || k == BNum - 4 - 1) {
				F[i][j]	|=	C_URON;
/*						F[i][j]	|=	C_EMPBND|C_URON;*//* later... */

		if ((typem & C_FRSLP) || (typem1 & C_FRSLP))
		    F[i][j] |= C_FRSLP;
	    }

	    if (xm2 < xm1)
		F[i][j + 1] |= C_COR;
/* R */
	    m += 3;
	}


    }				/* more boundaries? */




/*	F[32][2] |= C_COR;*/
/*	F[31][1] |= C_COR;*/
/*	F[2][2] |= C_COR;*/


	F[1][2] &= ~C_COR;
/*	F[i][j] |= typem;*/


/* C */

/*	F[2][2] |= C_URON;*/

/* Flag any IN cell which is adjacent to a NOSLP cell as INCOR */

/*	for(j=1;j<=ny;j++)*/
/*	{*/
/*	  for(i=1;i<=nx;i++)*/
/*			{*/
/*				if((F[i][j] & C_IN) || (F[i][j+1] & C_NOSLP))*/
/*        F[i][j] |= C_INCOR;*/
/*			}*/
/*	}*/





/* my correction for C_URON cells */


/*    for (j = 1; j <= ny; j++)*/
/*	for (i = 1; i <= nx; i++)*/
/*	{*/
/*	    F[i][j] &= ~C_URON;*/
/*	    F[i][j] &= ~C_COR;*/
	    /* for COR correction */


/*	    if (F[i + 1][j + 1] & C_BND && (!(F[i][j] & C_BND)))*/
/*		F[i][j] |= C_URON;*/

/*	}*/

/* my correction for C_COR cells*/

/*    for (j = 1; j <= ny; j++) {*/
/*	for (i = 1; i <= nx; i++) {*/
/*	    if (F[i][j] & C_OB && F[i + 1][j + 1] & C_OB*/
/*		&& F[i + 1][j] & C_BND) F[i + 1][j] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i + 1][j + 1] & C_OB*/
/*		&& F[i][j + 1] & C_BND) F[i][j + 1] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i + 1][j - 1] & C_OB*/
/*		&& F[i + 1][j] & C_BND) F[i + 1][j] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i + 1][j - 1] & C_OB*/
/*		&& F[i][j - 1] & C_BND) F[i][j - 1] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i - 1][j + 1] & C_OB*/
/*		&& F[i - 1][j] & C_BND) F[i - 1][j] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i - 1][j + 1] & C_OB*/
/*		&& F[i][j + 1] & C_BND) F[i][j + 1] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i - 1][j - 1] & C_OB*/
/*		&& F[i - 1][j] & C_BND) F[i - 1][j] |= C_COR;*/
/**/
/*	    if (F[i][j] & C_OB && F[i - 1][j - 1] & C_OB*/
/*		&& F[i][j - 1] & C_BND) F[i][j - 1] |= C_COR;*/
/**/
/**/
/*	}*/
/*    }*/


/* one sweep to kill all outsided cells */


/* left -> right */
    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {
	    if (F[i][j] & (C_BND|C_COR))	/* if C_BND cell found */
		break;

	    F[i][j] |= C_EMPBND;
	}
    }


/* right -> left */
    for (j = 1; j <= ny; j++) {
	for (i = nx; i > 1; i--) {
	    if (F[i][j] & (C_BND|C_COR))	/* if C_BND cell found */
		break;

	    F[i][j] |= C_EMPBND;
	}
    }

/* down -> up */
    for (i = 1; i <= nx; i++) {
	for (j = 1; j <= ny; j++) {
	    if (F[i][j] & (C_BND|C_COR))	/* if C_BND cell found */
		break;

	    F[i][j] |= C_EMPBND;
	}
    }

/* up -> down */
    for (i = 1; i <= nx; i++) {
	for (j = ny; j > 1; j--) {
	    if (F[i][j] & (C_BND|C_COR))	/* if C_BND cell found */
		break;

	    F[i][j] |= C_EMPBND;
	}
    }
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
							void ShowBoundary(int** F, int nx, int ny);
	  it:
							Show actual config of Flag[nx][JMAX] table.
	         	  At the moment - only C_BND flag as "*" char -
	          	other: " ".
		args:
							int** F - table of cell flags
							int nx, int ny - number of cells i,j dir.
		return: 	none
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/



void ShowBoundary(int **F, int nx, int ny)
{
    int i, j;

    printf("-");
    for (i = 1; i <= nx; i++)
	printf("-");
    printf("-");

    printf("\n");


    for (j = ny; j > 0; j--) {

	printf("|");

	for (i = 1; i <= nx; i++) {
	    printf(" ");

	    if (F[i][j] & C_URON)
		printf("\bu");
/*						if(F[i][j] & C_OB)*/
/*	             printf("\b°");*/
	    if (F[i][j] & C_COR)
		printf("\bC");
/*							      if(F[i][j] & C_BND)*/
/*											printf("\b*");*/
/*												if(F[i][j] & C_EMPBND)*/
/*		    						      printf("\b.");*/
	}
	printf("|\n");
    }

    printf("-");
    for (i = 1; i <= nx; i++)
	printf("-");
    printf("-");
    printf("\n");

}




   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: CalculateBNDConditions(int **F, double **U, double **V, int nx, int ny);
	  it:       Calculate and store velocities
		args:     **F - table of cells flags, **U,V - u,v for every cells
		return:   none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void CalculateBNDConditions(double **Vn1, double **Un1, int **F,
			    double **U, double **P, double **V, double **D,
			    int nx, int ny, double gx, double gy,
			    double vis, double dx, double dy)
{
    int i, j;

    for (j = 1; j <= ny; j++)
			for (i = 1; i <= nx; i++)
			{

		    if (F[i][j] & C_OB)
				{
					if ((F[i][j] & C_FULL) || (F[i][j] & C_SUR)) {

			    /* zakîadam, ûe zawsze FRSLP */

		    /*    BO
		       BO
		       BO
		       BO   */

		    if (F[i - 1][j] & C_BND)
				{

					if(F[i - 1][j] & C_FRSLP)
					{
							V[i-1][j] = V[i][j];
							U[i-1][j] = 0.0;
					}

					if(F[i - 1][j] & C_NOSLP)
					{
							V[i-1][j] = -1.0*V[i][j];
							U[i-1][j] = 0.0;
					}


/*						printf("%d,%d OB\n",i,j);*/
/*	        V[i-1][j] = V[i][j];*/
/*					V[i-1][j] = 0;*/
/*					U[i-1][j-1] = (-1.0) * U[i-1][j];*/
/*					U[i-1][j+1] = (-1.0) * U[i-1][j];*/

/*				P[i-1][j] = P[i][j];*/

/* - gx*dx;*/
	 /*- ((2*vis)/dx)*(U[i-1][j] - U[i][j]);*/
/*					D[i-1][j] = D[i][j];*/
		    }

		    /*    OB
		       OB
		       OB
		       OB   */

		    if (F[i + 1][j] & C_BND)
				{

					if(F[i + 1][j] & C_FRSLP)
					{
							V[i+1][j] = V[i][j];
							U[i][j] = 0.0;
					}

					if(F[i + 1][j] & C_NOSLP)
					{
							V[i+1][j] = -1.0*V[i][j];
							U[i][j] = 0.0;
					}


/*	        V[i+1][j] = V[i][j];*/

/*					V[i+1][j] = 0;*/
/*					U[i][j-1] = (-1.0) * U[i][j];*/
/*					U[i][j+1] = (-1.0) * U[i][j];*/


/*					P[i+1][j] = P[i][j];*/
/* - gx*dx;*/
			/* - ((2*vis)/dx)*(U[i+1][j] - U[i][j]); */
/*					D[i+1][j] = D[i][j];*/
		    }


		    /*
		       OOOOOOOOOOO
		       BBBBBBBBBBB
		     */

		    if (F[i][j - 1] & C_BND)
				{

					if(F[i][j-1] & C_FRSLP)
					{
						V[i][j-1] = 0.0;
						U[i][j-1] = U[i][j];
					}

					if(F[i][j-1] & C_NOSLP)
					{
						V[i][j-1] = 0.0;
						U[i][j-1] = -1.0*U[i][j];
					}

/*					U[i-1][j-1] = U[i][j];*/


/*						V[i-1][j-1] = (-1.0) * V[i][j-1];*/
/*						V[i+1][j-1] = (-1.0) * V[i][j-1];*/


/*						U[i][j-1] = 0;*/


/*				P[i][j-1] = P[i][j];*/

/* + gy*dy;*/
			/* - ((2*vis)/dy)*(V[i][j-1] - V[i][j]); */
/*					D[i][j-1] = D[i][j];*/
		    }

		    /*
		       BBBBBBBBBBB
		       OOOOOOOOOOO
		     */

		    if (F[i][j + 1] & C_BND)
				{

					if(F[i][j+1] & C_FRSLP)
					{
						U[i][j+1] = U[i][j];
						V[i][j] = 0.0;
					}

					if(F[i][j+1] & C_NOSLP)
					{
						U[i][j+1] = -1.0*U[i][j];
						V[i][j] = 0.0;
					}



/*					U[i-1][j+1] = U[i-1][j];*/


/*						U[i][j+1] = 0;*/
/*						V[i-1][j] = (-1.0) * V[i][j];*/
/*						V[i+1][j] = (-1.0) * V[i][j];*/


/*				P[i][j+1] = P[i][j];*/
/* + gy*dy;*/
			/* - ((2*vis)/dy)*(V[i][j-1] - V[i][j]); */
/*					D[i][j+1] = D[i][j];*/
		    }



/*		    if (F[i][j + 1] & C_BND)*/
/*			    if (F[i+1][j] & C_BND)*/
/*					{*/
/*						U[i][j] = 0.0;*/
/*						V[i][j] = 0.0;*/
/*            V[i+1][j] = 0.0;*/
/*						U[i][j+1] = 0.0;*/
/*					}*/


			}
		}
	    /* OB + FULL */
	}




}
