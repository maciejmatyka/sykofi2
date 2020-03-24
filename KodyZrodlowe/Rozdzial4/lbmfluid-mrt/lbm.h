#ifndef _I_lbm_H_
#define _I_lbm_H_

void lbm(void);
void initlbm();
#define L 600

//const float U0 = 0.24;
extern double tau;
extern float U0;
void updateRe(float _re);
float ReynoldsNumber();

extern float goalRe;

void savevdf(int);


extern int FLAG[];
extern double U[];
extern double R[];
extern double V[];
extern double df[2][L*L][9];
extern const double w[9];
extern double omega;

float volumeflux2d(void);
float volumeflux(void);

#endif

