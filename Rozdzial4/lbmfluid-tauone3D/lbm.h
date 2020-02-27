#ifndef _I_lbm_H_
#define _I_lbm_H_

void LBMTAU1(int c);
void initlbm();

const int q = 15;

#define LX 80
#define LY 40
#define LZ 40

extern int F[][LY][LZ];
extern float U[2][LX][LY][LZ];
extern float V[2][LX][LY][LZ];
extern float X[2][LX][LY][LZ];
extern float R[2][LX][LY][LZ];
extern const float w[q];
extern float omega;
#endif

