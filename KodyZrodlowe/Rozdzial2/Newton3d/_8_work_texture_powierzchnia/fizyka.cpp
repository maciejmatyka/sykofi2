

#include <stdio.h>
#include <stdlib.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"
#include "view.h"
#include "kolizje.h"



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




void WyznaczSily(Punkt *korzenP, Sprezyna *korzenS, Obszar *korzenO)
{
	Punkt *roboczyP;
	Sprezyna *roboczaS;
//	Obszar *roboczyO;

//	Wektor Vbak,Vbak1;
//	float w1,w2,w3,w4;			// wspolczynniki dla kolizji
//	int n_kolizji;			// liczba kolizji




	float d_odl;

	float f;
	Wektor F,r_pred,Odl;


//	Wektor Vn,Vs;		// predkosc styczna i normalna do pow. sfery
	Wektor n;			// normalna do pow. sfery







// grawitacja
	for(roboczyP=korzenP;roboczyP != NULL; roboczyP=roboczyP->prawy)
	{
	 // grawitacja
	// Fg= m * g

	if(!(roboczyP->flaga & P_ZAW))
//		roboczyP->f=Mnozenie(Graw,roboczyP->masa);

		roboczyP->f= Graw * roboczyP->masa;

	}





// sprezynki


#define p1 roboczaS->punkt1		 // wskaznik na strukture 1 punktu sprezynki
#define p2 roboczaS->punkt2

#define s_dlug roboczaS->dlugosc	 // dlugosc sprezynki
#define C roboczaS->c
					   
	for(roboczaS=korzenS;roboczaS != NULL; roboczaS=roboczaS->prawy)
	{

		Odl = (p1->r - p2->r);
		d_odl = Odl.dlugosc();								// absolutna odleglosc dwoch punktow (skalar)
		r_pred = (p1->v - p2->v);	 // roznica vi

		f = (d_odl -  s_dlug) * roboczaS->spr + (r_pred * Odl) * C / d_odl;
	
		if(d_odl != 0)
		{	
			F = (Odl * (1.0f/d_odl));
			F = (F * f);
			
//			Kopiuj(Suma(F,p2->f),&(p2->f));
			p2->f = F + p2->f;
			p1->f = F * (-1) + p1->f;


//			Kopiuj(Suma(Mnozenie(F,-1),p1->f),&(p1->f));


		}

						   

	}



			  

}



Wektor W_Euler(Wektor f, float h)
{
//	return(Mnozenie(f,h));
	return f * h;
}



/*
	Dokladne kolizje sfera - sfera

pomys�: artykul:
"Pool Hall Lessons: Fast, Accurate Collision Detection between Circles or Spheres"
Je Van Den Heuvel and Miles Jackson
Gamasutra, 01.18.2002


zak�adamy, ze p1 sie rusza. potem transformujemy v do ukladu odniesienia w ktorym p2 spoczywa.

*/




void przyrost(Wektor *r, Wektor dr)
{
	(*r) = (*r) + dr;
}





void RozwiazEuler(Punkt *korzenP, float dt)
{
	Punkt *roboczyP;

	for(roboczyP=korzenP;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_ZAW))
	{

		

		roboczyP->dv = W_Euler(roboczyP->f * (1/roboczyP->masa),dt); 
		roboczyP->v = roboczyP->v + roboczyP->dv; 
		roboczyP->dr = roboczyP->v * dt; 

//		roboczyP->r = roboczyP->r + roboczyP->dr; 

		przyrost(&(roboczyP->r),roboczyP->dr);

//		Kopiuj(W_Euler(Mnozenie(roboczyP->f,1/roboczyP->masa),dt) , &(roboczyP->dv));
//		Kopiuj(Suma(roboczyP->v,roboczyP->dv) , &(roboczyP->v));
//		Kopiuj(W_Euler(roboczyP->v,dt) , &(roboczyP->dr));
//		Kopiuj(Suma(roboczyP->r,roboczyP->dr),&(roboczyP->r));

	}


}


/* Wpisuje tylko przyrosty dv i dr jako wektory do struktury punktu i dodaje dzielac na dwa 


	w polach przyrostow przechowujemy poprzednie wartosci 
*/


void RozwiazMidPoint_k1(Punkt *korzenP, float dt)
{
	Punkt *roboczyP;
	Wektor przech;

	for(roboczyP=korzenP;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_ZAW))
	{

		roboczyP->dv = W_Euler(roboczyP->f * (1/roboczyP->masa),dt); 
		roboczyP->dr = W_Euler(roboczyP->v + roboczyP->dv * 0.5f, dt); 


// przechowaj r w dr i v w dv
		przech = roboczyP->v;
		roboczyP->v = roboczyP->v + roboczyP->dv * 0.5;
		roboczyP->dv = przech;
 
		przech = roboczyP->r;

//		roboczyP->r = roboczyP->r + roboczyP->dr * 0.5;

		przyrost(&(roboczyP->r),roboczyP->dr * 0.5);

		roboczyP->dr = przech;

							  

	}
}




/* 

w polach dr i dv struktury Punkt funkcja MidPoint_k1 zostawila stare wartosci v i r, do ktorych dodamy
przyrosty obliczone z wartosci nowych wyliczonych przez k2 */


void RozwiazMidPoint_k2(Punkt *korzenP, float dt)
{
	Punkt *roboczyP;

	Wektor dr,dv;


	for(roboczyP=korzenP;roboczyP != NULL; roboczyP=roboczyP->prawy)
	if(!(roboczyP->flaga & P_ZAW))
	{

		dv = W_Euler(roboczyP->f * (1/roboczyP->masa),dt); 
		roboczyP->v = dv + roboczyP->dv; 
		dr = roboczyP->v * dt; 
		roboczyP->r = dr;


		przyrost(&(roboczyP->r),roboczyP->dr);


	}




}







