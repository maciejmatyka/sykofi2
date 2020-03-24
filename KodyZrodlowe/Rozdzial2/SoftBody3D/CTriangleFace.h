
#ifndef __CTRIANGLEFACE_I_
#define __CTRIANGLEFACE_I_

#include "CMaterialPoint.h"

/**
 * @short Simple definition of a triangle. Keeps pointers to three points from SoftObject 
 * point vector.
 *
 * 
 * @author Maciej Matyka
 * http://panoramix.ift.uni.wroc.pl/~maq
 *
 */


class CTriangleFace
{

public:
	
	CTriangleFace(CMaterialPoint *p1, CMaterialPoint *p2, CMaterialPoint *p3) : _p1(p1), _p2(p2), _p3(p3)
	{}

	CTriangleFace()
	{}


	/**
	 * Calculate normal vector to a triangle and its field 
	 */

	void doCalculateNormalAndField(void)
	{
		// use vector multiplication

		CVector3 n=(_p1->_r - _p2->_r) % (_p1->_r - _p3->_r);

		// field as n length

		_Field = n.getLength();

		// normalize the vector

		n.doNormalize();
		
		// set normal vector

		_n = n;

		_p1->_n = (_p1->_n + _n * (1.0/6.0));
		_p2->_n = (_p2->_n + _n * (1.0/6.0));
		_p3->_n = (_p3->_n + _n * (1.0/6.0));
	}


	/**
	 * Clear points normals                                                                     
	 */

	void doClearPointsNormals(void)
	{
		_p1->_n = CVector3(0,0,0);
		_p2->_n = CVector3(0,0,0);
		_p3->_n = CVector3(0,0,0);
	}


public:
	
	CMaterialPoint	*_p1;		// first point of the Triangle
	CMaterialPoint	*_p2;
	CMaterialPoint	*_p3;


	CVector3 _n;			// normal to a triangle
	float _Field;		// field of the triangle 
	CVector3 _Force;		// force acting on a tringle (wind etc.)
};



#endif