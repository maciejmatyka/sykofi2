void drawpar(int mode);
void initpar();
void movepar(double dt);
void moveparinterpolate(double dt);

const int NPARMAX = 256*4096;
extern float mnoznik_alpha;

extern int npar;			// num of particles
void initrendervbo(void);
extern double probrand_part;
