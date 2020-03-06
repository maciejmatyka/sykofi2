
void ParseFile(int argc, char *argv[],int *nx, int *ny, int *width, int *height, double *gx, double *gy, double *dt, double *vis, double *sta, int *gmode, int *BNum, int *BCells[], int *BPoints[]);

struct particle *ParticleFile(int argc, char *argv[], double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V,int verbose);


void mfscanf(FILE *fp,char workbuf2[],char workbuf[]);




#define MAXWORD 80

void MoveNextWord(FILE *fp, char *word);
int CheckNextWord(FILE *fp, char *word);
void ISetValue(FILE *fp, int *itoset);
void ISetBndValue(FILE *fp, int table[], int i, int NX, int NY);
void IReadValue(FILE *fp, char *word, int *placehere);

void FReadValue(FILE *fp, char *word, double *placehere);
void FSetValue(FILE *fp, double *ftoset);


char *posserr;

