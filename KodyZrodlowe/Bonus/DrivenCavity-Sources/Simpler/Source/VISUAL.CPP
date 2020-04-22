
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#include "view.h"
#include "minmax.h"

#define COLMAX 255

void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	double t;

	for(t=0;t<1;t+=0.01)
	 V_PointSmooth((int) ((1-t)*x1+t*x2),(int) ((1-t)*y1+t*y2), r, g, b);
//	 V_Point((int) ((1-t)*x1+t*x2),(int) ((1-t)*y1+t*y2), r, g, b);

}







// streamlines




void PlotStreamlinesSmear(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale)
{
	width=width*scale;
	height=height*scale;
	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	int i,j;
	double colu;
	double colv;
	double x01,y01;

	double **InterpolatedU;
	InterpolatedU = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedU[x]=new double[height];
	double **InterpolatedV;
	InterpolatedV = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedV[x]=new double[height];


	double s;
	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
		
		i=(int)((double)x/DX);
		j=(int)((double)y/DY);
		
		x01 = ((double)x/DX) - i;
		y01 = ((double)y/DY) - j;
	
		
		colu =  (1-y01)  *( (1-x01)*u[i][j] + x01*u[i+1][j]  );
		colu += (y01)    *( (1-x01)*u[i][j+1] + x01*u[i+1][j+1]  );
		colv =  (1-y01)  *( (1-x01)*v[i][j] + x01*v[i+1][j]  );
		colv += (y01)    *( (1-x01)*v[i][j+1] + x01*v[i+1][j+1]  );

	//s=1;
		s=sqrt(colu*colu + colv*colv);
			if(s!=0)
			{
				InterpolatedU[x][y]=colu/s;
				InterpolatedV[x][y]=colv/s;
			}
		}

// puszczamy (pawia :) particla
		double px,py;
		double pu,pv;

		double lx,ly;
		int lc;


	for(int krowa2=0;krowa2<width-DX*2;krowa2+=DX*0.9)
	{
		for(int krowa=0;krowa<height-DY*2;krowa+=DY*0.9)
		{
			px=krowa2; //width/2+1;
			py=krowa;//height/4;
	
			int k=0;
			while(k++<2500)// && px > 0 && py>0 && px<width-1 && py<height-1)
			{
	
				if(px<0)
					px = width + px;//-DX*2;
				if(py<0)
					py = height + py;//-DY*2;
				if(py>height)
					py = py-height -1;
				if(px>width)
					px = px-width - 1;
	

				lx=px;
				ly=py;

				pu=InterpolatedU[(int)px][(int)py];
				pv=InterpolatedV[(int)px][(int)py];

		//		s=sqrt(pu*pu+pv*pv);

				//if(s!=0);
				//pu /= s;
				//pv /= s;
	
				px=px+pu*0.02;//*10.001;
				py=py+pv*0.02;//*10.001;


				lc=V_GetPointColorR(lx,ly);
	//			if(rand()/(float)RAND_MAX > 0.7)
	//				V_Point((int)(lx+(px-lx)),(int)(ly+(py-ly)),lc,lc,lc);
		//		if(rand()/(float)RAND_MAX > 0.8)
		//			V_Point((int)(lx+(lx-px)),(int)(ly+(ly-py)),lc,lc,lc);
				
			//	cout << px << "," << py << endl;
	
				V_PointSmooth((int)px,(int)py, 255, 255, 255);
			}
		}
		
//		cout << "finish row: " << krowa2 << " of " <<  width-DX*2.5 << " rows." << endl;
		
		}
		





	 for (x=0; x<width; x++)
	  {
	    free(InterpolatedU[x]);
	    InterpolatedU[x]=NULL;
	  }
	
	 for (x=0; x<width; x++)
	  {
	    free(InterpolatedV[x]);
	    InterpolatedV[x]=NULL;
	  }
	
	  free(InterpolatedU);
	  InterpolatedU=NULL;
	  free(InterpolatedV);
	  InterpolatedV=NULL;
}



