
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "view.h"

int width, height;

float **picR=NULL;
float **picG=NULL;
float **picB=NULL;


void V_ClearRect(int x0, int y0, int x1, int y1)
{
  int x, y;

  for (y=y0; y<=y1; y++)
    for (x=x0; x<=x1; x++)
    {
/*      picR[y][x]=(int)255;
      picG[y][x]=(int)255;
      picB[y][x]=(int)255;
*/
      picR[y][x]=(int)0;
      picG[y][x]=(int)0;
      picB[y][x]=(int)0;

    
	}
}

void V_ClearRect(int x0, int y0, int x1, int y1, int col)
{
  int x, y;

  for (y=y0; y<=y1; y++)
    for (x=x0; x<=x1; x++)
    {
/*      picR[y][x]=(int)255;
      picG[y][x]=(int)255;
      picB[y][x]=(int)255;
*/
      picR[y][x]=(int)col;
      picG[y][x]=(int)col;
      picB[y][x]=(int)col;

    
	}
}

void V_Line(int x0, int y0, int x1, int y1)
{
}

void V_PointCross(int x, int y, int r, int g, int b)
{
	if(x>width-1 || y>height-1 || x<1 || y<1)
		return;

    if(r>255)
	r=255;

    if(g>255)
	g=255;

    if(b>255)
	b=255;

    if(r<0)
	r=0;

    if(g<0)
	g=0;

    if(b<0)
	b=0;

  

  
  
    picR[y][x]=r;
	picG[y][x]=g;
	picB[y][x]=b;

  if(y+4<height)
  {
    picR[y+1][x]=(picR[y+1][x]+r)/2;
	picG[y+1][x]=(picG[y+1][x]+g)/2;
	picB[y+1][x]=(picB[y+1][x]+b)/2;
	  if(x-4>0)								   //y+1x-1
	  {
	    picR[y+1][x-1]=(picR[y+1][x-1]+r)/2;
		picG[y+1][x-1]=(picG[y+1][x-1]+g)/2;
		picB[y+1][x-1]=(picB[y+1][x-1]+b)/2;
	  }
	  if(x+4>0)
	  {
	    picR[y+1][x+1]=(picR[y+1][x+1]+r)/2;   //y+1x+1
		picG[y+1][x+1]=(picG[y+1][x+1]+g)/2;
		picB[y+1][x+1]=(picB[y+1][x+1]+b)/2;
	  }
   }

  if(y-4>0)
  {
    picR[y-1][x]=(picR[y-1][x]+r)/2;
	picG[y-1][x]=(picG[y-1][x]+g)/2;
	picB[y-1][x]=(picB[y-1][x]+b)/2;
	  if(x-4>0)								   //y-1x-1
	  {
	    picR[y-1][x-1]=(picR[y-1][x-1]+r)/2;
		picG[y-1][x-1]=(picG[y-1][x-1]+g)/2;
		picB[y-1][x-1]=(picB[y-1][x-1]+b)/2;
	  }
	  if(x+4>0)
	  {
	    picR[y-1][x+1]=(picR[y-1][x+1]+r)/2;   //y-1x+1
		picG[y-1][x+1]=(picG[y-1][x+1]+g)/2;
		picB[y-1][x+1]=(picB[y-1][x+1]+b)/2;
	  }
   }

  if(x+4<width)
  {
    picR[y][x+1]=(picR[y][x+1]+r)/2;
	picG[y][x+1]=(picG[y][x+1]+g)/2;
	picB[y][x+1]=(picB[y][x+1]+b)/2;
	 if(y+4<height)						// x+1y+1
	  {
		 picR[y+1][x+1]=(picR[y+1][x+1]+r)/2;
		 picG[y+1][x+1]=(picG[y+1][x+1]+g)/2;
		 picB[y+1][x+1]=(picB[y+1][x+1]+b)/2;
	  }
	  if(y-4>0)								 //x+1y-1
	  {
	    picR[y-1][x+1]=(picR[y-1][x+1]+r)/2; 
		picG[y-1][x+1]=(picG[y-1][x+1]+g)/2;
		picB[y-1][x+1]=(picB[y-1][x+1]+b)/2;
	  }
  
  }

  if(x-4>0)
  {
    picR[y][x-1]=(picR[y][x-1]+r)/2;
	picG[y][x-1]=(picG[y][x-1]+g)/2;
	picB[y][x-1]=(picB[y][x-1]+b)/2;
	 if(y+4<height)						// x-1y+1
	  {
		 picR[y+1][x-1]=(picR[y+1][x-1]+r)/2;
		 picG[y+1][x-1]=(picG[y+1][x-1]+g)/2;
		 picB[y+1][x-1]=(picB[y+1][x-1]+b)/2;
	  }
	  if(y-4>0)								 //x-1y-1
	  {
	    picR[y-1][x-1]=(picR[y-1][x-1]+r)/2; 
		picG[y-1][x-1]=(picG[y-1][x-1]+g)/2;
		picB[y-1][x-1]=(picB[y-1][x-1]+b)/2;
	  }
   }


}

int V_GetPointColorR(int x, int y)
{
	return picR[y][x];
}

