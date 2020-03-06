

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dimalloc.h"
#include "particle.h"
#include "bndflags.h"
#include "parse.h"

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