void PlotStreamlines(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale)
{
	width=width*scale;
	height=height*scale;
	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	int i,j;
	double colu;
	double colv;
	double x01,y01;

	double **InterpolatedU;
	InterpolatedU = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedU[x]=new double[height];
	double **InterpolatedV;
	InterpolatedV = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedV[x]=new double[height];



	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
		
		i=(int)((double)x/DX);
		j=(int)((double)y/DY);
		
		x01 = ((double)x/DX) - i;
		y01 = ((double)y/DY) - j;
			

		colu =  (1-y01)  *( (1-x01)*u[i][j] + x01*u[i+1][j]  );
		colu += (y01)    *( (1-x01)*u[i][j+1] + x01*u[i+1][j+1]  );
		colv =  (1-y01)  *( (1-x01)*v[i][j] + x01*v[i+1][j]  );
		colv += (y01)    *( (1-x01)*v[i][j+1] + x01*v[i+1][j+1]  );
		InterpolatedU[x][y]=colu;
		InterpolatedV[x][y]=colv;

		}

// puszczamy (pawia :) particla
		double px,py;


	for(int krowa2=0;krowa2<width;krowa2+=DX)
	for(int krowa=0;krowa<height;krowa+=DY)
	{
		px=krowa2; //width/2+1;
		py=krowa;//height/4;

		int k=0;
		while(k++<500)// && px > 0 && py>0 && px<width-1 && py<height-1)
		{

			if(px<0)
				px = width + px-DX*2;
			if(py<0)
				py = height + py-DY*2;
			if(py>height)
				py = py-height;
			if(px>width)
				px = py-width;

			px=px+InterpolatedU[(int)px][(int)py]*0.9;
			py=py+InterpolatedV[(int)px][(int)py]*0.9;

		//	cout << px << "," << py << endl;

			V_PointSmooth((int)px,(int)py, 255, 255, 255);
		}
	}
	
	
}






void PlotStreamlinesRandom(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale)
{
	width=width*scale;
	height=height*scale;
	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	int i,j;
	double colu;
	double colv;
	double x01,y01;

	double **InterpolatedU;
	InterpolatedU = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedU[x]=new double[height];
	double **InterpolatedV;
	InterpolatedV = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedV[x]=new double[height];


	double s;
	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
		
		i=(int)((double)x/DX);
		j=(int)((double)y/DY);
		
		x01 = ((double)x/DX) - i;
		y01 = ((double)y/DY) - j;
			

		colu =  (1-y01)  *( (1-x01)*u[i][j] + x01*u[i+1][j]  );
		colu += (y01)    *( (1-x01)*u[i][j+1] + x01*u[i+1][j+1]  );
		colv =  (1-y01)  *( (1-x01)*v[i][j] + x01*v[i+1][j]  );
		colv += (y01)    *( (1-x01)*v[i][j+1] + x01*v[i+1][j+1]  );

	//s=1;
		s=sqrt(colu*colu + colv*colv);
			if(s!=0)
			{
				InterpolatedU[x][y]=colu/s;
				InterpolatedV[x][y]=colv/s;
			}
		}

