#ifndef _I_importppm_H_
#define _I_importppm_H_

#include "global.h"



//void importppm(string fname, int field[NX][NY], int _nx, int _ny);
void importppmcol(string fname, int field[PICTURE_WIDTH][PICTURE_HEIGHT][3], int _nx, int _ny);
//void importppmcol(string fname, int field[640][360][3], int _nx, int _ny);
void getpropertiesppm(string fname, int &nx, int &ny);

#endif

