
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"

#include "view.h"

#include "kolizje.h"




int szer=250,wys=250;

float xmin=-0.9,xmax=0.9,ymin=-0.9,ymax=0.9;


float dt=0.006;
float bak_dt=dt;



float tlumienie = 0.8;


Wektor Graw(0,0.0f,0);


Punkt *KorzenPunktow;
Sprezyna *KorzenSprezyn;

Obszar *KorzenObszarow;


//#define lina

int main(int argc, char **argv)
{
	int i;
	Punkt *roboczyP2;


	KorzenPunktow=AlokujPunkt(0,P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
	KorzenSprezyn=AlokujSprezyne(0,0,0,KorzenPunktow,KorzenPunktow);
	KorzenObszarow=AlokujObszar(0,Wektor(0,0,0),0,O_NAKT,NULL);


   	//DodajObszar(KorzenObszarow,1.0,Wektor(0.02,0,0),0.9,O_AKT,NULL);


 
//	DodajPunkt(KorzenPunktow,4,P_NORM,Wektor(-1.6,0,0),Wektor(0,0,0),0.25);


/*   	for(i=35;i<90;i++)
	{
	   DodajPunkt(KorzenPunktow,1,P_NORM,Wektor((i-60)/120.f,0.5f,0),Wektor(0,0,0));
//	   DodajPunkt(KorzenPunktow,10,P_NORM,Wektor(0.5 - rand()/(float)RAND_MAX, rand()/(float)RAND_MAX,0),Wektor(0,0,0));
//	   DodajObszar(KorzenObszarow,0.03,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));
	}
*/

//	   DodajPunkt(KorzenPunktow,1,P_NORM,Wektor(0.4,0.5,0),Wektor(0,0,0),0.0);


/*	   DodajPunkt(KorzenPunktow,1.5,P_NORM,Wektor(0,0.1,0),Wektor(0,0,0),0.2);
	   DodajPunkt(KorzenPunktow,0.5,P_NORM,Wektor(-0.1,0,0),Wektor(0,0,0),0.02);
	   DodajPunkt(KorzenPunktow,0.5,P_NORM,Wektor(0.1,0,0),Wektor(0,0,0),0.02);
*/


/*m1=10000,[0,0,0],[-2.5,1.5,0],[12,1]
m2=1000,[0,9,0],[25,-15,0],[4,6]
*/

/*
	   DodajPunkt(KorzenPunktow,260,P_NORM|P_WEKT,Wektor(0,0,0),Wektor(-0.0250,0.015,0),0);		 
	   DodajPunkt(KorzenPunktow,10,P_NORM|P_WEKT,Wektor(0,0.19,0),Wektor(0.25,-0.15,0),0);
	   DodajPunkt(KorzenPunktow,10,P_NORM|P_WEKT,Wektor(0,-0.19,0),Wektor(-0.25,0.15,0),0);
  */




#ifdef lina
	for(i=0;i<40;i++)
	{		
		if(i<5 || i>32)
	   		DodajPunkt(KorzenPunktow,1,P_ZAW,Wektor(-0.9+(float)i/21.0f,0.1,0),Wektor(0,0,0),0);
	   	else
  			DodajPunkt(KorzenPunktow,1,P_NORM|P_WEKT,Wektor(-0.9+(float)i/21.0f,0.1,0),Wektor(0,0,0),0);

	  	DodajObszar(KorzenObszarow,0.01,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));
   	}
												     

  	for(roboczyP2=KorzenPunktow;roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL; roboczyP2=roboczyP2->prawy)
//  	for(roboczyP2=KorzenPunktow;roboczyP2->prawy->prawy->prawy->prawy != NULL; roboczyP2=roboczyP2->prawy)
	{

	if(roboczyP2->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.001,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy);
					   
						 
	if(roboczyP2->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.0015,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy);

	if(roboczyP2->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.002,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy);

				
	if(roboczyP2->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.0025,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy);


	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.003,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy);


	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.0035,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy);
											

	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,12,0.004,30.1,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy);

    }

#endif







	glutInit(&argc, argv);

    glutInitWindowPosition(150, 150); glutInitWindowSize( szer, wys);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    if (glutCreateWindow("Newton") == GL_FALSE) {exit(1);}

    Init();
									   
    glutKeyboardFunc(Key);
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(idle);
    glutMainLoop();
    
	return (0);

}



int k=4;
int kv=0;

#define prsf 0.05
#define dziel 9.5f



void dodaj_punktS(void)
{
//	DodajPunkt(KorzenPunktow,1,P_NORM,Wektor(0.0,0.8,0),Wektor(0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4));
 
	DodajPunkt(KorzenPunktow,50.0,P_NORM|P_WEKT,Wektor(0.0,0.8,0),Wektor(0,0,0),0);
  	DodajObszar(KorzenObszarow,0.2,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));
}


void dodaj_punkt(void)
{
//	Punkt *roboczyP;
	Punkt *roboczyP2;
	k=k+1;

//	return;

	roboczyP2 = Ostatni(KorzenPunktow);

	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.4,0),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM|P_WEKT,Wektor(-0.5+(float)k/(dziel),0,0),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));

	DodajSprezyne(KorzenSprezyn,14.2,0.4,15.5,roboczyP2->prawy,roboczyP2->prawy->prawy);

	return;
}

void dodaj_punkt2(void)
{
	Punkt *roboczyP2;

	roboczyP2 = Ostatni(KorzenPunktow);

	k=k+1;

	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.4,0),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM|P_WEKT,Wektor(-0.5+0.4+(float)k/(dziel),0.4,0),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));

	DodajSprezyne(KorzenSprezyn,14.2,0.4,15.5,roboczyP2->prawy,roboczyP2->prawy->prawy);

	return;

}



//#define Euler
#define MidPoint


void Timestep(void)
{
//	Punkt *roboczyP;
// metoda Eulera


#ifdef Euler

	WyznaczSily(KorzenPunktow,KorzenSprezyn,KorzenObszarow);
	RozwiazEuler(KorzenPunktow,dt);

#endif


#ifdef MidPoint

	WyznaczSily(KorzenPunktow,KorzenSprezyn,KorzenObszarow);
	RozwiazMidPoint_k1(KorzenPunktow,dt);

 	WyznaczSily(KorzenPunktow,KorzenSprezyn,KorzenObszarow);
	RozwiazMidPoint_k2(KorzenPunktow,dt);

#endif


// rozpatruj kolizje


	PrzesunObszary(KorzenObszarow);
 	Kolizje(KorzenPunktow,KorzenObszarow);
	PrzesunObszary(KorzenObszarow);

	
}