// puszczamy (pawia :) particla
		double px,py;
		double pu,pv;

		double lx,ly;
		int lc;


	int ile=(height-DY*2)*(width-DX*2)*0.1;

	for(int krowa=0;krowa<ile;krowa+=DY)
	{
		px=(rand()/(double)(RAND_MAX))*(width-DX*2); //krowa2; //width/2+1;
		py=(rand()/(double)(RAND_MAX))*(height-DY*2);//height/4;
	
			int k=0;
			while(k++<3200)// && px > 0 && py>0 && px<width-1 && py<height-1)
			{
	
				if(px<0)
					px = width + px;//-DX*2;
				if(py<0)
					py = height + py;//-DY*2;
				if(py>height)
					py = py-height;
				if(px>width)
					px = px-width;
	

				lx=px;
				ly=py;

				pu=InterpolatedU[(int)px][(int)py];
				pv=InterpolatedV[(int)px][(int)py];

		//		s=sqrt(pu*pu+pv*pv);

				//if(s!=0);
				//pu /= s;
				//pv /= s;
	
				px=px+pu*0.1;//*10.001;
				py=py+pv*0.1;//*10.001;


				lc=V_GetPointColorR(lx,ly);
	//			if(rand()/(float)RAND_MAX > 0.7)
	//				V_Point((int)(lx+(px-lx)),(int)(ly+(py-ly)),lc,lc,lc);
		//		if(rand()/(float)RAND_MAX > 0.8)
		//			V_Point((int)(lx+(lx-px)),(int)(ly+(ly-py)),lc,lc,lc);
				
			//	cout << px << "," << py << endl;
	
				V_PointSmooth((int)px,(int)py, 255, 255, 255);
			}

			
			cout << "finish row: " << krowa << "of" << ile << " rows." << endl;
		}
		

	 for (x=0; x<width; x++)
	  {
	    free(InterpolatedU[x]);
	    InterpolatedU[x]=NULL;
	  }
	
	 for (x=0; x<width; x++)
	  {
	    free(InterpolatedV[x]);
	    InterpolatedV[x]=NULL;
	  }
	
	  free(InterpolatedU);
	  InterpolatedU=NULL;
	  free(InterpolatedV);
	  InterpolatedV=NULL;
}









//
//
// smooth, interpolated color field
//

void PlotThreeFieldsSmooth(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale)
{
	int i,j;


	width=width*scale;
	height=height*scale;

	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	double x01,y01;
	
	double ui1,ui2,ui;

	double centerU,centerV;



	double UMAX=u[0][0], fU;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fU=fabs(u[i][j]);
				UMAX=max(fU,UMAX);
			}

	double VMAX=v[0][0], fV;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fV=fabs(v[i][j]);
				VMAX=max(fV,VMAX);
			}

	double PMAX=p[0][0], fP;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fP=fabs(p[i][j]);
				PMAX=max(fP,PMAX);
			}




/*	double **InterpolatedU;
	InterpolatedU = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedU[x]=new double[height];
*/

	double colu;
	double colv;
	double colp;

	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
		
		i=(int)((double)x/DX);
		j=(int)((double)y/DY);
		
		x01 = ((double)x/DX) - i;
		y01 = ((double)y/DY) - j;
			

		colu =  (1-y01)  *( (1-x01)*fabs(u[i][j]) + x01*fabs(u[i+1][j])  );
		colu += (y01)    *( (1-x01)*fabs(u[i][j+1]) + x01*fabs(u[i+1][j+1])  );
		colv =  (1-y01)  *( (1-x01)*fabs(v[i][j]) + x01*fabs(v[i+1][j])  );
		colv += (y01)    *( (1-x01)*fabs(v[i][j+1]) + x01*fabs(v[i+1][j+1])  );
		colp =  (1-y01)  *( (1-x01)*fabs(p[i][j]) + x01*fabs(p[i+1][j])  );
		colp += (y01)    *( (1-x01)*fabs(p[i][j+1]) + x01*fabs(p[i+1][j+1])  );
		
		colu *= (8225/UMAX);
		colv *= (8225/UMAX);
		colp *= (8225/UMAX);

			V_Point(x,y, fabs(colu),fabs(colv) ,fabs(colp));
//			V_Point(x,y, fabs(colu),fabs(colu) ,fabs(colu));
		}

}



void PlotOneSmoothField(int width, int height, int nx, int ny, double dx, double dy, double **f, int scale)
{
	int i,j;


	width=width*scale;
	height=height*scale;

	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	double x01,y01;
	
	double ui1,ui2,ui;

	double centerf;



	double UMAX=f[0][0], fU;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fU=fabs(f[i][j]);
				UMAX=max(fU,UMAX);
			}


	double colu;

	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
			i=(int)((double)x/DX);
			j=(int)((double)y/DX);
		
			x01 = ((double)x/DX) - i;
			y01 = ((double)y/DX) - j;
			

		colu =  (1-y01)  *( (1-x01)*fabs(f[i][j]) + x01*fabs(f[i+1][j])  );
		colu += (y01)    *( (1-x01)*fabs(f[i][j+1]) + x01*fabs(f[i+1][j+1])  );
		colu *= (8225/UMAX);

			if(colu>0)
				V_Point(x,y, colu, colu, colu);
			else
				V_Point(x,y, 0,0, fabs(colu));
		}

}



