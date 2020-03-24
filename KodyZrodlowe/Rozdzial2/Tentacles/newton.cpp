#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "wektory.h"
#include "punkty.h"
#include "fizyka.h"
#include "view.h"
#include "kolizje.h"
#include "newton.h"
#include "cloth.h"
//#include "object.h"

#include "Bitmap.h"
#include "Texture.h"

const int szer=800,wys=600;

float xmin=-1.9,xmax=1.9,ymin=-1.5,ymax=1.9, zmin = -1.9, zmax = 1.9;

float dt=0.1;//5;//5;
float tlumienie = 0.9;

Wektor Graw(0,0,0);//-0.005,0);
Wektor Wind(0.2,0.0,0.00);

#ifndef PI
#define PI 3.141592654
#endif

unsigned int tekstura1;
unsigned int tekstura2;
unsigned int tekstura3;
void LoadTextures(void);

//	Cloth(nx,ny,ks,kd,lenght_constrain,x0,y0,z0,x1,y1,z1,mass)

// parzyste nx,ny

//Cloth cloth1(5,5, 22, 0.15,  5,   Wektor(-0.3,0.3,0), Wektor(0.3,-0.3,0),   0.45);		// zaw
//Cloth cloth3(14,14, 122, 0.15,  5,   Wektor(-0.9,0.3,0), Wektor(-0.25,-0.3,0),   0.45);		// zaw
//Cloth cloth3(16,20, 80,0.1,   3,  Wektor(-0.4,0.3,0), Wektor(0.4,-0.3,0),     0.3);		// flaga
//Cloth cloth3(80,40, 80,1.1,   1,  Wektor(-0.4,0.3,0), Wektor(0.4,-0.3,0),     1.3);		// flaga
//Cloth cloth3(10,1340, 120,3.1,   1,  Wektor(-0.4,0.3,0), Wektor(0.4,-0.3,0),     9.3);		// flaga1
Cloth cloth3(10,1002, 120,3.1,   1,  Wektor(-0.4,0.3,0), Wektor(0.4,-0.3,0),     9.3);		// flaga1
//Cloth cloth3(10,340, 120,3.1,   1,  Wektor(-0.4,0.3,0), Wektor(0.4,-0.3,0),     9.3);		// flaga1




// robocze
//Cloth cloth1(2,2, 22, 0.15,  5,   Wektor(-0.3,0.3,0), Wektor(0.3,-0.3,0),   0.45);		// zaw
//Cloth cloth2(2,2, 122, 0.15,  5,   Wektor(-0.9,0.3,0), Wektor(-0.25,-0.3,0),   0.45);		// zaw


//#define fullscreen

int main(int argc, char **argv)
{
	int i,j;

// zmieniamy powierzchnie - zawieszamy dwa punkty p[0,0] i p[nx,0]


/*	for(j=0;j<cloth3.ny;j+=1)
	{
		if(j<=0 || j>=cloth3.ny-1)
		cloth3.Points[0][j]->flaga |=P_ZAW;
	}*/

	

	glutInit(&argc, argv);
	
	
	
	
    //	http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
	

#ifndef fullscreen

    glutInitWindowPosition(250, 250); glutInitWindowSize( szer, wys);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    if (glutCreateWindow("Newton") == GL_FALSE) {exit(1);}

#else
 	glutGameModeString("640x480:16@60");
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutEnterGameMode();
    glutFullScreen();
#endif

    Init();
	LoadTextures();


    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  //  GLuint FramebufferName = 0;
//    glGenFramebuffers(1, &FramebufferName);
//    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);




    glutKeyboardFunc(Key);
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(idle);
    glutMainLoop();
    
	return (0);
}


//#define Euler
#define MidPoint




float t=0;
void Timestep(void)
{

//	t=t+(PI/180)/2;
//	Wind=Wektor(sin(t)*10.0,0,sin(t/2)*5);


//	Wind = Wektor(0.1,-0.1,-0.1);
#ifdef Euler

//	WyznaczSily(cloth1,dt);
//	RozwiazEuler(cloth1,dt);
//	CCC_Springs(cloth1);

//	WyznaczSily(cloth2,dt);
//	RozwiazEuler(cloth2,dt);
//	CCC_Springs(cloth2);

#endif
#ifdef MidPoint

//	WyznaczSily(cloth1,dt);
//	WyznaczSily(cloth2,dt);
	WyznaczSily(cloth3,dt);
    Kolizje(cloth3);

//	RozwiazMidPoint_k1(cloth1,dt);
//	RozwiazMidPoint_k1(cloth2,dt);
	RozwiazMidPoint_k1(cloth3,dt);

//	CCC_Springs(cloth1);4
//	CCC_Springs(cloth2);
	CCC_Springs(cloth3);

//	Kolizje(cloth3);


//	WyznaczSily(cloth1,dt);
//	WyznaczSily(cloth2,dt);
    WyznaczSily(cloth3,dt);
    Kolizje(cloth3);
    
//	RozwiazMidPoint_k2(cloth1,dt);
//	RozwiazMidPoint_k2(cloth2,dt);
	RozwiazMidPoint_k2(cloth3,dt);

//	CCC_Springs(cloth1);
//	CCC_Springs(cloth2);
	CCC_Springs(cloth3);
	
    	    
	
	//Kolizje(cloth3);


#endif	

}

void LoadTextures(void)
{
	glGenTextures(1,&tekstura1);
	glGenTextures(1,&tekstura2);
	glGenTextures(1,&tekstura3);

	glBindTexture(GL_TEXTURE_2D,tekstura1);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	TextureLoadMipmap("obrus.bmp");
	TextureLoadMipmap("pl.bmp");

	glBindTexture(GL_TEXTURE_2D,tekstura2);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//	TextureLoadMipmap("flaga.bmp");
	TextureLoadMipmap("uwr.bmp");
	TextureLoadMipmap("pl.bmp");

	glBindTexture(GL_TEXTURE_2D,tekstura3);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	TextureLoadMipmap("env.bmp");
	TextureLoadMipmap("pl.bmp");




//	cloth1.BindTextureOnIt(tekstura1);
//	cloth2.BindTextureOnIt(tekstura3);
	cloth3.BindTextureOnIt(tekstura2);

}
