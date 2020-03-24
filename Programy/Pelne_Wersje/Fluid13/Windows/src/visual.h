
extern void DrawMesh(int width, int height, int nx, int ny, double dx, double dy);
extern void DrawBndCells(int width, int height, int nx, int ny, double dx, double dy, int** F);
extern void PlotParticles(struct particle *wrkpart2, int width, int height, double **table, int nx, int ny);
extern void PlotParticlesColor(struct particle *wrkpart2, int width, int height, int nx, int ny, int R, int G, int B);
extern void PlotVelocities(int width, int height, int nx, int ny, double dx, double dy, double **U, double **V, int** F);

extern void PlotParticlesColorObject(struct particle *wrkpart2, int width, int height, int nx, int ny, int R, int G, int B, int object);

