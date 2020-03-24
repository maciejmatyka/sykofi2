
#include <iostream>
#include <complex>

using namespace std;

#define NX 90	 
#define NY 90
//#define VEL 2

// renderer
#define PX 0
#define PY 1
#define PZ 2

//	char blah[100];

// boundary flags

#define C_NORMAL  1
#define C_BND     2
#define C_SRC     4
#define C_BNDTEMP 8
#define C_VELHALF 16
#define C_POT 32


extern float DT;

class Grid
{
//	friend	complex<double>;

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



	// initialize wave packet
					
	SPaczki=(float)NX/(float)10;
	v=4;
	IloscIteracji=5;

/*
	int px = 4;		// 2 paczki
	int py = 30;
	
	

	double exp_w1,exp_w2;	// pierwsza paczka


	for(j=0;j<NY;j++)
	{
		exp_w1=pow(j-SPaczki-py,2);
//		exp_w3=pow(j-SPaczki-py2,2);		// dla drugiej paczki

		for(i=0;i<NX;i++)
		{
			exp_w2=pow(i-SPaczki-px,2);
			exp_w2=(-exp_w1-exp_w2)/pow(SPaczki,2);

			psi_t[i][j] = exp(complex<double>(exp_w2,i)); 
		}
	}
*/


	int px = 4;		// 2 paczki
	int py = 36;
	
	int px2 = 64;
	int py2 = 36;
	

	double exp_w1,exp_w2;	// pierwsza paczka
	double exp_w3,exp_w4;	// druga paczka


	for(j=0;j<NY;j++)
	{
		exp_w1=pow(j-SPaczki-py,2);
		exp_w3=pow(j-SPaczki-py2,2);		// dla drugiej paczki

		for(i=0;i<NX;i++)
		{
			exp_w2=pow(i-SPaczki-px,2);
			exp_w2=(-exp_w1-exp_w2)/pow(SPaczki,2);

			exp_w4=pow(i-SPaczki-px2,2);
			exp_w4=(-exp_w3-exp_w4)/pow(SPaczki,2);

			psi_t[i][j] = exp(complex<double>(exp_w2,i))+exp(complex<double>(exp_w4,-i)); 
		}
	}


    for(i=0;i<NX;i++)
		psi_t[i][0]=psi_t[i][NY]=complex<double>(0,0);

    for(j=0;j<NY;j++)
		psi_t[0][j]=psi_t[NX][j]=complex<double>(0,0);



	exp_vdt=exp(complex<double>(0,-DT*v));

  for(j=0;j<NY;j++)
	for(i=0;i<NX;i++)
	{
		c=abs(j-NY/2);
    if(i/2-c > 20 && i<60)
	{ 
		V[i][j]=2; 
		Flag[i][j] |= C_BND;
	}
	else  
		V[i][j]=1; 
	}

 
	clear_psi();


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




	void makemeE(void);

	void copy_psi(void);
	void psi_iteration(int i, int j);
	void clear_psi(void);


	void makebnda(void);

	void place_gauss(int i, int j);



// to i owo na kwantowo

	void solveshroedinger(float t, float DT, float DX, float DY, int sources);



	float itoscreen(int i);
	float countE(int i, int j);
	void setEColour(int i, int j);


private:

	complex<double> psi[NX+1][NY+1];
	complex<double> psi_t[NX+1][NY+1];
	complex<double> exp_vdt;

	int i,j;

	double z1,z2,pr; // render

//	double ami;
	double ai,ar;
	int n,m;
	double kw;
	double potencjal, IloscIteracji, tmax;
	double Amplituda,zp,SPaczki,c;
	int	n2;


	float E[NX+1][NY+1];  // e biezace (wychylenie)
	float U[NX+1][NY+1];
	float Ess[NX+1][NY+1]; // e stare
	float Enn[NX+1][NY+1]; // e nowe
	float I[NX+1][NY+1];

	int Flag[NX+1][NY+1];


	int v;
	float V[NX+1][NY+1];

	float point[3];
	float point1[3];
	float point2[3];

};



