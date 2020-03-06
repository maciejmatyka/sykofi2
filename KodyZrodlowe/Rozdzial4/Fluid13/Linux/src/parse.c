

/*

Parse .opt file functions


*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dimalloc.h"
#include "particle.h"
#include "bndflags.h"

#define MAXWORD 80

void MoveNextWord(FILE *fp, char *word);
int CheckNextWord(FILE *fp, char *word);
void ISetValue(FILE *fp, int *itoset);
void ISetBndValue(FILE *fp, int table[], int i, int NX, int NY);
void IReadValue(FILE *fp, char *word, int *placehere);

void FReadValue(FILE *fp, char *word, double *placehere);
void FSetValue(FILE *fp, double *ftoset);


char *posserr;

void testeof(FILE *fp)
{
	int a;

	a=ungetc(getc(fp),fp);
	if(a==EOF)
	{
    printf("Parsing for \"%s\" option false.\n",posserr);
		printf("Error with .opt file! \n");
		exit(0);
	}
}

void mfscanf(FILE *fp,char workbuf2[],char workbuf[])
{
		testeof(fp);
		fscanf(fp,workbuf2,workbuf);
}

struct particle *ParticleFile(int argc, char *argv[], double dx, double dy, int nx, int ny, int width, int height, int **F, double **U, double **V,int verbose)
{
	FILE *fp;
	char workbuf[MAXWORD];

	int			x1,y1,x2,y2;
	double	step;
	double	uk,vk;

	struct particle *wrkpart;
	struct particle *partptr;



	workbuf[0]=0;

  if(argc==1)
  {
		printf("Option file not specified.\n");
		printf("USAGE: fluid <file.opt>\n");
		exit(0);
  }

  if(argc==2)
	{
	  if(strcmp(argv[1],"?")==0 || strcmp(argv[1],"-?")==0 || strcmp(argv[1],"h")==0 || strcmp(argv[1],"-h")==0 || strcmp(argv[1],"help")==0 || strcmp(argv[1],"-help")==0)
		{
			printf("USAGE: fluid <file.opt>\n");
			exit(0);
	  }

	  if(!(fp=fopen(argv[1],"r")))
		{
			printf("Unable to open %s file.\n",argv[1]);
			exit(1);
	  }
	}

  if(argc!=2 && argc!=1)
   {
		printf("Too many parametrs.\n");
		exit(1);
   }




  MoveNextWord(fp,"PARTICLES:");

	wrkpart=AllocOneParticle(0.0,0.0,P_AVAIL,0,0,0.0,0.0);
  partptr=wrkpart;

	x1=x2=y1=y2=step=0;
	uk=vk=0.0;

	printf("\n");

	while(strcmp(workbuf,"ENDPAR."))
	{
		mfscanf(fp,"%s",workbuf);

		if(!(strcmp(workbuf,"BOX")))
		{
			if(verbose)
			printf("BOX  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			ISetValue(fp,&y2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%d,%f,%f,%f\n",x1,y1,x2,y2,step,uk,vk);

			wrkpart=AllocParticleBox(x1 , y1 , x2 ,y2 ,step , uk,vk ,wrkpart,dx,dy,nx,ny,width,height,F,U,V);

    }


		if(!(strcmp(workbuf,"BALL")))
		{
			if(verbose)
			printf("BALL  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%f,%f,%f\n",x1,y1,x2,step,uk,vk);

			wrkpart=AllocParticleBall(x1 , y1 , x2 ,step , uk,vk ,wrkpart,dx,dy,nx,ny,width,height,F,U,V);
		}


		if(!(strcmp(workbuf,"!BALL")))
		{
			if(verbose)
			printf("!BALL  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%f,%f,%f\n",x1,y1,x2,step,uk,vk);

			NegAllocParticleBall(x1 , y1 , x2 ,step , uk,vk ,partptr,dx,dy,nx,ny,width,height,F,U,V);
		}


		if(!(strcmp(workbuf,"!BOX")))
		{
			if(verbose)
			printf("!BOX  ");

			ISetValue(fp,&x1);
			ISetValue(fp,&y1);
			ISetValue(fp,&x2);
			ISetValue(fp,&y2);
			FSetValue(fp,&step);

			FSetValue(fp,&uk);
			FSetValue(fp,&vk);

			if(verbose)
	    printf("%d,%d,%d,%d,%f,%f,%f\n",x1,y1,x2,y2,step,uk,vk);

			NegAllocParticleBox(x1 , y1 , x2 , y2,step , uk,vk ,partptr,dx,dy,nx,ny,width,height,F,U,V);
		}

  }



	fclose(fp);

	return(partptr);
}




   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
					void ParseFile(int argc, char *argv[],int *nx, int *ny, int *width, int *height, double *gx, double *gy, double *dt, int *BNum, int *BCells[], int *BPoints[]);
	  it:
          Get .opt file name from program arguments, and parse it to... read
					arguments.

          It alloc tables for boundary cells and points. Free tables after
          CellSetup!

		args:
					see below.
		return:
					none. exit if problem.
    todo: none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void ParseFile(int argc, char *argv[],int *nx, int *ny, int *width, int *height, double *gx, double *gy, double *dt, double *vis, double *sta, int *gmode, int *BNum, int *BCells[], int *BPoints[])
{
	FILE *fp;
	int i;
	char workbuf[MAXWORD];


  if(argc==1)
  {
		printf("Option file not specified.\n");
		printf("USAGE: fluid <file.opt>\n");
		exit(0);
  }

  if(argc==2)
	{
	  if(strcmp(argv[1],"?")==0 || strcmp(argv[1],"-?")==0 || strcmp(argv[1],"h")==0 || strcmp(argv[1],"-h")==0 || strcmp(argv[1],"help")==0 || strcmp(argv[1],"-help")==0)
		{
			printf("USAGE: fluid <file.opt>\n");
			exit(0);
	  }

	  if(!(fp=fopen(argv[1],"r")))
		{
			printf("Unable to open %s file.\n",argv[1]);
			exit(1);
	  }
	}

  if(argc!=2 && argc!=1)
   {
		printf("Too many parametrs.\n");
		exit(1);
   }



	IReadValue(fp,"WIDTH",width);
	IReadValue(fp,"HEIGHT",height);
	IReadValue(fp,"NX",nx);
	IReadValue(fp,"NY",ny);
	FReadValue(fp,"GX",gx);
	FReadValue(fp,"GY",gy);
	FReadValue(fp,"DT",dt);
	FReadValue(fp,"VIS",vis);
	FReadValue(fp,"STABILITY",sta);

	IReadValue(fp,"GMODE",gmode);

/*	IReadValue(fp,"BoundariesNum",BNum);*/


	fclose(fp);

  if(!(fp=fopen(argv[1],"r")))
	{
		printf("Unable to open %s file.\n",argv[1]);
		exit(1);
  }

  MoveNextWord(fp,"BNDCELLS:");

	*BNum=0;

  while(!(CheckNextWord(fp,"ENDBND.")))
	{
		mfscanf(fp,"%s",workbuf);

/*		printf("%s\n",workbuf);*/

		mfscanf(fp,"%s\n",workbuf);

/*		printf("%s");*/

		(*BNum)+=1;
	}

