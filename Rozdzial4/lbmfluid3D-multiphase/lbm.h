#ifndef _I_lbm_H_
#define _I_lbm_H_

void lbm(int c);
void initlbm();

const int q = 15;

const int W=50;
const int H=50;
const int D=50;

extern int F[][H][D];
extern float UX[][H][D];
extern float UY[][H][D];
extern float UZ[][H][D];
extern float R[][H][D];
extern const float w[q];
extern float omega;
#endif

