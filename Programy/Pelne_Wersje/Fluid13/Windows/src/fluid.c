
   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	 The MAC method for incompresible, viscous fluid flow.
	   Main file: fluid.c



              Maciej Matyka 
	      mailto:maq@panoramix.ift.uni.wroc.pl

                Uniwersytet Wroclawski
             Wydzial Fizyki i Astronomii



    Wszelkie pytania odnosnie kodu, problemy ze skompilowaniem i obsluga
   (nielatwa, to wiem) prosze kierowac bezposrednio do mnie. 

    Na koniec sprawa techniczna - w katalogu z ktorego uruchamiany jest program
    musi znajdowac sie katalog ./frames/ w ktorym program zapisuje
    kolejne fazy animacji.  

    Dla zainteresowanych: nowe wersje programu fluid, niebawem.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/




/*
 changes:

 define DTMAX number, bufor for every datas copy before timestep,
 dt increment doubled every timestep, halved when
 D[i][j] test fail, then copy old datas and run timestep with dt/2.

*/

/*

	h i L calculate maximum of xm and ym particle positions from setup


	Region #55 - done in ReflagCells in tscells.c

	skipped:
					 1. create and destroy particles (need for B_IN cells)

					 2.
							void	CalculateBNDConditions(int **F, double **U, double **V, int nx, int ny);
							in bndinit.c

           3. #55 For each SUR cell, calculate the velocity for the side next to an EMP
F              cell (equation).





@todo:     !!!  reading type of boundary in parse.c

@todo:     U*V corners ????????????


*/

/*
#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/exec.h>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bndflags.h"
#include "parse.h"
#include "dimalloc.h"
#include "bndinit.h"
#include "view.h"
#include "particle.h"
#include "visual.h"
#include "tscells.h"
#include "pressure.h"
#include "velocity.h"
#include "minmax.h"
#include "fluid.h"



struct particle *particle1;
struct particle *_particle1;

int timestep(double dx, double dy, double gx, double gy, double vis,
	     double t, double dt, int nx, int ny, int imax, int jmax,
	     int width, int height, int **Flag, double **Un1, double **Vn1,
	     double **U, double **V, double **P, double **D, double **R, double sta);
void CopyTable(double **T1, double **T2, int nx, int ny);
void CopyTableI(int **T1, int **T2, int nx, int ny);
void CopyParticles(struct particle *par1, struct particle *par2);
 
int teststability(double **U, double **V, int nx, int ny, double dx,
		  double dy, double dt, double vis, double sta);


#define DTSTART 1.0/8
#define DTMAX DTSTART

/*struct DosBase *DosBase;
*/

int teststability(double **U, double **V, int nx, int ny, double dx,
		  double dy, double dt, double vis, double sta)
{
    double UMAX, VMAX, UREADED, VREADED;
    int i, j;
    double A, d;

    UMAX = 0.0;
    VMAX = 0.0;

    for (j = 1; j <= ny; j++)
	for (i = 1; i <= nx; i++) {
	    UREADED = fabs(U[i][j]);
	    VREADED = fabs(V[i][j]);

	    UMAX = max(UREADED, UMAX);
	    VMAX = max(VREADED, VMAX);
	}

    A = max(UMAX, VMAX);
    d = max(dx, dy);


    if (((A * dt) / d) > sta) {
/*	printf("vmax=%f, dmax=%f, dt=%f, ", A, d, dt);
	printf(" vmax*dt/d=%f\n", (A * dt) / d);
	printf
	    (" (1)                             stability test fail !\n");
*/
	return (1);
    }



/* second stability condition */

/*
    UMAX = 0;
    VMAX = 0;

    for (j = 1; j <= ny; j++)
	for (i = 1; i <= nx; i++) {
	    UREADED = fabs(U[i][j] - U[i - 1][j]);
	    VREADED = fabs(V[i][j] - V[i][j - 1]);

	    UMAX = max(UREADED, UMAX);
	    VMAX = max(VREADED, VMAX);
	}

    A = max(UMAX, VMAX);
    d = max(1/dx, 1/dy);


    if (dt > ((2 * vis ) / (A * d))) {
	printf("dvmax=%f, dt=%f, right[(2*vis)/(A*d)]=%f\n", A, dt,
	       (2 * vis) / (A * d));
	printf
	    (" (2)                             stability test fail !\n");
	return (1);
    }
*/

    return (0);

}

void CopyParticles(struct particle *par1, struct particle *par2)
{
	struct particle *wrkpart1;
	struct particle *wrkpart2;

	wrkpart2 = par2;
	
	
        for (wrkpart1 = par1; wrkpart1 != NULL; wrkpart1 = wrkpart1->next)
        {
         	
         	wrkpart2->x = wrkpart1->x;
         	wrkpart2->y = wrkpart1->y;
         	wrkpart2->uk = wrkpart1->uk;
         	wrkpart2->vk = wrkpart1->vk;
         	wrkpart2->flag = wrkpart1->flag;
         	wrkpart2->i = wrkpart1->i;
         	wrkpart2->j = wrkpart1->j;
			wrkpart2->object = wrkpart1->object;

             	wrkpart2 = wrkpart2->next;
        }
}


void CopyTable(double **T1, double **T2, int nx, int ny)
{
    int i, j;

    for (j = 1; j <= ny; j++)
	for (i = 1; i <= nx; i++)
	    T2[i][j] = T1[i][j];
}


void CopyTableI(int **T1, int **T2, int nx, int ny)
{
    int i, j;

    for (j = 1; j <= ny; j++)
	for (i = 1; i <= nx; i++)
	    T2[i][j] = T1[i][j];
}



