void CellSetup15(int** F, int BPoints[], int BNum, int width, int height, int nx, int ny, double dx, double dy);
void ShowBoundary(int** F,int nx, int ny);
int CellPosX(double dx, int i);
int CellPosY(double dy, int j);
void BundaryCellsToPoints(int BCells[], int BPoints[], int BNum, int width, int height, double dx, double dy);

void	CalculateBNDConditions(double **Un1, double **Vn1,int **F, double **U, double **P, double **V, double **D, int nx, int ny, double gx, double gy, double vis, double dx, double dy);
