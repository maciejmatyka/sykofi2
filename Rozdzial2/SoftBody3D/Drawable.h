
#ifndef __I_DRAWABLE_H_
#define __I_DRAWABLE_H_


/**
 * @short Property of some objects. It tells us that object can be drawed in OpenGL.
 *
 *
 * @detailed Contains one simple virtual procedure - glVisualize()
 * 
 * @author Maciej Matyka
 * http://panoramix.ift.uni.wroc.pl/~maq
 *
 */

class Drawable
{
public:

	Drawable(){}

	virtual void glVisualize(void) const { };	


};




#endif
