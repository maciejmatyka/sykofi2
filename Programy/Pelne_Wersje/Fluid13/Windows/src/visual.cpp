#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#include "bndflags.h"
#include "particle.h"
#include "bndinit.h"
#include "minmax.h"

#define COLMAX 225

#include "view.h"



void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	double t;

	for(t=0;t<1;t+=0.1)
		View::V_Point((int) ((1-t)*x1+t*x2),(int) ((1-t)*y1+t*y2), r, g, b);

}

void PlotVelocities(int width, int height, int nx, int ny, double dx, double dy, double **U, double **V, int **F)
{
	int i,j;
	double UMAX,VMAX,fU,fV;

	UMAX=0.0;
	VMAX=0.0;

	for(j=4;j<=(ny-4);j+=2)
		for(i=4;i<=(nx-4);i+=2)
			if(!(F[i][j] & C_BND || F[i][j] & C_EMPBND))
			{
				fU=fabs(U[i][j]);
				fV=fabs(V[i][j]);

				UMAX=max(fU,UMAX);
				VMAX=max(fV,VMAX);
			}

	for(j=4;j<=(ny-4);j+=2)
		for(i=4;i<=(nx-4);i+=2)
/*			if(!(F[i][ny-j+1] & C_BND || F[i][ny-j+1] & C_EMPBND))*/
/*					V_Line((int)((i-0.5)*dx),(int)((j-0.5)*dy),(int)((i-0.5)*dx)+(int)((U[i][j] * (dx/2)) / UMAX),(int)((j-0.5)*dy)+(int)((V[i][j] * (dy/2)) / VMAX));*/
			DrawLine((int)((i-0.5)*dx),(1+height)-(int)((j-0.5)*dy),(int)((i-0.5)*dx)+(int)((U[i][j] * (dx)) / UMAX),(1+height)-(int)((j-0.5)*dy)-(int)((V[i][j] * (dy)) / VMAX),255,0,0);


}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:	void PlotParticles(struct particle *firstparticle);
	  it:       Plot particles on output view.
		args:     struct of firrst particle.
		return:   none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void PlotParticles(struct particle *wrkpart2, int width, int height, double **table, int nx, int ny)
{
    struct particle *wrkpart;


    int i,j;
    double mymax,mymin;
    double t;


    mymin=mymax=fabs(table[1][1]);

    
    for(i=1;i<=nx;i++)
	for(j=1;j<=ny;j++)
	{
	    mymax=max(mymax,fabs(table[i][j]));

	    if(table[i][j] != 0)
		mymin=min(mymin,fabs(table[i][j]));
	}

/*
    mymin = 0;
    mymax = 255;
*/    
    



/*    printf("max=%f,min=%f\n",mymax,mymin);    
*/
/*    V_ClearRect(0, 0, width-1, height-1);
*/
/*
	    mymax=height;
	    mymin=0;
*/

    for (wrkpart = wrkpart2; wrkpart != NULL; wrkpart = wrkpart->next)
	if (wrkpart->flag != P_AVAIL) {


	    if (wrkpart->x > 0 && wrkpart->x < width && wrkpart->y > 0
		&& wrkpart->y < height)
		{
		
	if(mymin==0 && mymax==0)
	{
		V_PointCross((int) (wrkpart->x + 1),
			(int) (height - (wrkpart->y) -1)

			,120,140,240);
	

	} else
		V_PointCross((int) (wrkpart->x + 1),
			(int) (height - (wrkpart->y) -1)

			,120 + (int) ((fabs(table[wrkpart->i][wrkpart->j]) - mymin)*(COLMAX-120))/(mymax-mymin)
			,140 + (int) ((fabs(table[wrkpart->i][wrkpart->j]) - mymin)*(COLMAX-140))/(mymax-mymin)
			,180 + (int) ((fabs(table[wrkpart->i][wrkpart->j]) - mymin)*(COLMAX-180))/(mymax-mymin));
		}


	}


}

void PlotParticlesColor(struct particle *wrkpart2, int width, int height, int nx, int ny, int R, int G, int B)
{
    struct particle *wrkpart;

    for (wrkpart = wrkpart2; wrkpart != NULL; wrkpart = wrkpart->next)
		if (wrkpart->flag != P_AVAIL) 
		{	
			if (wrkpart->x > 0 && wrkpart->x < width && wrkpart->y > 0 && wrkpart->y < height)
				V_Point((int) (wrkpart->x + 1),(int) (height - (wrkpart->y) -1),R,G,B);
		}
}



