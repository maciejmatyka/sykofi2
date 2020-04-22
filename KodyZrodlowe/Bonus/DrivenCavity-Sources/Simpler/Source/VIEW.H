void V_ClearRect(int x0, int y0, int x1, int y1);
void V_ClearRect(int x0, int y0, int x1, int y1, int col);
void V_Line(int x0, int y0, int x1, int y1);

void V_Point(int x, int y, int r, int g, int b);
void V_PointCross(int x, int y, int r, int g, int b);
void V_PointSquare(int x, int y, int r, int g, int b);
void V_PointSmooth(int x, int y, int r, int g, int b);
int V_GetPointColorR(int x, int y);


void V_OpenView(int sizex, int sizey);
void V_CloseView(void);
int V_CheckExitTrue(void);

int V_Write(int frame);