//
//
// really - plot preety kind of color visualization
//

void PlotOneField(int width, int height, int nx, int ny, double dx, double dy, double **f, int scale)
{
	int i,j;


	width=width*scale;
	height=height*scale;

	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	double x01,y01;
	
	double ui1,ui2,ui;

	double centerf;



	double UMAX=f[0][0], fU;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fU=fabs(f[i][j]);
				UMAX=max(fU,UMAX);
			}


	double colu;

	for(x=0;x<width-DX*2;x++)
		for(y=0;y<height-DY*2;y++){
			i=(int)((double)x/DX);
			j=(int)((double)y/DX);
		
			x01 = ((double)x/DX) - i;
			y01 = ((double)y/DX) - j;
			
			colu=(fabs((f[i][j]+f[i+1][j])*(x01))/UMAX)*3255;
			colu=((f[i][j])/UMAX)*11255;

			if(colu>0)
				V_Point(x,y, colu, colu, colu);
			else
				V_Point(x,y, 0,0, fabs(colu));
		}



	/*
	for(y=DY;y<width-2*DY;y++)
		for(x=DX;x<width-2*DX;x++){

			i=(int)((double)x/DX);
			j=(int)((double)y/DX);

			x01 = ((double)x/DX) - i;
			y01 = ((double)y/DX) - j;

//			V_Point(x,y, fabs(u[i][ny-j])*6855, fabs(v[i][ny-j])*6855, fabs(p[i][ny-j])*6855 );
			V_Point(x,y, 0, 0, fabs(p[i][ny-j])*2855 );
			
			//std::cout << x01 << std::endl;
		}
*/

			


}
void PlotThreeFields(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, double **p, int scale)
{
	int i,j;


	width=width*scale;
	height=height*scale;

	double DX=(double)width/(double)nx;
	double DY=(double)height/(double)ny;
	int x,y;
	double x01,y01;
	
	double ui1,ui2,ui;

	double centerU,centerV;



	double UMAX=u[0][0], fU;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fU=fabs(u[i][j]);
				UMAX=max(fU,UMAX);
			}

	double VMAX=v[0][0], fV;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fV=fabs(v[i][j]);
				VMAX=max(fV,VMAX);
			}

	double PMAX=p[0][0], fP;
	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
				fP=fabs(p[i][j]);
				PMAX=max(fP,PMAX);
			}




/*	double **InterpolatedU;
	InterpolatedU = new double*[width];
	for(x=0;x<width;x++)
		InterpolatedU[x]=new double[height];
*/

	double colu;
	double colv;
	double colp;

	for(x=0;x<width-DX;x++)
		for(y=0;y<height-DY;y++){
			i=(int)((double)x/DX);
			j=(int)((double)y/DX);
		
			x01 = ((double)x/DX) - i;
			y01 = ((double)y/DX) - j;
			
//			colu=(fabs((u[i][j]+u[i+1][j])*(x01))/UMAX)*3255;
			colu=((u[i][j])/UMAX)*655;
			colv=((v[i][j])/VMAX)*655;
			colp=((p[i][j])/PMAX)*655;

			V_Point(x,y, fabs(colu),fabs(colv) ,fabs(colp));
		}



	/*
	for(y=DY;y<width-2*DY;y++)
		for(x=DX;x<width-2*DX;x++){

			i=(int)((double)x/DX);
			j=(int)((double)y/DX);

			x01 = ((double)x/DX) - i;
			y01 = ((double)y/DX) - j;

//			V_Point(x,y, fabs(u[i][ny-j])*6855, fabs(v[i][ny-j])*6855, fabs(p[i][ny-j])*6855 );
			V_Point(x,y, 0, 0, fabs(p[i][ny-j])*2855 );
			
			//std::cout << x01 << std::endl;
		}
*/

			


}





