

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "particle.h"
#include "bndinit.h"
#include "bndflags.h"
#include "tscells.h"
#include "minmax.h"

struct particle *PlaceParticle(double x, double y, struct particle *wrkpart, double Uk, double Vk, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);




   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: ParticleMovement(struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
	  it:				move particle using calculated velocities.
		args:     see.
		return:   none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void ParticleMovement(struct particle *firstparticle, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V, double dt)
{
  struct particle *wrkpart;
	double fx,fy;
	double u1,u2,u3,u4,v1,v2,v3,v4;
	double Sx,Sy;
	int i,j,ip,jp;    /* ip = i', jp = j' */
	double x,y;


/*	int flag;*/



/*	for(wrkpart=firstparticle;wrkpart->next!=NULL;wrkpart=wrkpart->next)*/
	for(wrkpart=firstparticle;wrkpart!=NULL;wrkpart=wrkpart->next)
	{
  	if(wrkpart->flag != P_AVAIL)
		{

			x=wrkpart->x;
			y=wrkpart->y;

/*      y=height-y;*/

 			i=(int) (x/dx + 1);
			j=(int) (y/dy + 1);



/*			j=ny-j;*/

				fx=(x/dx + 1)-i;
				fy=(y/dy + 1)-j;


/*			j=ny-j;*/
/*			fy=1-fy;*/



/*			if( (F[i][j] & C_FULL) || (F[i][j] & C_SUR) )*/
/*			{*/

        /* y calculations */


				if(fy<0.5)
					jp=j-1;
				else
					jp=j;

/*				printf("fy=%f\n",fy);*/

				Sx=(i*dx - dx/2 - x)/dx;
				Sy=(jp*dy - y)/dy;



				if( U[i-1][jp+1] == 0.0)
	        u1 = U[i-1][j];
				else
					if( F[i-1][j] & C_COR )
						u1 = 0.0;
					else
						u1 = U[i-1][jp+1];

				if( U[i][jp+1] == 0.0)
	        u2 = U[i][j];
				else
					if( F[i+1][j] & C_COR )
						u2 = 0.0;
					else
						u2 = U[i][jp+1];

				if( U[i-1][jp] == 0.0)
	        u3 = U[i-1][j];
				else
					if( F[i-1][j] & C_COR )
						u3 = 0.0;
					else
						u3 = U[i-1][jp];

				if( U[i][jp] == 0.0)
	        u4 = U[i][j];
				else
					if( F[i+1][j] & C_COR )
						u4 = 0.0;
					else
						u4 = U[i][jp];





				wrkpart->uk=(0.5+Sx)*(0.5-Sy)*u1 +
										(0.5-Sx)*(0.5-Sy)*u2 +
										(0.5+Sx)*(0.5+Sy)*u3 +
										(0.5-Sx)*(0.5+Sy)*u4;

				wrkpart->x=((( wrkpart->uk)*(dt)) + (wrkpart->x));


        /* x calculations */


				if(fx<0.5)
					ip=i-1;
				else
					ip=i;


/*				printf("fx=%f\n",fx);*/

				Sx=(ip*dx - x)/dx;
				Sy=(j*dy - dy/2 - y)/dy;


				if(V[ip][j] == 0.0)
					v1=V[i][j];
				else
          if(F[i][j+1] & C_COR)
						v1=0.0;
          else
						v1=V[ip][j];

				if(V[ip+1][j] == 0.0)
					v2=V[i][j];
				else
          if(F[i][j+1] & C_COR)
						v2=0.0;
          else
						v2=V[ip+1][j];

				if(V[ip][j-1] == 0.0)
					v3=V[i][j-1];
				else
          if(F[i][j-1] & C_COR)
						v3=0.0;
          else
						v3=V[ip][j-1];

				if(V[ip+1][j-1] == 0.0)
					v4=V[i][j-1];
				else
          if(F[i][j-1] & C_COR)
						v4=0.0;
          else
						v4=V[ip+1][j-1];






				wrkpart->vk=(0.5+Sx)*(0.5-Sy)*v1 +
										(0.5-Sx)*(0.5-Sy)*v2 +
										(0.5+Sx)*(0.5+Sy)*v3 +
										(0.5-Sx)*(0.5+Sy)*v4;

				wrkpart->y= (((wrkpart->vk)*(dt)) + (wrkpart->y));

/*				wrkpart->y= wrkpart->y + (wrkpart->vk)*(1/dt);*/


				wrkpart->i=(int) ((wrkpart->x)/dx + 1);
				wrkpart->j=(int) ((wrkpart->y)/dy + 1);

/*			}*/
 /* only particles in full or sur cells */
		} /* no P_AVAIL particles */
	} /* computate for next particle */


}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: void ParticleSetup16(struct particle *wrkpart)
	  it:       Allocate configuration of particles. At the moment supporting
              boxes, and balls of fluid.
		args:     struct particle *
		return:   none
    todo:     Parsing configurations from .opt file. Add fluid velocities.
              Use a table config (filled later with parse.c) to get config
              of particles.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

/*struct particle *ParticleSetup16(double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)*/
/*{*/
/*	struct particle *wrkpart;*/
/*	struct particle *partptr;*/
		/* for return */
/*	int i,j;*/



/* at the beginning all cells will be reflaggd to EMP cell */

/*	wrkpart=AllocOneParticle(0,0,P_AVAIL,0,0,0.0,0.0);*/

/*  partptr=wrkpart;*/

/* try */
/*	wrkpart=AllocParticleBox(1,1,100,100,2,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/


/* rownaj */
/*	wrkpart=AllocParticleBox(1,1,width/4-10,80,2,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/
/*	wrkpart=AllocParticleBox(width/4,1,width/2,40,2,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/

/* splash */
/*	wrkpart=AllocParticleBall(width/2,100+10+20,20,1,0.0,-4.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/
/*	wrkpart=AllocParticleBox(1,1,width,100+10,2,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/

/* sluza */
/*	wrkpart=AllocParticleBox(1,(int)dy*4,(int)dx*7,(int)dy*17,1,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/
/*	wrkpart=AllocParticleBox(1,1,width,(int)dy*4,1,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/

/* spad */
/*	wrkpart=AllocParticleBox(1,height-(int)dy*8,width,height-(int)dy*0,1,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/

/* broken damn */

/*	wrkpart=AllocParticleBall((int) dx*3 ,(int) dy*5,20,1,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/
/*	wrkpart=AllocParticleBox((int) dx*1 ,(int) dy*1,(int)dx*12,(int)dy*16,2,0.0,0.0,wrkpart,dx,dy,nx,ny,width,height,F,U,V);*/


/*	return(partptr);*/
/*}*/

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: struct particle *PlaceParticle(int x, int y, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F);
	  it:				Place particle. Test for boundary cells.
		args:
		return: none
    todo:   none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
struct particle *PlaceParticle(double x, double y, struct particle *wrkpart, double Uk, double Vk, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	int i,j,k;		/* k - flag for actual particle */

	k=0;

if(x>1 && y>1 && x<width && y<height)
{
		i=(int) (x/dx+1);
		j=(int) (y/dy+1);

/*		printf("%d %d\n",i,j);*/

	if(F[i][j] & C_BND)					/* i,j = BND ? */
	{
		if(F[i][j] & C_IN)				/* i,j = IN ? */
		{


		}
  }
	else                        /* i,j != BND */
	{
		if(!(F[i][j] & C_EMPBND))	/* i,j != EMPBND? */
		{

			ReflagTo(F,i,j,C_FULL);
      k = P_REG;


      /* At the moment, all particles have initial value of velocity - (0,0). */

			U[i][j]=Uk;
			U[i-1][j]=Uk;
			V[i][j]=Vk;
			V[i][j-1]=Vk;

/*	if(Uk!=0.0)*/
/*	printf("Uk=%f\n Vk=%f\n",Uk,Vk);*/

/*			wrkpart->next=AllocOneParticle(x,y,k,i,j,Uk,Vk);*/
			wrkpart->next=AllocOneParticle(x,y,k,i,j,Uk,Vk);
			wrkpart=wrkpart->next;


		}
  }
}

	return(wrkpart);
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: struct particle *AllocParticleBox(int bx0, int by0, int bx1, int by1, double partstep, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F)
	  it:       Allocate box of particles. Box specified by corners. Use
              partstep to make steps between particles.
		args:     see below...
		return:   struct particle *
    todo:     nooone
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

struct particle *AllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;


/*	if(x0<2) x0=2;*/
/*	if(x1<2) x1=2;*/
/*	if(x0>width-2) x0=width-2;*/
/*	if(x1>width-2) x1=width-2;*/
/*	if(y0<2) y0=2;*/
/*	if(y1<2) y1=2;*/
/*	if(y0>height-2) y0=height-2;*/
/*	if(y1>height-2) y1=height-2;*/


	for(y=y0;y<y1;y+=partstep)
	{
		for(x=x0;x<x1;x+=partstep)
		{

			wrkpart=PlaceParticle(x,y,wrkpart,Uk,Vk,dx,dy,nx,ny,width,height,F,U,V);

 	  }
  }

	return(wrkpart);
}



void NegAllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;

	struct particle *workpart;

	for(workpart=wrkpart;workpart!=NULL;workpart=workpart->next)
	{

/*		printf("!\n");*/
			x=workpart->x;
			y=workpart->y;

/*			printf("%f,%f\n",x,y);*/

			if( x > min(x1,x0) && x< max(x1,x0) && y > min(y1,y0) && y < max(y1,y0))
			{
					workpart->i = 0;
  	      workpart->j = 0;
    	    workpart->flag=P_AVAIL;
      	  workpart->x = 0;
        	workpart->y = 0;

			}


  }

}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: struct particle *AllocParticleBall(int x0, int y0, int r, double partstep, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F)
	  it:       Alocate ball of particles.
		args:     see...
		return:   struct particle *
    todo:     grrr..
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

