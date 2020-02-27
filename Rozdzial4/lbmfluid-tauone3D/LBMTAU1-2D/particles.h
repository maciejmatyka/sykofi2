
void drawpar();
void drawpar(int mode);
void initpar();
void addpar(int n);
void movepar(double dt, int c);
void initparprobability(int c);
void initparprobabilityINVERSE(int c);
void initparALL(void);

extern int npar;
const int NPARMAX = 20*4096;			// num of particles
void initrendervbo(void);
extern float probrand_part;

extern float mnoznik_alpha;

