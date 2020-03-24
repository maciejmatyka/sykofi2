


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "punkty.h"
#include "kolizje.h"



#define epsilon 0.02f


extern Obszar *KorzenObszarow;

void Kolizje(Punkt *korzenP, Obszar *korzenO)
{
	Punkt *roboczyP;
	Obszar *roboczyO;
	Obszar *roboczyO2;



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




	for(roboczyP=korzenP;roboczyP != NULL; roboczyP=roboczyP->prawy)

	if(roboczyP->flaga & P_NORM)
	{


		if(roboczyP->r.x > xmax)
		{
			roboczyP->r.x=xmax;
			roboczyP->v.x=roboczyP->v.x * (-tlumienie);
		}

		if(roboczyP->r.y > ymax)
		{
			roboczyP->r.y=ymax;
			roboczyP->v.y=roboczyP->v.y * (-tlumienie);
		}


		if(roboczyP->r.z > zmax)
		{
			roboczyP->r.z=zmax;
			roboczyP->v.z=roboczyP->v.z * (-tlumienie);
		}



		if(roboczyP->r.x < xmin)
		{
			roboczyP->r.x=xmin;
			roboczyP->v.x=roboczyP->v.x * (-tlumienie);
		}

		if(roboczyP->r.y < ymin)
		{
			roboczyP->r.y=ymin;
			roboczyP->v.y=roboczyP->v.y * (-tlumienie);
		}

		if(roboczyP->r.z < zmin)
		{
			roboczyP->r.z=zmin;
			roboczyP->v.z=roboczyP->v.z * (-tlumienie);
		}


// punkt o promieniu R i sfera

/*
		double d,z;

		for(roboczyO=korzenO;roboczyO != NULL; roboczyO=roboczyO->prawy)
	  	if(!(roboczyO->flaga & O_NAKT) && !roboczyO->punkt && (!(roboczyP->flaga & P_OBSZ)))									// obszar ruchomy - pomijaj
		{
			d=(roboczyO->r1 - roboczyP->r).dlugosc() + roboczyP->R;
			if(d - roboczyO->R > 0)
			{
				n = (roboczyO->r1 - roboczyP->r);
				n.normuj();

  // przesun
			
				z = d - roboczyO->R;
			   	roboczyP->r = roboczyP->r + n * z;

// v
  				Vn = n * (n * roboczyP->v);
			  	Vs = roboczyP->v - Vn;

			   	roboczyP->v = Vs - Vn;
  
			    roboczyP->v = roboczyP->v * (roboczyO->tlum);
			}
	 	}


*/	 

		for(roboczyO=korzenO;roboczyO != NULL; roboczyO=roboczyO->prawy)
	  	if(!(roboczyO->flaga & O_NAKT) && !roboczyO->punkt && (!(roboczyP->flaga & P_OBSZ)))									// obszar ruchomy - pomijaj
			if( (roboczyO->r1 - roboczyP->r).dlugosc() - roboczyO->R < epsilon)
			{
				n = (roboczyO->r1 - roboczyP->r);
				n.normuj();

  				Vn = n * (n * roboczyP->v);
			  	Vs = roboczyP->v - Vn;

			   	roboczyP->v = Vs - Vn;
  
			   	roboczyP->r = roboczyP->r + roboczyP->v * dt *2;
			    roboczyP->v = roboczyP->v * (roboczyO->tlum);
			}
	 

	}
 
	// kolizje punkt - sfera kolizji


//	} // petla po punktach








/*
	Dokladne kolizje sfera - sfera

pomys¦: artykul:
"Pool Hall Lessons: Fast, Accurate Collision Detection between Circles or Spheres"
Je Van Den Heuvel and Miles Jackson
Gamasutra, 01.18.2002


zak¦adamy, ze p1 sie rusza. potem transformujemy v do ukladu odniesienia w ktorym p2 spoczywa.

*/


	/*
	jesli p2->v nie jest rowne zeru to

		1. pamietamy V2=p2->v
		2. przechodzimy do ukladu, gdzie p2 spoczywa (-V2)
		3. rozwiazujemy kolizje
		4. wracamy do naszego ukladu odniesienia, (+V2)
	*/

	#define O1 roboczyO
	#define O2 roboczyO2
	#define p1 roboczyO->punkt
	#define p2 roboczyO2->punkt

	#define m1 p1->masa
	#define m2 ((p2) ? (p2->masa) : (p1->masa)) // jak nieruchome to odbijaj zawsze tak samo
	#define v2 ((p2) ? (p2->v) : (Wektor(0,0,0))  ) // predkosc drugiego punktu
			 

	Wektor dr;			// roznica wektorow p1 i p2
	Wektor V;				// wektor przesuniecia V = v * dt
	Wektor nV;				// normalny wektor przesuniecia dV
	double nVdr;			// nV * dp - odleglosc srodka p1 do npunktu najblizszego B na wektorze V
	double F,T;

	Wektor V2;				// predkosc drugiego punktu

//	Wektor n;		// odbicie
//	Wektor Vn2;		// skladowa normalna wektora predkosci

	float w1,w2,w3,w4;

	for(O1=korzenO;O1 != NULL; O1=O1->prawy)
	 if(O1->flaga & O_AKT)
		for(O2=korzenO;O2 != NULL; O2=O2->prawy)
		 if(O2->flaga & O_AKT)
			if(p1 && p2 && (p1 != p2))		// obszar ruchomy i nie koliduje sam ze soba
			{
					   					  
				dr = O1->r1 - O2->r1;

				V = (p1->v - v2) * (dt) * 2;

				if(V.dlugosc() >= dr.dlugosc() - O1->R - O2->R)	// kolizja w nast. kroku czasowym?
			 	if(V * dr <= 0)									// przesuniecie w odp. kierunku
			 	{					
					nV = V;
					nV.normuj();
					nVdr = nV * dr;
								
					F = dr.dlugosc() * dr.dlugosc() - nVdr*nVdr;

					if(F <= (O1->R + O2->R) * (O1->R + O2->R))
					{

						T = (O1->R + O2->R) * (O1->R + O2->R) - F;
					   	
					if(dr.dlugosc() > (nVdr - sqrt(T))*dt)
					{

						p1->r = p1->r + ( nV * (nVdr - sqrt(T)) )*dt;

						O1->r1 = p1->r;
			

					// wyznacz normalna

						n=dr;
						n.normuj();
						   
						n2=n * (-1);


					// zostaw styczne, pamietaj normalne skladowe v	

						Vn = n * (n * p1->v);
						Vn2 = n2 * (n2 * p2->v);

						p1->v = p1->v - Vn;		  // zostaje styczna bez zmian
						p2->v = p2->v - Vn2;		  // zostaje styczna bez zmian
  
//						Vn = Vn * (m1 / (m1 + m2));
 
							w1 = (m1 - m2) / (m1 + m2);
							w2 = 2 * m2 / (m1 + m2);
							w3 = 2 * m1 / (m1 + m2);
							w4 = (m2 - m1) / (m1 + m2);
							
							p1->v = p1->v + (Vn * w1 + Vn2 * w2);
							p2->v = p2->v + (Vn * w3 + Vn2 * w4);

					//    p1->r = p1->r - p1->v * dt; // przesun	 
					//    p2->r = p2->r - p2->v * dt; // przesun	 


							p1->v  = p1->v * O2->tlum;
							p2->v  = p2->v * O1->tlum;


	  				}
					} // F > (O1->R + O2->R) * (O1->R + O2->R)
			 	} // dp * v


//						p1->r = p1->v * dt;
//					    p1->r = p1->r + nV * ((nVdr - sqrt(T))); // przesun	 

			}
			else if(p1 && (p1 != p2))
			{

				dr = O1->r1 - O2->r1;

				V = (p1->v)* (dt);

				if(V.dlugosc() >= dr.dlugosc() - O1->R - O2->R)	// kolizja w nast. kroku czasowym?
			 	if(V * dr <= 0)									// przesuniecie w odp. kierunku
			 	{					
					nV = V;
					nV.normuj();
					nVdr = nV * dr;
								
					F = dr.dlugosc() * dr.dlugosc() - nVdr*nVdr;

					if(F <= (O1->R + O2->R) * (O1->R + O2->R))
					{

						T = (O1->R + O2->R) * (O1->R + O2->R) - F;
					   	
					if(dr.dlugosc() > (nVdr - sqrt(T))*dt)
					{

						p1->r = p1->r + ( nV * (nVdr - sqrt(T)) )*dt;
						O1->r1 = p1->r;
			

					// wyznacz normalna

						n=dr;
						n.normuj();
						   
					// zostaw styczne, pamietaj normalne skladowe v	

						Vn = n * (n * p1->v);

						p1->v = p1->v - Vn;		  // zostaje styczna bez zmian
						p1->v = p1->v - Vn;

						p1->v  = p1->v * O2->tlum;

	  				}
					} // F > (O1->R + O2->R) * (O1->R + O2->R)
			 	} // dp * v




			} // petla



 
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

