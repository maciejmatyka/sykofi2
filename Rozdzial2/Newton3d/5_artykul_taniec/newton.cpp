
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "wektory.h"

#include "punkty.h"

#include "fizyka.h"

#include "view.h"


#include "kolizje.h"
//#include "newton.h"




int szer=250,wys=250;

float xmin=-0.9,xmax=0.9,ymin=-0.9,ymax=0.9, zmin = -0.9, zmax = 0.9;


float dt=0.004;//5;//5;


float tlumienie = 1.0;


Wektor Graw(0,-4.0,0);


Punkt *KorzenPunktow;
Sprezyna *KorzenSprezyn;

Obszar *KorzenObszarow;





//#define fullscreen



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







double dlug4;		// dlugosc sprezyny ktora animujemy
double dlug3;		// dlugosc sprezyny ktora animujemy
double dlug2;		// dlugosc sprezyny ktora animujemy
double dlug;		// dlugosc sprezyny ktora animujemy
Sprezyna *spre;		// sprezyna ktora animujemy
double st;

 

int main(int argc, char **argv)
{

	int i,j;
	Punkt *roboczyP2;


	KorzenPunktow=AlokujPunkt(1,P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
	KorzenSprezyn=AlokujSprezyne(0,0,0,KorzenPunktow,KorzenPunktow,0);
	KorzenObszarow=AlokujObszar(0,Wektor(0,0,0),0,O_NAKT,NULL);


	DodajPunkt(KorzenPunktow,10.0,P_NORM|P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);

#define a 0.13 




  	DodajPunkt(KorzenPunktow,1.0,P_ZAW,Wektor(0,4*a,0),Wektor(0,0,0),0);		// 1


	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_WEKT,Wektor(1.4*a,0,0),Wektor(0,0,0),0);		// 2
	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_WEKT,Wektor(-1.4*a,0.0,0),Wektor(0,0,0),0);	// 3
	DodajPunkt(KorzenPunktow,0.1,P_NORM|P_WEKT,Wektor(0.0,2.5*a*0.01,0),Wektor(0,0,0),0);		// 4   szyja

	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_WEKT,Wektor(0.0,-2.5*a,0),Wektor(0,0,0),0);		// 5

 	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_WEKT,Wektor(1.5*a,-2.5*1.8*a,0),Wektor(0,0,0),0);		// 6	// przedkolano
 	DodajPunkt(KorzenPunktow,1.0,P_NORM|P_WEKT,Wektor(-1.5*a,-2.5*1.8*a,0),Wektor(0,0,0),0);		// 7

	DodajPunkt(KorzenPunktow,0.081,P_NORM|P_WEKT,Wektor(2.8*a,0.1*a,0),Wektor(0,0,0),0);		// 8 // lokcie		
	DodajPunkt(KorzenPunktow,0.081,P_NORM|P_WEKT,Wektor(-2.8*a,0.1*a,0),Wektor(0,0,0),0);		// 9

 	DodajPunkt(KorzenPunktow,2.8,P_NORM|P_WEKT,Wektor(2.0*a,-2.5*3.0*a,0),Wektor(0,0,0),0);		// 10	stopy
 	DodajPunkt(KorzenPunktow,2.8,P_NORM|P_WEKT,Wektor(-2.0*a,-2.5*3.0*a,0),Wektor(0,0,0),0);		// 11

	DodajPunkt(KorzenPunktow,0.1,P_NORM|P_WEKT,Wektor(0.0,2.5*a*0.5,0),Wektor(0,0,0),0);		// 12   glowa

   	DodajObszar(KorzenObszarow,0.1,Wektor(0,0,0),0.5,O_AKT|O_RYS,Ostatni(KorzenPunktow));
												
	DodajPunkt(KorzenPunktow,0.021,P_NORM|P_WEKT,Wektor(3.7*a,0.3*a,0),Wektor(0,0,0),0);		// 13 // rece		
	DodajPunkt(KorzenPunktow,0.021,P_NORM|P_WEKT,Wektor(-3.7*a,0.3*a,0),Wektor(0,0,0),0);		// 14





	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(6),P(5),S_OK);			// *
	DodajSprezyne(KorzenSprezyn,6.0f,0,14.0f,P(6),P(7),S_OK|S_NRYS);	// *
	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(7),P(5),S_OK);			// *


	DodajSprezyne(KorzenSprezyn,4.0f,0,67.0f,P(8),P(2),S_OK);		// tu³ów
	DodajSprezyne(KorzenSprezyn,4.0f,0,67.0f,P(9),P(3),S_OK);		// tu³ów

	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(10),P(11),S_OK|S_NRYS);		// kostka - kostka
	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(10),P(11),S_OK|S_NRYS);		// kostka - kostka



 




	DodajSprezyne(KorzenSprezyn,1.0f,0,1.0f,P(8),P(5),S_OK|S_NRYS);		// lokcie - srodek
	DodajSprezyne(KorzenSprezyn,1.0f,0,1.0f,P(9),P(5),S_OK|S_NRYS);		//


	DodajSprezyne(KorzenSprezyn,4.0f,0,173.0f,P(13),P(8),S_OK);		// rece - przedramie
	DodajSprezyne(KorzenSprezyn,4.0f,0,173.0f,P(14),P(9),S_OK);		//
	DodajSprezyne(KorzenSprezyn,4.0f,0,173.0f,P(13),P(12),S_OK|S_NRYS);		// rece - glowa
	DodajSprezyne(KorzenSprezyn,4.0f,0,173.0f,P(14),P(12),S_OK|S_NRYS);		//






	DodajSprezyne(KorzenSprezyn,1.0f,0,160.0f,P(7),P(12),S_OK|S_NRYS);	 // * 2 utrzymuja glowe
 	DodajSprezyne(KorzenSprezyn,1.0f,0,170.0f,P(12),P(6),S_OK|S_NRYS);



 	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(10),P(7),S_OK|S_NRYS);		// dod. kol - kostka krzyz
	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(11),P(6),S_OK|S_NRYS);		// 


	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(6),P(10),S_OK);		// kolano - kostka
	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(7),P(11),S_OK);		//
 
	DodajSprezyne(KorzenSprezyn,4.0f,0,180.0f,P(4),P(12),S_OK);		// szyja - glowa
	DodajSprezyne(KorzenSprezyn,4.0f,0,120.0f,P(3),P(12),S_OK|S_NRYS);		// szyja - glowa
	DodajSprezyne(KorzenSprezyn,4.0f,0,110.0f,P(2),P(12),S_OK|S_NRYS);		// szyja - glowa
	DodajSprezyne(KorzenSprezyn,4.0f,0,110.0f,P(3),P(4),S_OK);		// szyja - glowa
	DodajSprezyne(KorzenSprezyn,4.0f,0,120.0f,P(2),P(4),S_OK);		// szyja - glowa


		

	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(5),P(4),S_OK);		// tu³ów


 	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(3),P(5),S_OK|S_NRYS);
	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(5),P(2),S_OK|S_NRYS);

	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(6),P(5),S_OK);

	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(6),P(2),S_OK|S_NRYS);


