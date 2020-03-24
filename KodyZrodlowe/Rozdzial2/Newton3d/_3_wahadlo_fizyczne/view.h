


#include "kolizje.h"

#ifndef __I_VIEW_H_
#define __I_VIEW_H_


extern int szer,wys;

extern float xmin,xmax,ymin,ymax;

extern Punkt *KorzenPunktow;
extern Sprezyna *KorzenSprezyn;
extern Obszar *KorzenObszarow;

extern Wektor Graw;



void idle(void);
void Reshape(int width, int height);
void Init(void);
void Key(unsigned char key, int x, int y);
void Draw(void);



#endif