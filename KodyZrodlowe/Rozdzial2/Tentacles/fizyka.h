

#include "punkty.h"
#include "wektory.h"
#include "kolizje.h"
#include "cloth.h"

#ifndef __I_FIZYKA
#define __I_FIZYKA



// numeryka

Wektor W_Euler(Wektor , float );

void RozwiazEuler(Punkt *korzenP, float dt);
void RozwiazMidPoint(Punkt *korzenP, float dt);
void RozwiazEuler(Cloth &cloth, float dt);


void RozwiazMidPoint_k1(Cloth &cloth, float dt);
void RozwiazMidPoint_k2(Cloth &cloth, float dt);

void PrzesunObszary(Obszar *korzenO);


// fizyka

void WyznaczSily(Punkt *korzenP, Sprezyna *korzenS, Obszar *korzenO);
void WyznaczSily(Cloth &cloth, float dt);
void CCC_Springs(Cloth &cloth);
Wektor PrzyrostEulera(Punkt *point,float dt);


extern Wektor Graw;
extern float tlumienie;




#endif
