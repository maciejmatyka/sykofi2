

#ifndef __I_KOLIZJE_H_
#define __I_KOLIZJE_H_

#include "wektory.h"
#include "punkty.h"

#define O_AKT 0
#define O_NAKT 1

extern float xmin,xmax,ymin,ymax,zmin,zmax;
extern float tlumienie;
extern float dt;

typedef struct Obszar Obszar;

struct Obszar 
{
	int flaga;			// O_AKT/O_NAKT  aktywny, nieaktywny (dla korzenia)

	Punkt *punkt;		// przypisany do punktu (ruchomy) jesli == NULL (nieruchomy)

	Wektor r1;			// polozenie
	float R;			// promien
	float tlum;			// wsp. tlumienia - 1.0 brak tlumienia

	float r,g,b;

	Obszar *prawy;
};






Obszar *AlokujObszar(float promien, Wektor r, float tlum, int flaga, Punkt *p);
void DodajObszar(Obszar *korzen, float promien, Wektor r, float tlum, int flaga, Punkt *p);

void Kolizje(Punkt *korzenP, Obszar *korzenO);
Wektor KolizjeObszary(Punkt *p, Wektor V);

//void Kolizje_SRtoSR(Obszar *korzenO);

#endif