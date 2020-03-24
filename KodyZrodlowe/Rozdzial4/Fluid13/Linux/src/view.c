
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "view.h"

int width, height;

int **picR=NULL;
int **picG=NULL;
int **picB=NULL;


void V_ClearRect(int x0, int y0, int x1, int y1)
{
  int x, y;

  for (y=y0; y<=y1; y++)
    for (x=x0; x<=x1; x++)
    {
      picR[y][x]=(int)205;
      picG[y][x]=(int)195;
      picB[y][x]=(int)215;
    }
}

void V_Line(int x0, int y0, int x1, int y1)
{
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

  picR=calloc(height, sizeof(int *));
  picG=calloc(height, sizeof(int *));
  picB=calloc(height, sizeof(int *));

  for (y=0; y<height; y++)
  {
    picR[y]=calloc(width, sizeof(int));
  }

  for (y=0; y<height; y++)
  {
    picG[y]=calloc(width, sizeof(int));
  }

  for (y=0; y<height; y++)
  {
    picB[y]=calloc(width, sizeof(int));
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

/*

As you see here - it's important that you must have frames/ drawer created, you can change it here */

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
