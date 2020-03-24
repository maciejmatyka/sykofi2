
#define P_REG   2			/*	a regular particle defining the fluid within the system */
#define P_INPUT 4			/*	particle falling within an B_IN cell. (every time step
                        	changed to P_REG and created one new P_INPUT behind it) */
#define P_AVAIL 8     /*	nonexistent in fluid particle (outsided e.g.) available
													for use. */

struct particle
{
	int object,flag,i,j;				/* i,j calculated from (x,y) while particle allocating */
	double uk,vk,x,y;
	struct particle *next;
};
/*
extern struct particle *AllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk, double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
extern struct particle *AllocParticleBall(int x0, int y0, int r, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
extern void   NegAllocParticleBall(int x0, int y0, int r, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
extern void   NegAllocParticleBox(int x0, int y0, int x1, int y1, double partstep, double Uk ,double Vk, struct particle *wrkpart, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);
*/
extern struct particle *AllocOneParticle(double px, double py, int P_Flag, int i, int j, double Uk, double Vk);

/*struct particle *ParticleSetup16(double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);*/
extern void FreeAllParticles(struct particle *firstparticle);
extern void ParticleMovement(struct particle *firstparticle, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V, double dt);

struct particle *PlaceParticle(double x, double y, struct particle *wrkpart, double Uk, double Vk, double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V);

