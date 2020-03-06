

extern void V_DIV2Point(int x, int y, int r, int g, int b);
extern void V_Point(int x, int y, int r, int g, int b);
extern void V_PointCross(int x, int y, int r, int g, int b);
extern void V_ClearRect(int x0, int y0, int x1, int y1);
extern void V_Line(int x0, int y0, int x1, int y1);
extern void V_OpenView(int sizex, int sizey);
extern void V_CloseView(void);
extern int V_CheckExitTrue(void);
extern void V_Write(int frame);
extern void V_WriteJpeg(int frame);
extern void OutputFrameGL(void);
extern int V_Red(int y,int x);
extern int V_Green(int y,int x);
extern int V_Blue(int y,int x);






