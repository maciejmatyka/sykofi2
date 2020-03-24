
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"

#include "view.h"

#include "kolizje.h"




int szer=800,wys=800;

float xmin=-0.9,xmax=0.9,ymin=-0.9,ymax=0.9, zmin = -0.9, zmax = 0.9;

float dt=0.01;

float tlumienie = 0.8;

Wektor Graw(0,-10,0);

Punkt *KorzenPunktow;
Sprezyna *KorzenSprezyn;

Obszar *KorzenObszarow;


//#define lina


void dodaj_punkt(void);
void dodaj_punktS(void);
void dodaj_punkt2(void);

int main(int argc, char **argv)
{
	int i;
	Punkt *roboczyP2;


	KorzenPunktow=AlokujPunkt(1,P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
	KorzenSprezyn=AlokujSprezyne(0,0,0,KorzenPunktow,KorzenPunktow);
	KorzenObszarow=AlokujObszar(0,Wektor(0,0,0),0,O_NAKT,NULL);

/*
   	DodajObszar(KorzenObszarow,10.94,Wektor(0,0,0),0.8,O_AKT,NULL);


for(int j=0;j<5;j++)
for(i=0;i<5;i++)
{
    DodajPunkt(KorzenPunktow,i,P_NORM,Wektor((float)(i-2)*4.5,0,(float)(j-2)*4),Wektor((rand()/(float)RAND_MAX-0.5)*3,(rand()/(float)RAND_MAX-0.5)*3,(rand()/(float)RAND_MAX-0.5)*3),(rand()/(float)RAND_MAX)*i*0.5);
}
*/




	//Graw = Wektor(0,0,0);

/*
   	for(i=0;i<20;i++)
	{
	   DodajPunkt(KorzenPunktow,1,P_NORM,Wektor(1-(rand()/(double)RAND_MAX)*2,1-(rand()/(double)RAND_MAX)*2,1-(rand()/(double)RAND_MAX)*2),Wektor(1-(rand()/(double)RAND_MAX)*2,1-(rand()/(double)RAND_MAX)*2,1-(rand()/(double)RAND_MAX)*2));
//	   DodajObszar(KorzenObszarow,0.03,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));
	}
*/


#ifdef lina

	for(i=0;i<40;i++)
	{		
		if(i<3 || i>32)
	   		DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.9+(float)i/21.0f,0.1,0),Wektor(0,0,0),0);
	   	else
  			DodajPunkt(KorzenPunktow,0.1,P_NORM,Wektor(-0.9+(float)i/21.0f,0.1,0),Wektor(0,0,0),0);

 	  	DodajObszar(KorzenObszarow,0.01,Wektor(0,0,0),1.0,O_AKT|O_RYS,Ostatni(KorzenPunktow));
   	}
												     

  	for(roboczyP2=KorzenPunktow;roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL; roboczyP2=roboczyP2->prawy)
	{

	if(roboczyP2->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.001,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
					   
						 
	if(roboczyP2->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.0015,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy);

	if(roboczyP2->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.002,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy);

				
	if(roboczyP2->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.0025,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy);


	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.003,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy);


	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.0035,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy);


	if(roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy != NULL)
		DodajSprezyne(KorzenSprezyn,1,0.004,2.5,roboczyP2->prawy,roboczyP2->prawy->prawy->prawy->prawy->prawy->prawy->prawy->prawy);

    }

#endif



	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt();
	dodaj_punkt2();




 

    glutInitWindowPosition(150, 150); 
    glutInitWindowSize( szer, wys);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
//	glutGameModeString("640x480:32@60");
//    glutEnterGameMode();
//    glutFullScreen();

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

void dodaj_punkt(void)
{


//	Punkt *roboczyP;
	Punkt *roboczyP2;


	k=k+1;


//	return;

	int j=0; 



  //	k=0;
   
//	for(int j=-3;j<3;j++)
//	{	
	roboczyP2 = Ostatni(KorzenPunktow);


//	DodajPunkt(KorzenPunktow,1,P_ZAW,Wektor(0,0.4,0),Wektor(0,0,0));
	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.9,(float)j/(float)5),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM,Wektor(-0.5+(float)k/(dziel),0.4,(float)j/(float)5),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT|O_RYS,Ostatni(KorzenPunktow));

	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
//	}


	return;
}



void dodaj_punkt2(void)
{
	Punkt *roboczyP2;

	k=k+1;

  //	k=0;  

	int j=0; 


//  	for(int j=-3;j<3;j++)
//  	{	

	roboczyP2 = Ostatni(KorzenPunktow);

//	DodajPunkt(KorzenPunktow,1,P_ZAW,Wektor(0,0.4,0),Wektor(0,0,0));
	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.9,(float)j/(float)5),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM,Wektor(-0.5+0.3+(float)k/(dziel),0.9,(float)j/(float)5),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT|O_RYS,Ostatni(KorzenPunktow));

	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
//	}

	return;

}



void dodaj_punktS(void)
{
//	DodajPunkt(KorzenPunktow,1,P_NORM,Wektor(0.0,0.8,0),Wektor(0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4));
 
//	DodajPunkt(KorzenPunktow,90.0,P_NORM,Wektor(0.0,0.8,0),Wektor(0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4,0.2 - (rand()/(double)RAND_MAX)*0.4));
	#define SS 0.005
    	DodajPunkt(KorzenPunktow,93.0,P_NORM,Wektor(0+(rand()/float(RAND_MAX))*SS,0.8+(rand()/float(RAND_MAX))*SS,0+(rand()/float(RAND_MAX))*SS),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,0.14,Wektor(0,0,0),1.0,O_AKT|O_RYS,Ostatni(KorzenPunktow));
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


