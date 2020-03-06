

#include <stdio.h>
#include <stdlib.h>

#include "dimalloc.h"

   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
			function:	int **Alloc2TableD(int nx, int ny);
			it:    		Dynamic alloc table of pointers. (double) type of datas.
			args:			nx - number of rows in table
            		ny - number of columns in table
			return:		(double **) Table[nx][ny].
			todo:			Alloc cleared memory (no clear table).
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


double **Alloc2TableD(int nx, int ny)
{
    int i, j;
    double **table;


    /* Alloc table int *m[nx+1] */

    if (!
	(table =
	 (double **) malloc((unsigned) (nx + 1) * sizeof(double *)))) {
	Free2TableD(table, nx, ny);
	return (NULL);		/* problem */
    }


    /* and fill it */

    for (i = 0; i <= nx; i++) {
	if (!
	    (table[i] =
	     (double *) malloc((unsigned) (ny + 1) * sizeof(double)))) {
	    Free2TableD(table, nx, ny);
	    return (NULL);	/* problem */
	}
    }

    /* clear table */
    for (j = 0; j <= ny; j++)
	for (i = 0; i <= nx; i++)
	    table[i][j] = 0;

    return table;

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:	void Free2TableD(double ** table, int nx, int ny);
	  it:       Free memory allocated by AllocTableI.
		args:			double** table - pointer to table created before.
							nx - number of rows in table
              ny - number of columns in table
		return:		none
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void Free2TableD(double **table, int nx, int ny)
{
    int i;

    for (i = 0; i <= nx; i++) {
	if (table[i] != NULL)
	    free((char *) table[i]);
    }
    free((char *) table);

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
			function:	int **Alloc2TableI(int nx, int ny);
			it:    		Dynamic alloc table of pointers. (int) type of datas.
			args:			nx - number of rows in table
            		ny - number of columns in table
			return:		(int **) Table[nx][ny].
			todo:			Alloc cleared memory (no clear table).
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


int **Alloc2TableI(int nx, int ny)
{
    int i, j, **table;


    /* Alloc table int *m[nx+1] */

    if (!(table = (int **) malloc((unsigned) (nx + 1) * sizeof(int *)))) {
	Free2TableI(table, nx, ny);
	return (NULL);		/* problem */
    }


    /* and fill it */

    for (i = 0; i <= nx; i++) {
	if (!
	    (table[i] =
	     (int *) malloc((unsigned) (ny + 1) * sizeof(int)))) {
	    Free2TableI(table, nx, ny);
	    return (NULL);	/* problem */
	}
    }

    /* clear table */
    for (j = 0; j <= ny; j++)
	for (i = 0; i <= nx; i++)
	    table[i][j] = 0;

    return table;

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:	void Free2TableI(int** table, int nx, int ny);
	  it:       Free memory allocated by AllocTableI.
		args:			int** table - pointer to table created before.
							nx - number of rows in table
              ny - number of columns in table
		return:		none
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void Free2TableI(int **table, int nx, int ny)
{
    int i;

    for (i = 0; i <= nx; i++) {
	if (table[i] != NULL)
	    free((char *) table[i]);
    }
    free((char *) table);

}



   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
			function:	int *Alloc1TableI(int n);
			it:    		Dynamic alloc table (int) type of datas.
			args:			n - number of positions in table
			return:		(int *) Table[n].
			todo:			Alloc cleared memory (no clear table).
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/


int *Alloc1TableI(int n)
{
    int i, *table;


    /* Alloc table int m[n] */

    if (!(table = (int *) malloc((unsigned) (n + 1) * sizeof(int)))) {
	return (NULL);		/* problem */
    }

    /* clear table */
    for (i = 0; i <= n; i++)
	table[i] = 0;

    return table;

}


   /*____________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		function:	void Free1TableI(int* table, int n);
	  it:       Free memory allocated by AllocTableI.
		args:			int* table - pointer to table created before.
		return:   none
		todo:			none
   ______________________________________________________________________
   """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/

void Free1TableI(int *table)
{
    free((char *) table);
}
