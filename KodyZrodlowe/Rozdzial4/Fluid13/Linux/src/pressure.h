void CalculateDij(int **F, double **D, double **U, double **V, double dx,
		  double dy, int nx, int ny);
void PressureSource(int **F, double **U, double **V, double **D,
		    double **R, int nx, int ny, double dx, double dy,
		    double vis, double dt);
int CalculatePressure(int **F, double **R, double **P, int nx, int ny,
		      double dx, double dy, double gx, double gy,
		      double vis, double **U, double **V);