void V_PointSmooth(int x, int y, int r, int g, int b)
{
	if(x>width-2 || y>height-2 || x<2 || y<2)
		return;

    if(r>255)
	r=255;

    if(g>255)
	g=255;

    if(b>255)
	b=255;

    if(r<0)
	r=0;

    if(g<0)
	g=0;

    if(b<0)
	b=0;

#define rescale 35.6;

    picR[y][x]+=0.05 * rescale;
	picG[y][x]+=0.05 * rescale;
	picB[y][x]+=0.05 * rescale;

    picR[y+1][x]+=0.005 * rescale;
	picG[y+1][x]+=0.005 * rescale;
	picB[y+1][x]+=0.005 * rescale;
    picR[y][x+1]+=0.005 * rescale;
	picG[y][x+1]+=0.005 * rescale;
	picB[y][x+1]+=0.005 * rescale;
    picR[y-1][x]+=0.005 * rescale;
	picG[y-1][x]+=0.005 * rescale;
	picB[y-1][x]+=0.005 * rescale;
    picR[y][x-1]+=0.005 * rescale;
	picG[y][x-1]+=0.005 * rescale;
	picB[y][x-1]+=0.005 * rescale;



	if(picR[y][x]>255)
		picR[y][x]=255;
	if(picG[y][x]>255)
		picG[y][x]=255;
	if(picB[y][x]>255)
		picB[y][x]=255;	
	if(picR[y+1][x]>255)
		picR[y+1][x]=255;
	if(picG[y+1][x]>255)
		picG[y+1][x]=255;
	if(picB[y+1][x]>255)
		picB[y+1][x]=255;
	if(picR[y-1][x]>255)
		picR[y-1][x]=255;
	if(picG[y-1][x]>255)
		picG[y-1][x]=255;
	if(picB[y-1][x]>255)
		picB[y-1][x]=255;
	if(picR[y][x+1]>255)
		picR[y][x+1]=255;
	if(picG[y][x+1]>255)
		picG[y][x+1]=255;
	if(picB[y][x+1]>255)
		picB[y][x+1]=255;
	if(picR[y][x-1]>255)
		picR[y][x-1]=255;
	if(picG[y][x-1]>255)
		picG[y][x-1]=255;
	if(picB[y][x-1]>255)
		picB[y][x-1]=255;


}


void V_Point(int x, int y, int r, int g, int b)
{

	if(x>width-1 || y>height-1 || x<1 || y<1)
		return;

    if(r>255)
	r=255;


    if(g>255)
	g=255;

    if(b>255)
	b=255;

    if(r<0)
	r=0;

    if(g<0)
	g=0;

    if(b<0)
	b=0;

  picR[y][x]=r;
  picG[y][x]=g;
  picB[y][x]=b;
}

void V_PointSquare(int x, int y, int r, int g, int b)
{

	if(x>width-1 || y>height-1 || x<1 || y<1)
		return;

    if(r>255)
	r=255;


    if(g>255)
	g=255;

    if(b>255)
	b=255;

    if(r<0)
	r=0;

    if(g<0)
	g=0;

    if(b<0)
	b=0;

  picR[y][x]=r;
  picG[y][x]=g;
  picB[y][x]=b;
  picR[y][x+1]=r;
  picG[y][x+1]=g;
  picB[y][x+1]=b;
  picR[y][x-1]=r;
  picG[y][x-1]=g;
  picB[y][x-1]=b;
  picR[y+1][x]=r;
  picG[y+1][x]=g;
  picB[y+1][x]=b;
  picR[y-1][x]=r;
  picG[y-1][x]=g;
  picB[y-1][x]=b;
}

int V_CheckExitTrue(void)
{
    return 0;
}


void V_OpenView(int sizex, int sizey)
{
  int y;

  V_CloseView();
  
  width=sizex;
  height=sizey;

  picR=(float **)calloc(height, sizeof(float *));
  picG=(float **)calloc(height, sizeof(float *));
  picB=(float **)calloc(height, sizeof(float *));

  for (y=0; y<height; y++)
  {
    picR[y]=(float *)calloc(width, sizeof(float));
  }

  for (y=0; y<height; y++)
  {
    picG[y]=(float *)calloc(width, sizeof(float));
  }

  for (y=0; y<height; y++)
  {
    picB[y]=(float *)calloc(width, sizeof(float));
  }

}

void V_CloseView(void)
{
  int y;

  for (y=0; y<height; y++)
  {
    free(picR[y]);
    picR[y]=NULL;
  }

  for (y=0; y<height; y++)
  {
    free(picG[y]);
    picG[y]=NULL;
  }

  for (y=0; y<height; y++)
  {
    free(picB[y]);
    picB[y]=NULL;
  }

  free(picR);
  picR=NULL;

  free(picG);
  picG=NULL;

  free(picB);
  picB=NULL;

}

int V_Write(int frame)		// forgot - problems
{
  FILE *out;
  static int framenumber=0;
  int x, y;
  char fname[100];

  sprintf(fname, "frames/frame_%05d.ppm", framenumber++);

  out=fopen(fname, "w");

  fprintf(out, "P3\n%d %d\n255\n", width-1, height-1);

  for (y=1; y<height; y++)
    for (x=1; x<width; x++)
      fprintf(out, "%d %d %d\n", (int)picR[y][x], (int)picG[y][x], (int)picB[y][x]);
      
  fclose(out);

   return framenumber;
}
