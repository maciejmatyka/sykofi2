
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glaux.h>


#include "view.h"

int width, height;

extern int **picR=NULL;
int **picG=NULL;
int **picB=NULL;

#define PX 0
#define PY 1
#define PZ 2

float point1[3] = { 0.0, 0.0, 0.0 };



void OutputFrameGL(void)
{
	int x,y;
	
	glPointSize(8);
	glBegin(GL_POINTS);
	for(x=1;x<width-1;x++)
	{
		for(y=1;y<height;y++)
 		{
			glColor3f((float)picR[height-y][x]/(float)255,(float)picG[height-y][x]/(float)255,(float)picB[height-y][x]/(float)255);
			point1[PX]=(float)(x*2-width)/width;
			point1[PY]=(float)(y*2-height)/height;
			glVertex3fv(point1);
		}
	}
	glEnd();
	
}


/* eturn color value from RGB buffers */

int V_Red(int y,int x)
{
	return picR[y][x];	
}
int V_Green(int y,int x)
{
	return picG[y][x];	
}

int V_Blue(int y,int x)
{
	return picB[y][x];	
}



void V_ClearRect(int x0, int y0, int x1, int y1)
{
  int x, y;

  for (y=y0; y<=y1; y++)
    for (x=x0; x<=x1; x++)
    {
      picR[y][x]=(int)225;
      picG[y][x]=(int)235;
      picB[y][x]=(int)235;
	}

}

void V_Line(int x0, int y0, int x1, int y1)
{
}


void V_DIV2Point(int x, int y, int r, int g, int b)
{
  picR[y][x]=(int)((float)picR[y][x]/(float)1.2);
  picG[y][x]=(int)((float)picG[y][x]/(float)1.2);
  picB[y][x]=(int)((float)picB[y][x]/(float)1.2);
}



void V_Point(int x, int y, int r, int g, int b)
{


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

void V_PointCross(int x, int y, int r, int g, int b)
{
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

  picR=(int **) calloc(height, sizeof(int *));
  picG=(int **) calloc(height, sizeof(int *));
  picB=(int **) calloc(height, sizeof(int *));

  for (y=0; y<height; y++)
  {
    picR[y]=(int *)calloc(width, sizeof(int));
  }

  for (y=0; y<height; y++)
  {
    picG[y]=(int *)calloc(width, sizeof(int));
  }

  for (y=0; y<height; y++)
  {
    picB[y]=(int *)calloc(width, sizeof(int));
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

//normal


void V_Write(int frame)
{
  FILE *out;
  int x, y;
  char fname[100];

  sprintf(fname, "frames/frame_%05d.ppm", frame);

  out=fopen(fname, "w");

  fprintf(out, "P3\n%d %d\n255\n", width, height);

  for (y=0; y<height; y++)
    for (x=0; x<width; x++)
      fprintf(out, "%d %d %d\n", picR[y][x], picG[y][x], picB[y][x]);
      
  fclose(out);

}



//small
/*
#define div 4

void V_Write(int frame)
{
  FILE *out;
  int x, y;
  char fname[100];

  sprintf(fname, "frames/frame_%05d.ppm", frame);

  out=fopen(fname, "w");

  fprintf(out, "P3\n%d %d\n255\n", width/div, height/div);

  for (y=0; y<height; y+=div)
    for (x=0; x<width; x+=div)
      fprintf(out, "%d %d %d\n", picR[y][x], picG[y][x], picB[y][x]);
      
  fclose(out);

}
*/