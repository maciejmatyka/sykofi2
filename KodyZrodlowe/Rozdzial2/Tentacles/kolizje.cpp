


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "punkty.h"
#include "kolizje.h"
#include "cloth.h"


#define epsilon 0.02f


extern Obszar *KorzenObszarow;

void Kolizje(Punkt *korzenP, Obszar *korzenO)
{
	Obszar *roboczyO;

	Wektor Vbak;				// wektor przechowalnia dla kolizji

	Wektor Vn,Vs;		// predkosc styczna i normalna do pow. sfery
	Wektor Vn2,Vs2;		// predkosc styczna i normalna do pow. sfery


	Wektor n,n1,n2;			// normalna do pow. sfery

// kolizja - sfera, sciana

for(roboczyO=korzenO;roboczyO != NULL; roboczyO=roboczyO->prawy)
if(roboczyO->punkt)	   				// obszar nieruchomy - pomijaj
	if(roboczyO->flaga & O_AKT)
	{				

		if(roboczyO->punkt->r.x > xmax - roboczyO->R)
		{
			roboczyO->punkt->r.x=xmax - roboczyO->R;
			roboczyO->punkt->v.x=roboczyO->punkt->v.x * (-tlumienie);
		}

		if(roboczyO->punkt->r.y > ymax - roboczyO->R)
		{
			roboczyO->punkt->r.y=ymax - roboczyO->R;
			roboczyO->punkt->v.y=roboczyO->punkt->v.y * (-tlumienie);
		}

		if(roboczyO->punkt->r.z > zmax + roboczyO->R )
		{
			roboczyO->punkt->r.z=zmax + roboczyO->R;
			roboczyO->punkt->v.z=roboczyO->punkt->v.z * (-tlumienie);
		}


		if(roboczyO->punkt->r.x < xmin + roboczyO->R )
		{
			roboczyO->punkt->r.x=xmin + roboczyO->R ;
			roboczyO->punkt->v.x=roboczyO->punkt->v.x * (-tlumienie);
		}

		if(roboczyO->punkt->r.y < ymin + roboczyO->R )
		{
			roboczyO->punkt->r.y=ymin + roboczyO->R;
			roboczyO->punkt->v.y=roboczyO->punkt->v.y * (-tlumienie);
		}

		if(roboczyO->punkt->r.z < zmin + roboczyO->R )
		{
			roboczyO->punkt->r.z=ymin + roboczyO->R;
			roboczyO->punkt->v.z=roboczyO->punkt->v.z * (-tlumienie);
		}


//		Kopiuj(roboczyO->punkt->r,&(roboczyO->r1));

		roboczyO->r1 = roboczyO->punkt->r;

	}



}


extern float SPHERERAD;
void Kolizje(Cloth &c)
{
	int i,j;
	
	for(i=0;i<c.nx;i++) 
    for(j=0;j<c.ny;j++)
    {
        if(!(c.Points[i][j]->flaga & P_ZAW))
        if( c.Points[i][j]->r.dlugosc() < SPHERERAD )
        {
            Wektor nor = c.Points[i][j]->r;
            nor.normuj();
            nor = nor * SPHERERAD*1.02;
            
  
          c.Points[i][j]->f = c.Points[i][j]->f + nor*1.2;
            
//            c.Points[i][j]->r = nor;
        }
    }	
	
	

/*for(j=0;j<cloth.ny;j++)
for(i=0;i<cloth.nx;i++)
	if(!(cloth.Points[i][j]->flaga & P_ZAW))
	{
		if(cloth.Points[i][j]->r.x > xmax)
		{
			cloth.Points[i][j]->r.x=xmax;
			cloth.Points[i][j]->v.x=cloth.Points[i][j]->v.x * (-tlumienie);
		}

		if(cloth.Points[i][j]->r.y > ymax)
		{
			cloth.Points[i][j]->r.y=ymax;
			cloth.Points[i][j]->v.y=cloth.Points[i][j]->v.y * (-tlumienie);
		}

		if(cloth.Points[i][j]->r.z > zmax)
		{
			cloth.Points[i][j]->r.z=zmax;
			cloth.Points[i][j]->v.z=cloth.Points[i][j]->v.z * (-tlumienie);
		}


		if(cloth.Points[i][j]->r.x < xmin)
		{
			cloth.Points[i][j]->r.x=xmin;
			cloth.Points[i][j]->v.x=cloth.Points[i][j]->v.x * (-tlumienie);
		}

		if(cloth.Points[i][j]->r.y < ymin)
		{
			cloth.Points[i][j]->r.y=ymin;
			cloth.Points[i][j]->v.y=cloth.Points[i][j]->v.y * (-tlumienie);
		}

		if(cloth.Points[i][j]->r.z < zmin)
		{
			cloth.Points[i][j]->r.z=ymin;
			cloth.Points[i][j]->v.z=cloth.Points[i][j]->v.z * (-tlumienie);
		}
	}
*/
}





Obszar *AlokujObszar(float promien, Wektor r,  float tlum, int flaga, Punkt *p)
{
	Obszar *RoboczyObszar;


	if (!(RoboczyObszar = (Obszar *) malloc((unsigned) sizeof(Obszar)))) 
	{
		exit(0);
    }



	RoboczyObszar->flaga=flaga;
	RoboczyObszar->R=promien;
	RoboczyObszar->tlum=tlum;
	RoboczyObszar->punkt=p;

	RoboczyObszar->r1 = r;



	RoboczyObszar->r=(rand()/(float)RAND_MAX);
	RoboczyObszar->g=(rand()/(float)RAND_MAX);
	RoboczyObszar->b=(rand()/(float)RAND_MAX);

/*	RoboczyObszar->r=0.8;
	RoboczyObszar->g=0.7;
	RoboczyObszar->b=0.8;
*/

	RoboczyObszar->prawy=NULL;

	return (RoboczyObszar);
}



void DodajObszar(Obszar *korzen, float promien, Wektor r,  float tlum, int flaga, Punkt *p)
{
	Obszar *Roboczy;

	for(Roboczy=korzen;Roboczy->prawy != NULL; Roboczy=Roboczy->prawy);

	Roboczy->prawy=AlokujObszar(promien,r,tlum,flaga,p);

	if(p)
  	(*p).flaga |= P_OBSZ;

}

