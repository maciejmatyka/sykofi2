

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "glucik.h"
#include "fluid.h"


static void Idle( void )
{

	CalculateSurface();


	glutPostRedisplay();
}





static void Display( void )
{
   glClear( GL_COLOR_BUFFER_BIT );



   DrawFluidSurface();


  
   glutSwapBuffers();
}


static void Reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluOrtho2D(-1,1,0,1);
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
}


static void Key( unsigned char key, int x, int y )
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
   glutPostRedisplay();
}


static void SpecialKey( int key, int x, int y )
{
   switch (key) {
      case GLUT_KEY_UP:

  
         break;
      case GLUT_KEY_DOWN:
         break;
      case GLUT_KEY_LEFT:
         break;
      case GLUT_KEY_RIGHT:
         break;
   }
   glutPostRedisplay();
}


static void Init( void )
{
	glClearColor(1,1,1,0);
	InitSurface();


}


extern double DT;
extern double G; 

int main( int argc, char *argv[] )
{


	printf("%f < %f\n",DT,1.0f/sqrt(G));


   glutInit( &argc, argv );
   glutInitWindowPosition( 200, 200 );
   glutInitWindowSize( WIDTH, HEIGHT );

   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

   glutCreateWindow("Fluid Surface");

   Init();

   glutReshapeFunc( Reshape );
   glutKeyboardFunc( Key );
   glutSpecialFunc( SpecialKey );
   glutDisplayFunc( Display );
   glutIdleFunc( Idle );

   glutMainLoop();
   
   return 0;
}
