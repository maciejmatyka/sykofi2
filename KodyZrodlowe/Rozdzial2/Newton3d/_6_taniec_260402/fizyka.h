

#include "punkty.h"
#include "wektory.h"
#include "kolizje.h"


#ifndef __I_FIZYKA
#define __I_FIZYKA



// numeryka

Wektor W_Euler(Wektor , float );

void RozwiazEuler(Punkt *korzenP, float dt);
void RozwiazMidPoint(Punkt *korzenP, float dt);


void RozwiazMidPoint_k1(Punkt *korzenP, float dt);
void RozwiazMidPoint_k2(Punkt *korzenP, float dt);

void PrzesunObszary(Obszar *korzenO);


// fizyka

void WyznaczSily(Punkt *korzenP, Sprezyna *korzenS, Obszar *korzenO);


extern Wektor Graw;
extern float tlumienie;




#endif