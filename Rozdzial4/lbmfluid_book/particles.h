void drawpar(int mode);
void initpar();
void movepar(double dt);
void moveparinterpolate(double dt);

const int NPARMAX = 6*4096;
extern float mnoznik_alpha;

extern int npar;			// num of particles
extern int PSIZ;
void initrendervbo(void);
extern double probrand_part;
