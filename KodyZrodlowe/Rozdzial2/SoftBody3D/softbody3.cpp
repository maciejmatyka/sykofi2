
/**************************************************************
 *
 * Soft body 3.0
 *
 *
 * by Maciej Matyka,
 * http://panoramix.ift.uni.wroc.pl/~maq
 *
 * Based on publication:
 *
 * Matyka, M., Ollila, M. "A Pressure Soft Body Model", Proc. of SIGRAD, 2003
 *
 * (see pdf on author page)
 *
 * g++ *.cpp -lglut -lGLU -lGL
 */





#include <iostream>
using namespace std;


#include "CVector3.h"

#include "CMaterialPoint.h"
#include "CHookeSpring.h"
#include "CSoftObject.h"

#include "GlutCallbacks.h"



/*
 *	Create GLOBAL soft body object
 */

CSoftObject mySoftBody(
							CVector3(0,0,0),				// initial translation in space
							0.9f,							// rescaling loaded object
							1.0f,							// mass of single mass point
							221100,							// ks spring coefficient
							30.0f,							// kd damping factor
							"/home/maq/SYKOFI2/codes/SoftBody3D/Objects/OBJEKT2.ASC",			// object file (.asc 3d studio file)
							1,								// id of the body (for collisions between bodies)
							1826320							// initial pressure of the body
						);
int gravityonoff = 0;

/*
 *	Main function
 */

int main(int pargc, char** argv)
{
	GLUT::glutInit(&pargc, argv);


	mySoftBody.setGravity(CVector3(0,-1300,0));



	/*
	 * Yep, I know, that "using namespace" exist
	 */

	GLUT::width = 600;
	GLUT::height = 600;


	/*
	 *	Print Debug info
	 */

	cout << "Window Width: " << GLUT::width << endl;
	cout << "Window Height: " << GLUT::height << endl << endl;
	cout << "Loaded: " << mySoftBody.iNumf() << " faces..." << endl;
	cout << "Loaded: " << mySoftBody.iNump() << " points..." << endl;
	cout << "Created: " << mySoftBody.iNums() << " spring connections..." << endl;


	GLUT::glutInitWindowPosition(200, 200);
	GLUT::glutInitWindowSize( GLUT::width, GLUT::height);
	GLUT::glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

	if (GLUT::glutCreateWindow("Soft Body 3.0 by Maciej Matyka, http://panoramix.ift.uni.wroc.pl/~maq") == GL_FALSE)
	{
		::exit(0);
	}


	GLUT::glutMouseFunc(GLUT::Mouse);
	GLUT::glutMotionFunc(GLUT::Motion);
	GLUT::glutKeyboardFunc(GLUT::Key);
	GLUT::glutReshapeFunc(GLUT::Reshape);
	GLUT::glutDisplayFunc(GLUT::Draw);
	GLUT::glutIdleFunc(GLUT::Idle);

	GLUT::glutMainLoop();



	return (2==3);
}






















