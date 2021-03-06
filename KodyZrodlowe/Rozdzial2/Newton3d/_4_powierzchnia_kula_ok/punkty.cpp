



#include "stdio.h"
#include "stdlib.h"

#include "punkty.h"
#include "wektory.h"



// funkcja PrzesunNaKorzeniu - przesuwa wskaznik na n-ty element w liscie i zwraca jego a(dres)
// zwraca to co dostal +1, jesli blad
Punkt *PrzesunNaKorzeniu(Punkt *korzen,int n)
{
	Punkt *Roboczy;
	int i;	 

	Roboczy = korzen;

	for(i=0;i<n;i++)
	if(Roboczy->prawy)
		Roboczy=Roboczy->prawy;
	else
		return(korzen);
	  

	return Roboczy;
}


// funkcja Ostatni - zwraca wskaznik na  opstatni obiekt w liscie punktow
Punkt *Ostatni(Punkt *korzen)
{
	Punkt *Roboczy;
		  
	for(Roboczy=korzen;Roboczy->prawy != NULL; Roboczy=Roboczy->prawy);
		  
	return Roboczy;
}


Punkt *AlokujPunkt(float m, int flaga, Wektor r,Wektor v, double R)
{
	Punkt *RoboczyPunkt;


	if (!(RoboczyPunkt = (Punkt *) malloc((unsigned) sizeof(Punkt)))) 
	{
		exit(0);
    }



	RoboczyPunkt->masa=m;
	RoboczyPunkt->flaga=flaga;

	RoboczyPunkt->r = r;
	RoboczyPunkt->R = R;
	RoboczyPunkt->v = v;


	RoboczyPunkt->col=rand()/(float)RAND_MAX;


//	Kopiuj(r,&(RoboczyPunkt->r));
//	Kopiuj(v,&(RoboczyPunkt->v));
////	Kopiuj(f,&(RoboczyPunkt->f));

	RoboczyPunkt->prawy=NULL;

	return (RoboczyPunkt);
}


Sprezyna *AlokujSprezyne(float c, float dlugosc, float spr, Punkt *p1, Punkt *p2)
{
	Sprezyna *RoboczaSprezyna;


	if (!(RoboczaSprezyna = (Sprezyna *) malloc((unsigned) sizeof(Sprezyna)))) 
	{
		exit(0);
    }

	RoboczaSprezyna->c=c;

	if(!dlugosc)
	RoboczaSprezyna->dlugosc=(p1->r-p2->r).dlugosc()/1.2;
	RoboczaSprezyna->spr=spr;


	RoboczaSprezyna->punkt1=p1;
	RoboczaSprezyna->punkt2=p2;

	RoboczaSprezyna->prawy=NULL;

	return (RoboczaSprezyna);
}



void DodajPunkt(Punkt *korzen, float m, int flaga,Wektor r, Wektor v, double R)
{
	Punkt *Roboczy;

	for(Roboczy=korzen;Roboczy->prawy != NULL; Roboczy=Roboczy->prawy);

	Roboczy->prawy=AlokujPunkt(m,flaga,r,v,R);

	
}

void DodajSprezyne(Sprezyna *korzen, float c, float dlugosc, float spr, Punkt *p1, Punkt *p2)
{
	Sprezyna *Robocza;

	for(Robocza=korzen;Robocza->prawy != NULL; Robocza=Robocza->prawy);

	Robocza->prawy=AlokujSprezyne(c,dlugosc,spr,p1,p2);
}


