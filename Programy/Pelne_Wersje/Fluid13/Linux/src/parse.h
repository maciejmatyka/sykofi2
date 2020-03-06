void ParseFile(int argc, char *argv[],int *nx, int *ny, int *width, int *height, double *gx, double *gy, double *dt, double *vis, double *sta, int *gmode, int *BNum, int *BCells[], int *BPoints[]);

struct particle *ParticleFile(int argc, char *argv[], double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V,int verbose);

