
//#include <iostream>

#include <stdio.h>
#include <math.h>
#include "wektory.h"




// metody



double Wektor::dlugosc()
{
	return sqrt( (*this) * (*this));			// szybciej niz iloczyn skalarny
}


void Wektor::normuj()
{
	double d = dlugosc();

	if(d)
		*this = *this * (1/d);
}
	


// definicje operatorów


// + dodawanie skladowych
Wektor Wektor::operator+(Wektor const &w)
{
	return Wektor(x + w.x, y + w.y, z + w.z);
}

// - odejmowanie skladowych
Wektor Wektor::operator-(Wektor const &w)
{
	return Wektor(x - w.x, y - w.y, z - w.z);
}

// * iloczyn wektor - skalar
Wektor Wektor::operator*(double liczba)
{
	return Wektor(x*liczba, y*liczba, z*liczba);
}

// iloczyn skalarny
double Wektor::operator*(Wektor const &w)
{
	return (x*w.x + y*w.y + z*w.z);
}

// iloczyn wektorowy (bo modulo z wektora nie istnieje, wiec wykorzystuje operator modulo)
Wektor Wektor::operator%(Wektor const &w)
{
	return Wektor(y*w.z - z*w.y, z*w.x - x*w.z, x*w.y - y*w.x);
}

// rownosc dwu wektorow
void Wektor::operator=(Wektor const &w)
{
	x = w.x;
	y = w.y;
	z = w.z;
}

  












