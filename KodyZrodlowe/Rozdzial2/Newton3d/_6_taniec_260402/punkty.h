
#include "wektory.h"
#include "kolizje.h"

#ifndef __I_PUNKTY_
#define __I_PUNKTY_

typedef struct Punkt Punkt;
typedef struct Sprezyna Sprezyna;
typedef struct Obszar Obszar;

#define P_NORM 1			// normalny - fizyka dziala
#define P_ZAW 2				// nie rozwiazuje rownan rochu dla ciala (polozenie = const)
#define P_OBSZ 4             // podczepiony opbszar kolizji
#define P_NRYS 8			// nie rysuj
#define P_WEKT 16           // dolacz wektor



#define S_OK 1
#define S_PEK 2
#define S_NRYS 4
#define S_RYSL 8


struct Punkt 
{
		float masa;
		int flaga;
	
		Wektor r;
		Wektor v;
		Wektor f;

		Wektor dr;
		Wektor dv;

		double R;

		double col;

		Punkt *prawy;
};


struct Sprezyna
{

	Punkt *punkt1;
	Punkt *punkt2;
	
	float ks;      // stala sprezystosci
	float kd;	    // stala wygaszania drgan
	float dlugosc;	// dlugosc sprezyny 

	int flag;

	Sprezyna *prawy;

};


Punkt *AlokujPunkt(float , int , Wektor ,Wektor, double);
Sprezyna *AlokujSprezyne(float , float , float , Punkt *, Punkt *, int);
void DodajPunkt(Punkt *, float, int,Wektor , Wektor, double);
void DodajSprezyne(Sprezyna *korzen, float c, float dlugosc, float spr, Punkt *p1, Punkt *p2, int);

Punkt *Ostatni(Punkt *korzen);
Punkt *PrzesunNaKorzeniu(Punkt *korzen,int n);

#endif