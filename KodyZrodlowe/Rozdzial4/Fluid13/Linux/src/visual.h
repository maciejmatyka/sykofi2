void DrawMesh(int width, int height, int nx, int ny, double dx, double dy);
void DrawBndCells(int width, int height, int nx, int ny, double dx, double dy, int** F);
void PlotParticles(struct particle *wrkpart2, int width, int height, double **table, int nx, int ny);
void PlotParticlesColor(struct particle *wrkpart2, int width, int height, int nx, int ny, int R, int G, int B);
void PlotVelocities(int width, int height, int nx, int ny, double dx, double dy, double **U, double **V, int** F);