void PlotParticlesColorObject(struct particle *wrkpart2, int width, int height, int nx, int ny, int R, int G, int B, int object)
{
    struct particle *wrkpart;

    for (wrkpart = wrkpart2; wrkpart != NULL; wrkpart = wrkpart->next)
		if (wrkpart->flag != P_AVAIL) 
		{	
			if (wrkpart->x > 0 && wrkpart->x < width && wrkpart->y > 0 && wrkpart->y < height)
		   		if(wrkpart->object == object)
				V_DIV2Point((int) (wrkpart->x + 1),(int) (height - (wrkpart->y) -1),R,G,B);
		}
}






   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void DrawBndCells(int width, int height, int nx, int ny, double dx, double dy, int** F);
	  it:
				Draw cell configuration on the screen. Specify of cell gfx - see
        code.
		args:
				int width, int height, int nx, int ny, double dx, double dy, int** F
				int** F - table of cells flags.
		return: none
    todo:   none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
void DrawBndCells(int width, int height, int nx, int ny, double dx,
		  double dy, int **F)
{
    int i, j, yr, xr;

    for (j = 1; j <= ny; j++) {
	for (i = 1; i <= nx; i++) {

	    if (F[i][j] & C_EMPBND || F[i][j] & C_BND)
	    {
		if(dx == dy)
	    	{


		

/*			for(xr = 1; xr < (dx-1); xr+=2)
*/
			for(yr = 1; yr <= (dy); yr+=1)
			for(xr = 1; xr <= (dx); xr+=1)
        	    		V_Point( (i-1)*dx+xr, height-((j-1)*dy+yr), 156, 168, 180);
			
/*
			for(yr = 2; yr < dy-1; yr--) 
			{
				xr++;
				V_Point( (i-1)*dx+xr+1-1, height-((j-1)*dy+yr), 96, 120, 140);
			}


			xr=dx-1;
*/			
/*			for(xr = dx-1; xr > 1; xr-=2)
*/
/*
			for(yr = 2; yr < dy -1; yr++) 
			{
				xr--;

				V_Point( (i-1)*dx+xr+1, height-((j-1)*dy+yr), 96, 120, 140);
			}
*/


		}
		
	    }


	    if (F[i][j] & C_BND)
	    {


		if(F[i-1][j] & C_OB)
		{
    			xr =1;
			for(yr = 1; yr <= (dy); yr++) 
			/*		for(xr = 1; xr < (dx ); xr++)*/
			V_Point( (i-1)*dx+xr, height-((j-1)*dy+yr), 46, 40, 80);



		}
		

		if(F[i+1][j] & C_OB)
		{
			xr=dx;	
		
			for(yr = 1; yr <= (dy); yr++)
		/*	for(xr = 1; xr < (dx); xr++)*/
    	    		V_Point( (i-1)*dx+xr, height-((j-1)*dy+yr), 46, 40, 80);
		}	

		if(F[i][j-1] & C_OB)
		{
			yr = 1;
			
		/*	for(yr = 1; yr < (dy); yr++)*/
			for(xr = 1; xr <= (dx ); xr++)
    			V_Point( (i-1)*dx+xr, height-((j-1)*dy+yr), 46, 40, 80);
		}


		if(F[i][j+1] & C_OB)
		{
			yr = dy;
			
		/*	for(yr = 1; yr < (dy); yr++)*/
			for(xr = 1; xr <= (dx ); xr++)
    			V_Point( (i-1)*dx+xr, height-((j-1)*dy+yr), 46, 40, 80);
		}



/*		    V_Line((int) ((i - 1) * dx + 2),
			   height - (int) ((j - 1) * dy + yr),
			   (int) ((i - 1) * dx + dx - 2),
			   height - (int) ((j - 1) * dy + yr));
*/
	    }


/*			if(F[i][j] & C_OB)*/
/*	    {*/
/*					for(yr=2; yr < (dy-1) ;yr+=5)*/
/*							V_Line( (int) ((i-1)*dx+2), height - (int) ((j-1)*dy+yr), (int) ((i-1)*dx+dx-2), height - (int) ((j-1)*dy+yr));*/
/*	    }*/

/*  		if(F[i][j] & C_NSWO)*/
/*	    {*/
/*						for(yr=2; yr < (dy-1) ;yr+=2)*/
/*								V_Line( (int) ((i-1)*dx+2), height - (int) ((j-1)*dy+yr), (int) ((i-1)*dx+dx-2), height - (int) ((j-1)*dy+yr));*/
/*	    }*/


	}
    }

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
						void DrawMesh(int width, int height, int nx, int ny, double dx, double dy)
	  it:			Draw cell mesh on output graphics view.
		args:   int width, int height, int nx, int ny
		return: none
		todo:		none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void DrawMesh(int width, int height, int nx, int ny, double dx, double dy)
{
    int i, j, xl, yl;


    for (i = 1; i <= nx; i++)
    {
	xl = i * dx - dx;

	for(yl = 1; yl < (height-1); yl++)
	    V_Point(xl,yl,220,210,200);

/*	V_Line(xl, 0, xl, height - 1);*/
    }

    for (j = 1; j <= ny; j++)
    {
	yl = j * dy - dy;
	for(xl = 1; xl < (width - 1); xl++)
	    V_Point(xl,yl,220,210,200);
/*	V_Line(0, yl, width - 1, yl);*/
    }

}
