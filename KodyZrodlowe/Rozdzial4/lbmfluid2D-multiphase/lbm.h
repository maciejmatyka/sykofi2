#ifndef _I_lbm_H_
#define _I_lbm_H_

void savevdf(int v);

float tortuosity(void);
float Reynolds(void);
void lbm(void);
void initlbm();
void initbnd(int m);        // boundaries
void exportvtk(void);
void exportprofile(void);
#define LX 200
#define LY 200

extern int F[LX][LY];
extern float U[LX][LY];
extern float R[LX][LY];
extern float V[LX][LY];
extern float df[2][LX][LY][9];
extern const float w[9];
extern float omega;
extern float fx;
#endif

