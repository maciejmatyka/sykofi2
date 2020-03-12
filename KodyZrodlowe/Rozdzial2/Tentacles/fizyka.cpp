
#include <vector>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"
#include "view.h"
#include "kolizje.h"
 
#include "cloth.h"

extern Wektor Wind;

void ConstrainedSizeCorrection(Sprezyna &spring, int PossiblyChange)
{
	double roznica = (spring.punkt2->r - spring.punkt1->r).dlugosc() - spring.dlugosc;
	Wektor n;
#define MAX_ODCHYL ((float)(PossiblyChange*spring.dlugosc)/100.0)
	
	if(roznica > MAX_ODCHYL )
	{

		// teraz roznica to bedzie roznica miedzy
		// aktualna wielkoscia sprezyny, a
		// maksymalnym odchyleniem
		roznica = (spring.punkt2->r-spring.punkt1->r).dlugosc() - (spring.dlugosc + MAX_ODCHYL);
		
		n = spring.punkt2->r - spring.punkt1->r;
		n.normuj();
		n = n * (roznica/2.0);

		if(!(spring.punkt1->flaga & P_ZAW))
			spring.punkt1->r = spring.punkt1->r + n;
		
		if(!(spring.punkt2->flaga & P_ZAW))
			spring.punkt2->r = spring.punkt2->r - n;

/*		if(!(spring.punkt1->flaga & P_ZAW))
		if((spring.punkt2->flaga & P_ZAW))
			spring.punkt1->r = spring.punkt1->r + n;

		if(!(spring.punkt2->flaga & P_ZAW))
		if((spring.punkt1->flaga & P_ZAW))
			spring.punkt2->r = spring.punkt2->r - n;
*/

	//		spring.punkt2->r = spring.punkt2->r - n*(!(spring.punkt2->flaga & P_ZAW)) - n*((spring.punkt1->flaga & P_ZAW))*(!(spring.punkt2->flaga & P_ZAW));
	//		spring.punkt1->r = spring.punkt1->r + n*(!(spring.punkt1->flaga & P_ZAW)) + n*((spring.punkt2->flaga & P_ZAW))*(!(spring.punkt1->flaga & P_ZAW));

	}
}


// Constrained Cloth Correction on a Springs 
void CCC_Springs(Cloth &cloth)
{
	int i,j;

    for(i=0; i<cloth.springs.size(); i++)
		ConstrainedSizeCorrection(cloth.springs[i],cloth.lenght_constrain);
}



// przesuwa srodek obszaru sferycznego  do punktu przypisanego do niego, jesli pole -> punkt != NULL
void PrzesunObszary(Obszar *korzenO)
{
	Obszar *roboczyO;

		for(roboczyO=korzenO;roboczyO != NULL; roboczyO=roboczyO->prawy)
			if(roboczyO->flaga & O_AKT)
				if(roboczyO->punkt)
//					Kopiuj(roboczyO->punkt->r,&(roboczyO->r1));						
					roboczyO->r1=roboczyO->punkt->r;

}


//	Funkcja:
//		(wektor) Sila_Nazwa(Punkt 1, [Punkt 2]) 
//
//	Opis:
//		zwraca zdefiniowana sile dzialajaca na dany punkt
//
//	Uwaga: 
//		opcjonalnie - drugi punkt od ktorego sila tez zalezy (np. sila spr. Hooka)


Wektor Sila_Grawitacji(Punkt *p)
{
	return Graw * (*p).masa;
}

void Sila_Sprezystosci(Sprezyna &spring)
{
	Wektor r12;		
	Wektor v12;		
	float f;
	Wektor F;

	r12 = (spring.punkt1->r - spring.punkt2->r);
	v12 = (spring.punkt1->v - spring.punkt2->v);	 // roznica vi

	f = (r12.dlugosc() -  spring.dlugosc) * spring.ks + (v12 * r12) * spring.kd / r12.dlugosc();
	
	if(r12.dlugosc() != 0)
	{	
		F = (r12 * (1.0f/r12.dlugosc()));
		F = (F * f);
		
		spring.punkt2->f = F + spring.punkt2->f;
		spring.punkt1->f = F * (-1) + spring.punkt1->f;
	}
}




