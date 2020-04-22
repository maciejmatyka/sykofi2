#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "importppm.h"

void getpropertiesppm(string fname, int &nx, int &ny)
{
	// now open the file
	ifstream ppmfile(fname.c_str());
	
	if(!ppmfile)
	{
		cout << "Cannot open the file: " << fname << endl;
		//system("PAUSE");
		exit(0);
	}

	#define line fname  // don't waste one string

	// string with one single line from the file

	getline (ppmfile,line);			// P3
	getline (ppmfile,line);			// # Created by IrfanView 
	
	// read and store as int in nx/ny
	ppmfile >> line;
	nx = atoi(line.c_str());
	ppmfile >> line;
	ny = atoi(line.c_str());

	// close file
	ppmfile.close();
}


//void importppm(string fname, int field[MAPW][MAPH], int _nx, int _ny)
void importppm(string fname, int field[NX][NY], int _nx, int _ny)
{
	int i,j;

	ifstream ppmfile(fname.c_str()); // open the file
	
	if(!ppmfile)
	{
		cout << "Cannot open the file: " << fname << endl;
		exit(1);
	}

	#define line fname  // don't waste one string
	
	// move the header
	getline(ppmfile,line);			// P3
	getline(ppmfile,line);			// NX, NY
	getline(ppmfile,line);			// # Created by IrfanView 
	getline(ppmfile,line);			// depth
		
	for(j=_ny-1; j>0; j--)				// read data
	for(i=0; i<_nx; i++)
	{
		ppmfile >> line;		// R
		field[i][j] = atoi(line.c_str());
		ppmfile >> line;		// G
		//field[i][j][1] = atoi(line.c_str());
		ppmfile >> line;		// B
		//field[i][j][2] = atoi(line.c_str());
	}

	// close file
	ppmfile.close();
}



//void importppmcol(string fname, int field[W][H][3], int _nx, int _ny)
//void importppmcol(string fname, int field[1280][720][3], int _nx, int _ny)
void importppmcol(string fname, int field[640][360][3], int _nx, int _ny)
{
	int i,j;

	ifstream ppmfile(fname.c_str()); // open the file
	
	if(!ppmfile)
	{
		cout << "Cannot open the file: " << fname << endl;
		exit(1);
	}

	#define line fname  // don't waste one string
	
	// move the header
	getline(ppmfile,line);			// P3
	getline(ppmfile,line);			// NX, NY
	getline(ppmfile,line);			// # Created by IrfanView 
	getline(ppmfile,line);			// depth
		
	for(j=_ny-1; j>0; j--)				// read data
	for(i=0; i<_nx; i++)
	{
		ppmfile >> line;		// R
		field[i][j][0] = atoi(line.c_str());
		ppmfile >> line;		// G
		field[i][j][1] = atoi(line.c_str());
		ppmfile >> line;		// B
		field[i][j][2] = atoi(line.c_str());
	}

	// close file
	ppmfile.close();
}