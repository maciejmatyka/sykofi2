#define DTSTART 1.0/8
#define DTMAX DTSTART

/*struct DosBase *DosBase;
*/

struct particle *particle1;
struct particle *_particle1;
struct particle *particleundo;
struct particle *wrkpartile;





//int timestep(double dx, double dy, double gx, double gy, double vis,
//	     double t, double dt, int nx, int ny, int imax, int jmax,
//	     int width, int height, int **Flag, double **Un1, double **Vn1,
//	     double **U, double **V, double **P, double **D, double **R, double sta);
void CopyTable(double **T1, double **T2, int nx, int ny);
void CopyTableI(int **T1, int **T2, int nx, int ny);
void CopyParticles(struct particle *par1, struct particle *par2);
 
int teststability(double **U, double **V, int nx, int ny, double dx,
		  double dy, double dt, double vis, double sta);

int **TS_ANY;					// how much particles in a cell? (ReflagCells function)
double **TS_UKNK,**TS_VKNK;		// sum of particle velocities (ReflagCells function)
	

	int particleobjects;

    double dx, dy;
    double gx, gy;
    double vis;
    double savet;
    double t,dt,dtmax,dtstart;
    int nx, ny, imax, jmax, fwidth, fheight, BNum;


    int frame;
    int stability;

    int **Flag, **_Flag;
    double **Un1, **Vn1, **U, **V, **P, **D, **R;

    double **_Un1, **_Vn1, **_U, **_V, **_P, **_D, **_R;


    int *BPoints, *BCells;
    double sta;
    int gmode;

	char blah[100];

