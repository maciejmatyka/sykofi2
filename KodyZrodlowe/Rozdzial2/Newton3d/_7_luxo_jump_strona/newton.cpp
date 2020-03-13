
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




int szer=600,wys=600;

float xmin=-10.9,xmax=10.9,ymin=-0.9,ymax=0.9, zmin = -0.9, zmax = 0.9;


float dt=0.008;//5;//5;


float tlumienie = 0.1;


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

int motionchange=0;

void dodaj_punkt2(void)		// f
{
	motionchange=150;
//	P(7)->v = Wektor(-9,16,0);

}


void dodaj_punktS(void)
{

//Wektor skok = Wektor(-0.7,1.8,(rand()/(float)RAND_MAX-0.5)*0.05);

/*	P(1)->v = P(1)->v + skok;
	P(2)->v = P(2)->v + skok;
	P(3)->v = P(3)->v + skok;
	P(4)->v = P(4)->v + skok;
*/

//	P(7)->v = P(7)->v + skok * 0.5;


	motionchange=1;
//	P(7)->r = P(7)->r - Wektor(-0.002,0.03,0); 
   //	  P(6)->r = P(6)->r - Wektor(0,0.1,0); 


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

#define a 0.23 
#define y -0.8





// podloze

  	DodajPunkt(KorzenPunktow,1.6,P_NORM,Wektor(-a*0.6,y,-a*0.6),Wektor(0,0,0),0);	// 1
  	DodajPunkt(KorzenPunktow,1.6,P_NORM,Wektor(a*0.6,y,-a*0.6),Wektor(0,0,0),0);		// 2
  	DodajPunkt(KorzenPunktow,1.6,P_NORM,Wektor(a*0.6,y,a*0.6),Wektor(0,0,0),0);		// 3
  	DodajPunkt(KorzenPunktow,1.6,P_NORM,Wektor(-a*0.6,y,a*0.6),Wektor(0,0,0),0);		// 4
  	DodajPunkt(KorzenPunktow,0.5,P_NORM,Wektor(0.0f,y,0),Wektor(0,0,0),0);		// 5

// szyja
  	DodajPunkt(KorzenPunktow,0.5,P_NORM,Wektor(-(float)a*0.2f,y+a*1.5,0),Wektor(0,0,0),0);		// 6

// glowa
  	DodajPunkt(KorzenPunktow,0.8,P_NORM,Wektor(-(float)a*0.9f,y+a*1.8,0),Wektor(0,0,0),0);		// 7
	DodajObszar(KorzenObszarow,0.05,Wektor(0,0,0),0.0,O_AKT|O_RYS,P(7));



// poloczenia podloze

  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(1),P(3),S_OK|S_NRYS);				
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(2),P(4),S_OK|S_NRYS);	   

  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(1),P(2),S_OK|S_NRYS);				
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(2),P(3),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(3),P(4),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(4),P(1),S_OK|S_NRYS);	   


// polaczenia podloze/srodek

  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(1),P(5),S_OK|S_NRYS);				
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(2),P(5),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(3),P(5),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,180.0f,P(4),P(5),S_OK|S_NRYS);	   


// szyja podloze

  	DodajSprezyne(KorzenSprezyn,14.0f,0,280.0f,P(1),P(6),S_OK|S_NRYS);				
  	DodajSprezyne(KorzenSprezyn,14.0f,0,280.0f,P(2),P(6),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,280.0f,P(3),P(6),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,280.0f,P(4),P(6),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,14.0f,0,280.0f,P(5),P(6),S_OK|S_RYSL);	   



// glowa - podloze
  	DodajSprezyne(KorzenSprezyn,14.0f,0,290.0f,P(6),P(7),S_OK|S_RYSL);		
  	DodajSprezyne(KorzenSprezyn,12.0f,0,50.0f,P(4),P(7),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,12.0f,0,50.0f,P(1),P(7),S_OK|S_NRYS);
  	DodajSprezyne(KorzenSprezyn,12.0f,0,50.0f,P(2),P(7),S_OK|S_NRYS);	   
  	DodajSprezyne(KorzenSprezyn,12.0f,0,50.0f,P(3),P(7),S_OK|S_NRYS);
   














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





//int motion = 0;



//#define Euler
#define MidPoint

void Motions(void)
{
	if(motionchange!=0)
	{
		motionchange--;
		P(7)->v = Wektor(-0.32,0.8,0);
	}
}


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

	Motions();

	PrzesunObszary(KorzenObszarow);
 	Kolizje(KorzenPunktow,KorzenObszarow);
	PrzesunObszary(KorzenObszarow);



	//PrzesunObszary(KorzenObszarow);




	//if(motion)
	//dodaj_punkt();
	
}


