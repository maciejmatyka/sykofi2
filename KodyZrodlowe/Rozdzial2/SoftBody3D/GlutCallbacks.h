#ifndef _I_GlutCallbacks_H_
#define _I_GlutCallbacks_H_

/**
 * Namespace GLUT is used to keep all procedures (mostly callbacks) of GLUT Api.
 *
 */

namespace GLUT
{


	/**
	 * A 'must be' include.	
	 */

#include <GL/glut.h> 
//#include <windows.h>

	/**
	 * GLUT standard callbacks.	
	 */

	void Motion (int x, int y);
	void Mouse (int button, int state, int x, int y);
	void Key(unsigned char key, int x, int y);
	void Init(void);
	void Reshape(int width, int height);
	void Draw(void);
	void Idle(void);

	/**
	 * 'Global' in 'GLUT' namespace variables
	 */

	extern int width;
	extern int height;

}



#endif