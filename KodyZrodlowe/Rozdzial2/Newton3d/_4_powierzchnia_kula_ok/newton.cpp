
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"

#include "view.h"


#include "kolizje.h"
#include "newton.h"




int szer=650,wys=650;

float xmin=-0.9,xmax=0.9,ymin=-0.9,ymax=0.9, zmin = -0.9, zmax = 0.9;


float dt=0.0015;//5;//5;


float tlumienie = 0.6;


Wektor Graw(0,-10,0);


Punkt *KorzenPunktow;
Sprezyna *KorzenSprezyn;

Obszar *KorzenObszarow;


//#define lina

int main(int argc, char **argv)
{

	int i,j;
	Punkt *roboczyP2;


	KorzenPunktow=AlokujPunkt(1,P_NRYS,Wektor(0,0,0),Wektor(0,0,0),0);
	KorzenSprezyn=AlokujSprezyne(0,0,0,KorzenPunktow,KorzenPunktow);
	KorzenObszarow=AlokujObszar(0,Wektor(0,0,0),0,O_NAKT,NULL);



   //	DodajObszar(KorzenObszarow,0.25,Wektor(0,0.2,0),1,O_AKT,NULL);


   	//DodajObszar(KorzenObszarow,0.2,Wektor(0,0,0),0.3,O_AKT,NULL);


	int flag;


#define kd 5.0f
#define ks 140.0f
#define siz 0.0f

		flag = P_NORM;

for(j=0;j<n;j++)
for(i=0;i<n;i++)
{
	if(j>n-2 || i>n-2 || j<2 || i<2)
//	if(j==n-1)
		flag = P_ZAW;
	else
		flag = P_NORM;

		DodajPunkt(KorzenPunktow,0.25,flag,Wektor((float)(i-n/2)/(float)(n),0,(float)(j-n/2)/(float)(n)),Wektor(0,0,0),0);
     	DodajObszar(KorzenObszarow,0.05,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));
}


	Punkt *roboczy;


for(int k=0;k<2;k++)
{



	roboczy = KorzenPunktow->prawy;	



// jestesmy na korzeniu, po kolei laczymy wiersze
	for(j=0;j<n;j++)
	{
		for(i=0;i<n-1;i++)
		{
			DodajSprezyne(KorzenSprezyn,kd,siz,ks,roboczy,roboczy->prawy);
			roboczy=roboczy->prawy;
		}
			roboczy=roboczy->prawy;
	}




	roboczy = KorzenPunktow->prawy;	
// teraz po³¹czenie co dwa
	for(j=0;j<n-1;j++)
	{
		for(i=0;i<n-2;i++)
		{
			DodajSprezyne(KorzenSprezyn,kd,siz,ks,roboczy,roboczy->prawy->prawy);
			roboczy=roboczy->prawy;
		}
			roboczy=roboczy->prawy->prawy;
	}



 roboczy = KorzenPunktow->prawy;	
/// teraz po³¹czenie co 3
	for(j=0;j<n-2;j++)
	{
		for(i=0;i<n-3;i++)
		{
			DodajSprezyne(KorzenSprezyn,kd,siz,ks,roboczy,roboczy->prawy->prawy->prawy);
			roboczy=roboczy->prawy;
		}
			roboczy=roboczy->prawy->prawy->prawy;
	}


 roboczy = KorzenPunktow->prawy;	
/// teraz po³¹czenie co 4
	for(j=0;j<n-3;j++)
	{
		for(i=0;i<n-4;i++)
		{
			DodajSprezyne(KorzenSprezyn,kd,siz,ks,roboczy,roboczy->prawy->prawy->prawy->prawy);
			roboczy=roboczy->prawy;
		}
			roboczy=roboczy->prawy->prawy->prawy->prawy;
	}

 roboczy = KorzenPunktow->prawy;	
/// teraz po³¹czenie co 5
	for(j=0;j<n-4;j++)
	{
		for(i=0;i<n-5;i++)
		{
			DodajSprezyne(KorzenSprezyn,kd,siz,ks,roboczy,roboczy->prawy->prawy->prawy->prawy->prawy);
			roboczy=roboczy->prawy;
		}
			roboczy=roboczy->prawy->prawy->prawy->prawy->prawy;
	}





// pionowo co 1
for(i=0;i<n;i++)
	for(j=0;j<n-1;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+1)*n+i+1));
    }

// pionowo co 2
	for(i=0;i<n;i++)
	for(j=0;j<n-3;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+2)*n+i+1));
    }

// pionowo co 3
	for(i=0;i<n;i++)
	for(j=0;j<n-5;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+3)*n+i+1));
    }

// pionowo co 4
	for(i=0;i<n;i++)
	for(j=0;j<n-6;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+4)*n+i+1));
    }


// pionowo co 5
	for(i=0;i<n;i++)
	for(j=0;j<n-7;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+5)*n+i+1));
    }







// krzyz 1
for(i=0;i<n-1;i++)
	for(j=0;j<n-1;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+1)*n+i+2));
    }


// krzyz 1_2
for(i=0;i<n-2;i++)
	for(j=0;j<n-2;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+2)*n+i+3));
    }



// krzyz 2
for(i=1;i<n;i++)
	for(j=0;j<n-1;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,j*n+i+1),PrzesunNaKorzeniu(KorzenPunktow,(j+1)*n+i));
    }

// krzyz 2_2
for(i=2;i<n-1;i++)
	for(j=1;j<n-1;j++)
	{
		DodajSprezyne(KorzenSprezyn,kd,siz,ks,PrzesunNaKorzeniu(KorzenPunktow,(j-1)*n+i+2),PrzesunNaKorzeniu(KorzenPunktow,(j+1)*n+i));
    }


}



//   	DodajObszar(KorzenObszarow,0.15,Wektor(0,0.2,0),0.9,O_AKT,NULL);



//#define fullscreen



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
	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.4,(float)j/(float)5),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM,Wektor(-0.5+(float)k/(dziel),0,(float)j/(float)5),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));

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
	DodajPunkt(KorzenPunktow,0.1,P_ZAW,Wektor(-0.5+(float)k/(dziel),0.4,(float)j/(float)5),Wektor(0,0,0),0);
	DodajPunkt(KorzenPunktow,0.1,P_NORM,Wektor(-0.5+0.3+(float)k/(dziel),0.4,(float)j/(float)5),Wektor(0,0,0),0);

  	DodajObszar(KorzenObszarow,prsf,Wektor(0,0,0),1.0,O_AKT,Ostatni(KorzenPunktow));

	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
	DodajSprezyne(KorzenSprezyn,8.2,0.4,50.5,roboczyP2->prawy,roboczyP2->prawy->prawy);
//	}

	return;

}



void dodaj_punktS(void)
{


//   	DodajObszar(KorzenObszarow,0.25,Wektor(0,0.2,0),1,O_AKT,NULL);
  			 

	DodajPunkt(KorzenPunktow,12.0,P_NORM,Wektor(0,0.8,0),Wektor(0,-8.0,0),0);
  	DodajObszar(KorzenObszarow,0.2,Wektor(0,0,0),1,O_AKT|O_RYS,Ostatni(KorzenPunktow));


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