struct particle *AllocParticleBall(int x0, int y0, int r, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;


/*	if(r>width || r>height) r=min(width,height);*/
	if(r<0) r=0;
/*	if(x0-r<2) r=2;*/
/*	if(y0-r<2) r=2;*/
/*	if(x0+r>width) r=width-x0;*/
/*	if(y0+r>height) r=height-y0;*/
                        /* r=0 - cancel ball creating */

	/* make ball */
  /* x,y,r */

	for(x=x0-r;x<x0+r;x+=partstep)
	{
		for(y=y0-r;y<y0+r;y+=partstep)
		{
			if( sqrt( (double) abs(x0-x)*abs(x0-x) + (double) abs(y0-y)*abs(y0-y)) < r )
			{
				wrkpart=PlaceParticle(x,y,wrkpart,Uk,Vk,dx,dy,nx,ny,width,height,F,U,V);
			}
		}
  }

	return(wrkpart);
}

void NegAllocParticleBall(int x0, int y0, int r, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V)
{
	double x,y;

	struct particle *workpart;

/*	if(r>width || r>height) r=min(width,height);*/
	if(r<0) r=0;
/*	if(x0-r<2) r=2;*/
/*	if(y0-r<2) r=2;*/
/*	if(x0+r>width) r=width-x0;*/
/*	if(y0+r>height) r=height-y0;*/
                        /* r=0 - cancel ball creating */

	/* make ball */
  /* x,y,r */

	for(workpart=wrkpart;workpart!=NULL;workpart=workpart->next)
	{

/*		printf("!\n");*/
			x=workpart->x;
			y=workpart->y;

/*			printf("%f,%f\n",x,y);*/

			if( sqrt( (double) abs(x0-x)*abs(x0-x) + (double) abs(y0-y)*abs(y0-y)) < r )
			{
				workpart->i = 0;
        workpart->j = 0;
        workpart->flag=P_AVAIL;
        workpart->x = 0;
        workpart->y = 0;
			}


  }

}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: void FreeAllParticles(struct particle *firstparticle);
	  it:       Dealloc all particles. Use part->*next position in
							particle struct.
		args:			struct particle *
		return:		none
    todo:     none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
void FreeAllParticles(struct particle *firstparticle)
{
  struct particle *wrkpart;

	for(wrkpart=firstparticle;wrkpart!=NULL;wrkpart=wrkpart->next)
  	free(wrkpart);

}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function: struct particle *AllocParticle(int px, int py, int P_Flag)
	  it:				Allocate ONE particle struct. Remember to unalloc it later.
		args:			int px,py - coordinates for particle
		return:		pointer to struct particle.
    todo:     handle errors.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

struct particle *AllocOneParticle(double px, double py, int P_Flag, int i, int j, double Uk, double Vk)
{
  struct particle *wrkpart;

  if((wrkpart=(struct particle *) calloc(1,sizeof(struct particle))) == NULL)
    {
     printf("Unable to alloc particles.\n");
     exit(1);
    }

  wrkpart->x = px;
	wrkpart->y = py;

	wrkpart->i = i;
	wrkpart->j = j;

/*	wrkpart->uk	=	Uk;*/
/*	wrkpart->vk	=	Vk;*/


	wrkpart->uk	=	Uk;
	wrkpart->vk	=	Vk;

	wrkpart->flag = P_Flag;
  wrkpart->next = NULL;

  return( wrkpart );
}