void PlotVelocities(int width, int height, int nx, int ny, double dx, double dy, double **u, double **v, int scale)
{
	int i,j;
	double UMAX,VMAX,fU,fV;

	double centerU,centerV;

	UMAX=u[0][0];
	VMAX=v[0][0];

	width=width*scale;
	height=height*scale;


	for(j=0;j<ny-1;j++)
		for(i=0;i<nx-1;i++){
			
			centerU=(u[i][j]+u[i][j+1])*0.5;
			centerV=(v[i][j]+v[i+1][j])*0.5;

			fU=fabs(centerU);
				fV=fabs(centerV);

				UMAX=max(fU,UMAX);
				VMAX=max(fV,VMAX);
			}

	fU=UMAX;
	fV=VMAX;
	double siz;

	for(j=1;j<ny-1;j++)
		for(i=1;i<nx-1;i++){


			centerU=(u[i][j]+u[i][j-1])*0.5;
			centerV=(v[i][j]+v[i-1][j])*0.5;
		
			// niecentralne na bokach
			DrawLine((i-0.5)*((double)width/(double)nx),((j)*((double)height/(double)ny)),
				(i-0.5)*((double)width/(double)nx)+(0.5*(centerU)/fU)*scale*66,
				(j)*((double)height/(double)ny)+(0.5*(centerV)/fV)*scale*66
				,0,0,0);


			// j.w. bez height -
/*			DrawLine((i+0.5)*((double)width/(double)nx),((j+0.5)*((double)height/(double)ny)),
				(i+0.5)*((double)width/(double)nx)+(0.5*(u[i][j])/fU)*scale*6,
				((j+0.5)*((double)height/(double)ny)+(0.5*(v[i][j])/fV)*scale*6)
				,255,0,0);
*/
			
			// usrenione (centralne)
/*			DrawLine((i+0.5)*((double)width/(double)nx),height - ((j-0.5)*((double)height/(double)ny)),
				(i+0.5)*((double)width/(double)nx)+(0.5*(u[i][j]+u[i+1][j])/fU)*scale*2,
				height - ((j-0.5)*((double)height/(double)ny)+(0.5*(v[i][j]+v[i][j+1])/fV)*scale*2)
				,255,0,0);
*/
			
			// jednostkowe
/*			fU=u[i][j];
			fV=v[i][j];

			siz = sqrt(pow(fU,2) + pow(fV,2));
			
			if(siz)
			DrawLine((i+0.5)*((double)width/(double)nx),height - ((j-0.5)*((double)height/(double)ny)),
				(i+0.5)*((double)width/(double)nx)+(1*(fU)/siz)*scale*0.8,
				height - ((j-0.5)*((double)height/(double)ny)+(1*(fV)/siz)*scale*0.8)
				,255,0,0);
*/
			
			//			DrawLine((int)((i-0.5))*scale,(1+height)-(int)((j-0.5)*dy)*scale,(int)((i-0.5)*dx)*scale+(int)((u[i][j] * (dx)) / UMAX)*scale,(1+height)-(int)((j-0.5)*dy)*scale-(int)((v[i][j] * (dy)) / VMAX)*scale,255,0,0);

		
		}

}






void PlotParticles(int nx, int ny, double **Table)
{
    int i,j;
    double mymax,mymin;
    double t;

    mymin=mymax=fabs(Table[2][2]);
    
    for(i=2;i<nx-1;i++)
	for(j=2;j<ny-1;j++)
	{
	    mymax=max(mymax,fabs(Table[i][j]));

	    if(Table[i][j] != 0)
			mymin=min(mymin,fabs(Table[i][j]));
	}

	for(i=2;i<nx-1;i++)
	for(j=2;j<ny-1;j++)
	{
		V_Point(i,ny-j
			,120 + (int) ((fabs(Table[i][j]) - mymin)*(COLMAX-120))/(mymax-mymin)
			,140 + (int) ((fabs(Table[i][j]) - mymin)*(COLMAX-140))/(mymax-mymin)
			,180 + (int) ((fabs(Table[i][j]) - mymin)*(COLMAX-180))/(mymax-mymin));
	}

//	V_Point(1,1,250,0,0);
}




