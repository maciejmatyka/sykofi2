


#ifndef _I_WEKTORY_H_
#define _I_WEKTORY_H_

class Wektor
{

public:

	Wektor(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{


	}

	Wektor() : x(0), y(0), z(0) {}


	~Wektor(){}



/*	void wypisz(void)
{
		//std::cout << " x=" << x << " y=" << y << " z=" << z << "\n"; 
	}
*/

// operatory
	
	Wektor operator+(Wektor const &);
	Wektor operator-(Wektor const &);
	double operator*(Wektor const &);	// iloczyn skalarny
	Wektor operator*(double);	// iloczyn skalar - wektor
	Wektor operator%(Wektor const &);	// iloczyn wektorowy
	void   operator=(Wektor const &);   // rownosc dwu wektorow




// metody

	double dlugosc();			// zwraca dlugosc wektora
	void normuj();				// normuje wektor


	double x,y,z;		// sk³adowe wektora		   - potem zmienic na private bez dostepu z zewnatrz

private:
	

};


#endif
