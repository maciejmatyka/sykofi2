#ifndef _I_lbm_H_
#define _I_lbm_H_

void LBMTAU1(int c);
void initlbm();
float ReynoldsNumber(int c);

#define LX 400
#define LY 200
#define L0 (LY/10)       	// wymiar charakterystyczny
const int W = LX/L0;   	// bezwymiarowa szerokość
const int H = LY/L0;

extern int F[][LY];
extern float U[2][LX][LY];
extern float V[2][LX][LY];
extern float R[2][LX][LY];
extern const float w[9];
extern float omega;
#endif

