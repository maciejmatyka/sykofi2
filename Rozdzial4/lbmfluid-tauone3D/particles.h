
void drawpar();
void drawpar(int mode);
void initpar();
void movepar(float dt, int c);

extern int npar;
const int NPARMAX = 20*4096;			// num of particles
void initrendervbo(void);
extern float probrand_part;

extern float mnoznik_alpha;