//	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(7),P(5),S_OK|S_NRYS);
	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(7),P(3),S_OK|S_NRYS);





//	DodajSprezyne(KorzenSprezyn,4.0f,0,67.0f,P(8),P(2),S_OK);		// tu³ów
//	DodajSprezyne(KorzenSprezyn,4.0f,0,67.0f,P(9),P(3),S_OK);		// tu³ów
 //	DodajSprezyne(KorzenSprezyn,1.0f,0,0.2f,P(8),P(7),S_OK|S_NRYS);		// ramie 1
 //	DodajSprezyne(KorzenSprezyn,1.0f,0,0.2f,P(9),P(6),S_OK|S_NRYS);		// ramie 2


										   
 	DodajSprezyne(KorzenSprezyn,4.0f,0,80.0f,P(10),P(11),S_OK|S_NRYS);		// kostka - kostka
  	DodajSprezyne(KorzenSprezyn,4.0f,0,90.0f,P(10),P(5),S_OK|S_NRYS);		// kostka - srodek
  	DodajSprezyne(KorzenSprezyn,4.0f,0,20.0f,P(11),P(5),S_OK|S_NRYS);		// kostka - srodek


	spre=KorzenSprezyn->prawy;
	dlug=spre->dlugosc;
	dlug2=spre->prawy->dlugosc;
	dlug3=spre->prawy->prawy->prawy->dlugosc;

	dlug4=spre->prawy->prawy->prawy->prawy->prawy->dlugosc;
	st=0;





	glutInit(&argc, argv);

#ifndef fullscreen

    glutInitWindowPosition(150, 150); glutInitWindowSize( szer, wys);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    if (glutCreateWindow("Newton") == GL_FALSE) {exit(1);}

#else
 	glutGameModeString("640x480:16@60");
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutEnterGameMode();
    glutFullScreen();
#endif

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

void dodaj_punkt(void)
{

	spre->dlugosc = dlug - dlug * sin(st)/1.5f;
   	spre->prawy->dlugosc = dlug2 - dlug2 * cos(st)/1.0f;
   	spre->prawy->prawy->dlugosc = dlug + dlug * sin(st*0.4)/1.5f;

   	spre->prawy->prawy->prawy->dlugosc = dlug3 - dlug3 * cos(st)/4.4f;			// reka - stopa
   	spre->prawy->prawy->prawy->prawy->dlugosc = dlug3 - dlug3 * sin(st)/4.4f;

   	spre->prawy->prawy->prawy->prawy->prawy->dlugosc = dlug4 - dlug4 * sin(st)/2.0f;
   	spre->prawy->prawy->prawy->prawy->prawy->prawy->dlugosc = dlug4 - dlug4 * sin(st)/2.0f;

//	st=st+(rand()/(float)RAND_MAX)*0.1;
	st=st+0.04;
	return;
}



void dodaj_punkt2(void)
{

	return;

}


int motion = 1;

void dodaj_punktS(void)
{


//	P(10)->v.y += 0.3;
//	P(11)->v.y += 0.3;


	
	motion = 1;


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

 	Kolizje(KorzenPunktow,KorzenObszarow);

 	WyznaczSily(KorzenPunktow,KorzenSprezyn,KorzenObszarow);
	RozwiazMidPoint_k2(KorzenPunktow,dt);

#endif


// rozpatruj kolizje


	PrzesunObszary(KorzenObszarow);
 	Kolizje(KorzenPunktow,KorzenObszarow);
	PrzesunObszary(KorzenObszarow);




	if(motion)
	dodaj_punkt();
	
}


