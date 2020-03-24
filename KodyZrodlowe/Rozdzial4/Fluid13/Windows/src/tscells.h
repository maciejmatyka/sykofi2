
extern void CalculateSurface(double **Vn1, double **Un1, int **F, double **P,
		      double **D, double **U, double **V, int nx, int ny,
		      double gx, double gy, double dt, double dx,
		      double dy);
extern void ReflagCells(double **Vn1, int **F, double **P, double **U, double **V,
		 int nx, int ny, struct particle *firstparticle);
extern int AnyParticles(int i, int j, struct particle *firstparticle);
extern void ReflagTo(int **F, int i, int j, int flag);
extern int SumParticles(struct particle *firstparticle, int i, int j);
double SumParticlesU(struct particle *firstparticle, int i, int j);
double SumParticlesV(struct particle *firstparticle, int i, int j);

