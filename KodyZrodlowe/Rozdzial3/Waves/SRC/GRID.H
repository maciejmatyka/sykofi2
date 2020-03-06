



#define NX 70	 
#define NY 70
#define VEL 2

// renderer
#define PX 0
#define PY 1
#define PZ 2

//	char blah[100];

// boundary flags

#define C_NORMAL  0x00000001
#define C_BND     0x00000002
#define C_SRC     0x00000004

#define C_VELHALF 0x00000008
#define C_BNDTEMP 0x000000010






class Grid
{


public:
	Grid()
	{
		int i,j,iw,jw;


		for(i=0;i<=NX+1;i++)
			for(j=0;j<=NY+1;j++)
				E[i][j]=U[i][j]=0.0f;





	// make boundaries

		for(i=0;i<=NX+1;i++)
			for(j=0;j<=NY+1;j++)
				Flag[i][j]=C_NORMAL;
/*
		for(i=2;i<=NX-2;i++)
		{
			   Flag[i][2] |=C_BND;
			   Flag[i][NY-2] |=C_BND;
		}

		for(j=2;j<=NY-2;j++)
		{
			   Flag[2][j] |=C_BND;
			   Flag[NX-2][j] |=C_BND;
		}
*/

			for(j=0;j<=NY+1;j++)



	// let's make young experiment


		for(jw=7;jw<8;jw++)
		{
			for(iw=-(NX/2);iw<=-2;iw++)
				Flag[NX/2+iw][NY/3+jw] |= C_BND;

			for(iw=2;iw<=(NX/2);iw++)
				Flag[NX/2+iw][NY/3+jw] |= C_BND;

			for(iw=-(NX/2);iw<=-10;iw++)
				Flag[NX/2+iw][NY/3+jw+25] |= C_BND;

			for(iw=10;iw<=(NX/2);iw++)
				Flag[NX/2+iw][NY/3+jw+25] |= C_BND;

			for(iw=-7;iw<=7;iw++)
				Flag[NX/2+iw][NY/3+jw+25] |= C_BND;
		}





	// place source cells

 		  	for(iw=-1;iw<1;iw++)
		  	for(jw=-1;jw<1;jw++)
				Flag[NX/2+iw][NY/4+jw] |= C_SRC;






	// clear work points for visualization

	point[PX]=point[PY]=point[PZ]=0.0f;
	point1[PX]=point1[PY]=point1[PZ]=0.0f;
	point2[PX]=point2[PY]=point2[PZ]=0.0f;



	}


	~Grid()
	{

	}
	

	void clearsources(void);

	void render3d(GLdouble width,GLdouble height, GLdouble aspect, int angle, int anglex, int angley, int anglez, float scale, float py);
	void render2d(int width, int height, float scale);
	void render2dlines(GLdouble width,GLdouble height, float scale);
 	void renderBnd(GLdouble width,GLdouble height);
 	void renderSrc(GLdouble width,GLdouble height);
 	void renderClear(void);

	void renderLine(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height);


	void bnd(void);

	void reset(void);
	void solvewave(float t, float DT, float DX, float DY, int sources);

	void addBnd(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height);
	void addBndTemp(float x1, float y1, float x2, float y2,GLdouble width,GLdouble height);
	void clearBndTemp(void);
	void changeSRC(float x, float y,GLdouble width,GLdouble height);


	float itoscreen(int i);
	float countE(int i, int j);
	void setEColour(int i, int j);


private:
	int i,j;

	float E[NX+1][NY+1];  // e biezace (wychylenie)
	float U[NX+1][NY+1];
	float Ess[NX+1][NY+1]; // e stare
	float Enn[NX+1][NY+1]; // e nowe
	float I[NX+1][NY+1];

	int Flag[NX+1][NY+1];
	

	float point[3];
	float point1[3];
	float point2[3];

};