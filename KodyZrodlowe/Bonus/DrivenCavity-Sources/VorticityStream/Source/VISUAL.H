void PlotVelocities(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, int scale);
void PlotParticles(int nx, int ny, double **);


void PlotThreeFields(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale);
void PlotOneField(int width, int height, int nx, int ny, double dx, double dy, double **f, int scale);
void PlotOneSmoothField(int width, int height, int nx, int ny, double dx, double dy, double **f, int scale);
void PlotThreeFieldsSmooth(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale);
void PlotStreamlines(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale);
void PlotStreamlinesSmear(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale);


void PlotStreamlinesRandom(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale);
