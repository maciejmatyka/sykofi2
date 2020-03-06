
void CalculateSurface(double **Vn1, double **Un1, int **F, double **P,
		      double **D, double **U, double **V, int nx, int ny,
		      double gx, double gy, double dt, double dx,
		      double dy);
void ReflagCells(double **Vn1, int **F, double **P, double **U, double **V,
		 int nx, int ny, struct particle *firstparticle);
int AnyParticles(int i, int j, struct particle *firstparticle);
void ReflagTo(int **F, int i, int j, int flag);
int SumParticles(struct particle *firstparticle, int i, int j);
