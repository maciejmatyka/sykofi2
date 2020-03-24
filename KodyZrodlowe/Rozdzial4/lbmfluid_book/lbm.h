#ifndef _I_lbm_H_
#define _I_lbm_H_

void savevdf(int v);

float tortuosity(void);
void lbm(void);
void initlbm();
#define LX 300
#define LY 75
#define L0 LY		// wymiar charakterystyczny
const int W = LX/L0;		// bezwymiarowa szerokość
const int H = LY/L0;

extern int F[][LY];
extern float U[][LY];
extern float R[][LY];
extern float V[][LY];
extern float df[2][LX][LY][9];
extern const float w[9];
extern float omega;
extern float fx;
#endif