void WyznaczSily(Cloth &cloth, float dt)
{
	int i,j;


// grawitacja, dla wszystkich punktow struktury "class Cloth"

	for(j=0;j<cloth.ny;j++)
	for(i=0;i<cloth.nx;i++)
		if(!(cloth.Points[i][j]->flaga & P_ZAW))
			cloth.Points[i][j]->f= Sila_Grawitacji(cloth.Points[i][j]);

    for(i=0; i<cloth.springs.size(); i++)
    {
		Sila_Sprezystosci(cloth.springs[i]);
    }
	
	cloth.CalculateVertexNormals();		// trzymane w ->n

	for(j=0;j<cloth.ny;j++)				// wiatr na trojkaty 1
	for(i=0;i<cloth.nx;i++)
	if(!(cloth.Points[i][j]->flaga & P_ZAW))
	{
	   
        //	   cloth.Points[i][j]
        //      wind = normal vector from 0,0 to cloth.Points0[i][j]
        
        
        Wind = cloth.Points[i][j]->r;
        Wind.normuj();
        Wind = Wind * 0.04;

    	cloth.Points[i][j]->f = cloth.Points[i][j]->f + cloth.Points[i][j]->n * ((Wind - cloth.Points[i][j]->v)*cloth.Points[i][j]->n);


    }
	

}

void RozwiazEuler(Cloth &cloth, float dt)
{
	int i,j;
// grawitacja, dla wszystkich punktow struktury "class Cloth"

	for(j=0;j<cloth.ny;j++)
	for(i=0;i<cloth.nx;i++)
		if(!(cloth.Points[i][j]->flaga & P_ZAW))
		{
			cloth.Points[i][j]->dv = (cloth.Points[i][j]->f * (1/cloth.Points[i][j]->masa))*dt; 
			cloth.Points[i][j]->v = cloth.Points[i][j]->v + cloth.Points[i][j]->dv; 
			cloth.Points[i][j]->dr = cloth.Points[i][j]->v * dt; 
			cloth.Points[i][j]->r=cloth.Points[i][j]->r+cloth.Points[i][j]->dr;
		}

}




/* Wpisuje tylko przyrosty dv i dr jako wektory do struktury punktu i dodaje dzielac na dwa 
	w polach przyrostow przechowujemy poprzednie wartosci 
*/


void RozwiazMidPoint_k1(Cloth &cloth, float dt)
{
	Wektor przech;
	int i,j;

	for(j=0;j<cloth.ny;j++)
	for(i=0;i<cloth.nx;i++)
		if(!(cloth.Points[i][j]->flaga & P_ZAW))
		{

			cloth.Points[i][j]->dv = (cloth.Points[i][j]->f * (1/cloth.Points[i][j]->masa))*dt; 
			cloth.Points[i][j]->dr = (cloth.Points[i][j]->v + cloth.Points[i][j]->dv * 0.5f) * dt; 

	// przechowaj r w dr i v w dv
			przech = cloth.Points[i][j]->v;
			cloth.Points[i][j]->v = cloth.Points[i][j]->v + cloth.Points[i][j]->dv * 0.5;
			cloth.Points[i][j]->dv = przech;
 
			przech = cloth.Points[i][j]->r;

			cloth.Points[i][j]->r = cloth.Points[i][j]->r + cloth.Points[i][j]->dr * 0.5;
			cloth.Points[i][j]->dr = przech;
		}
}



/* 

w polach dr i dv struktury Punkt funkcja MidPoint_k1 zostawila stare wartosci v i r, do ktorych dodamy
przyrosty obliczone z wartosci nowych wyliczonych przez k2 */


void RozwiazMidPoint_k2(Cloth &cloth, float dt)
{

	Wektor dr,dv;
	int i,j;

	for(j=0;j<cloth.ny;j++)
	for(i=0;i<cloth.nx;i++)
		if(!(cloth.Points[i][j]->flaga & P_ZAW))
		{
			dv = (cloth.Points[i][j]->f * (1/cloth.Points[i][j]->masa))*dt; 
			cloth.Points[i][j]->v = dv + cloth.Points[i][j]->dv; 
			dr = cloth.Points[i][j]->v * dt; 
			cloth.Points[i][j]->r = dr;
			cloth.Points[i][j]->r = cloth.Points[i][j]->r + cloth.Points[i][j]->dr;
		}
}