/*	printf("\nBNum=%d\n",*BNum);*/

	if(!(*BCells=Alloc1TableI((*BNum+3)*3+1)))
	{
		printf("No memory to alloc tables.\n");
		exit(1);
	}

	if(!(*BPoints=Alloc1TableI((*BNum+3)*3+1)))
	{
		printf("No memory to alloc tables.\n");
		exit(1);
	}

	fclose(fp);


  if(!(fp=fopen(argv[1],"r")))
	{
		printf("Unable to open %s file.\n",argv[1]);
		exit(1);
  }


  MoveNextWord(fp,"BNDCELLS:");

	for(i=0;i<=((*BNum)*3);i++)
	{
		ISetBndValue(fp, *BCells, i,*nx, *ny);
  }


/*	for(i=0;i<=((*BNum)*3);i++)*/
/*	{*/
/*		printf("%d \n",BCells[i]);*/
/*  }*/
/*	exit(0);*/


	fclose(fp);
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void IReadValue(FILE *fp, char *word, int *placehere);
				void FReadValue(FILE *fp, char *word, double *placehere);
	  it:
				Read value specified by "word" and write it to *placehere.
        That function support int types of values.

				FReadValue (for double value).

		args:
				FILE *fp - pointer to .opt file
				char *word - word to search in file
				int *placehere - here will be placed value readed from *fp file.
		return:
				none
		todo:
				error checking. return TRUE or FALSE of value reading.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void IReadValue(FILE *fp, char *word, int *placehere)
{
	MoveNextWord(fp,word);
	if(CheckNextWord(fp,"="))
    ISetValue(fp,placehere);
}

