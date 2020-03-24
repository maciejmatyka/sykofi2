
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"

#include "view.h"

#include "kolizje.h"




int szer=400,wys=400;

float xmin=-0.9,xmax=0.9,ymin=-0.9,ymax=0.9;


float dt=0.006;
float bak_dt=dt;



float tlumienie = 1.0;


Wektor Graw(0,0.0f,0);


Punkt *KorzenPunktow;
Sprezyna *KorzenSprezyn;

Obszar *KorzenObszarow;




// funkcja PrzesunNaKorzeniu - przesuwa wskaznik na n-ty element w liscie i zwraca jego a(dres)
// zwraca to co dostal +1, jesli blad
Punkt *P(int n)
{
	Punkt *Roboczy;
	int i;	 

	Roboczy = KorzenPunktow;

	for(i=-1;i<n;i++)
	if(Roboczy->prawy)
		Roboczy=Roboczy->prawy;
	else
		return(KorzenPunktow);
	  
	return Roboczy;
}






double dlug;		// dlugosc sprezyny ktora animujemy
Sprezyna *spre;		// sprezyna ktora animujemy
double st;



int main(int argc, char **argv)
{
	int i;
	Punkt *roboczyP2;


	KorzenPunktow=AlokujPunkt(0,P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
	KorzenSprezyn=AlokujSprezyne(0,0,0,KorzenPunktow,KorzenPunktow);
	KorzenObszarow=AlokujObszar(0,Wektor(0,0,0),0,O_NAKT,NULL);


	DodajPunkt(KorzenPunktow,10.0,P_NORM|P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
   	//DodajObszar(KorzenObszarow,1.0,Wektor(0.02,0,0),0.9,O_AKT,NULL);


#define a 0.1 


//	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_NRYS,Wektor(0,0.0,0),Wektor(0,0,0),0);		// 1


	DodajPunkt(KorzenPunktow,1.0,P_ZAW,Wektor(0,4*a,0),Wektor(0,0,0),0);		// 1

	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(2*a,0,0),Wektor(0,0,0),0);		// 2
	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(-2*a,0.0,0),Wektor(0,0,0),0);	// 3
	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(0.0,a,0),Wektor(0,0,0),0);		// 4
	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(0.0,-a,0),Wektor(0,0,0),0);		// 5

 	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(1.2*a,-3.4*a,0),Wektor(0,0,0),0);		// 6
 	DodajPunkt(KorzenPunktow,1.0,P_NORM,Wektor(-1.2*a,-3.4*a,0),Wektor(0,0,0),0);		// 7






//	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(1),P(2));
//	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(1),P(3));
//	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(1),P(4));
//	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(1),P(5));


	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(1),P(4));

	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(5),P(4));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(2),P(4));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(4),P(3));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(3),P(5));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(5),P(2));


	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(6),P(5));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(6),P(2));

	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(7),P(5));
	DodajSprezyne(KorzenSprezyn,24.0f,0,180.0f,P(7),P(3));

	DodajSprezyne(KorzenSprezyn,24.0f,0,40.0f,P(6),P(7));






	spre=KorzenSprezyn->prawy->prawy->prawy->prawy->prawy->prawy->prawy;
	dlug=spre->dlugosc;
	st=0;





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


int k=0;
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


	  /*
	spre->dlugosc += sin(st)/2549.0f;
	spre->prawy->dlugosc += sin(st/2)/2539.0f;
	spre->prawy->prawy->dlugosc += cos(st)/2539.0f;
	spre->prawy->prawy->prawy->dlugosc -= cos(st)/2539.0f;

		*/

//	spre->prawy->prawy->dlugosc += sin(st)/9.0f;
//	spre->prawy->prawy->prawy->dlugosc += sin(st)/9.0f;



	st=st+0.01;

 
}