void FReadValue(FILE *fp, char *word, double *placehere)
{
	MoveNextWord(fp,word);
	if(CheckNextWord(fp,"="))
    FSetValue(fp,placehere);
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void MoveNextWord(FILE *fp, char *word);
	  it:
				Move fp pointer in file to next word behind "word".
				It use mfscanf function.
		args:
				FILE *fp - pointer to .opt file
				char *word - word to search in file
		return:
				none, just change fp pointer.
		todo:
				error check.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void MoveNextWord(FILE *fp, char *word)
{
	char workbuf[MAXWORD];

	posserr=word;

	while(!(strcmp(workbuf,word)==0))
	{
		mfscanf(fp,"%s",workbuf);
		if((strcmp(workbuf,"/*")==0))			/* comment? */
			while(!(strcmp(workbuf,"*/")==0))
						mfscanf(fp,"%s",workbuf);
	}
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				int CheckNextWord(FILE *fp, char *word);
	  it:
				Check next word in file with "word".
		args:
				FILE *fp - pointer to .opt file
				char *word - word to check
		return:
				TRUE - next word in file == "word"
        FALSE - next word in file != "word"
		todo:
				Check EOF.
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

int CheckNextWord(FILE *fp, char *word)
{
	char workbuf[MAXWORD];


	posserr=word;

	mfscanf(fp,"%s",workbuf);

	return((strcmp(workbuf,word)==0));
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void ISetValue(FILE *fp, int *itoset);
				void FSetValue(FILE *fp, double *ftoset);
	  it:
				That function KNOW and set than next word is a value of itoset.
		args:
				FILE *fp - pointer to .opt file
				int *itoset - here place value of next word in *fp
		return:
				none
		todo:
				Check for value problems (range etc.)
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void FSetValue(FILE *fp, double *ftoset)
{
	char workbuf[MAXWORD];

	mfscanf(fp,"%s",workbuf);

		if((strcmp(workbuf,"/*")==0))			/* comment? */
    {
		while(!(strcmp(workbuf,"*/")==0))
					mfscanf(fp,"%s",workbuf);

		mfscanf(fp,"%s",workbuf);
    }


  *ftoset=atof(workbuf);
}

void ISetValue(FILE *fp, int *itoset)
{
	char workbuf[MAXWORD];

	mfscanf(fp,"%s",workbuf);

		if((strcmp(workbuf,"/*")==0))			/* comment? */
    {
		while(!(strcmp(workbuf,"*/")==0))
					mfscanf(fp,"%s",workbuf);

		mfscanf(fp,"%s",workbuf);
    }


  *itoset=atoi(workbuf);
}

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:
				void ISetBndValue(FILE *fp, int table[], int i, int NX, int NY);
	  it:
				That function KNOW and set than next word is a value of itoset.
        Its also check, if word = "NX" or "NY" then set actual
        value of NX or NY.
		args:
				FILE *fp - pointer to .opt file
				int table[] - here place value of next word in *fp
				int i - offset in table
				int pWIDTH, int pHEIGHT - values readed before
		return:
				none
		todo:
				Check for value problems (range etc.)
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


void ISetBndValue(FILE *fp, int table[], int i, int NX, int NY)
{
	char workbuf[MAXWORD];

	mfscanf(fp,"%s",workbuf);

		if((strcmp(workbuf,"/*")==0))			/* comment? */
    {

		while(!(strcmp(workbuf,"*/")==0))	/* search close of comment */
					mfscanf(fp,"%s",workbuf);

		mfscanf(fp,"%s",workbuf);
    }

  if((strcmp(workbuf,"NX")==0))
  {
		table[i]=NX;
		return;
  }

  if((strcmp(workbuf,"NY")==0))
  {
		table[i]=NY;
		return;
  }

	if((strcmp(workbuf,"FRSLP")==0))
	{
		table[i]=C_FRSLP;
		return;
	}

	if((strcmp(workbuf,"NOSLP")==0))
	{
		table[i]=C_NOSLP;
		return;
	}


  table[i]=atoi(workbuf);


/*	printf("table[%d]=%d\n",i,table[i]);*/

}
