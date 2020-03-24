
#include "wektory.h"
#include "kolizje.h"

#ifndef __I_PUNKTY_
#define __I_PUNKTY_

#define P_NORM 1			// normalny - fizyka dziala
#define P_ZAW 2				// nie rozwiazuje rownan rochu dla ciala (polozenie = const)
#define P_OBSZ 4            // podczepiony opbszar kolizji
#define P_NRYS 8			// nie rysuj
#define P_WEKT 16           // dolacz wektor

//#define S_OK 1
#define S_PEK 2
#define S_NRYS 4
#define S_RYSL 8


class Punkt 
{

public:
	Punkt()
	{};

	Punkt(int _flaga, float _masa, Wektor _r, Wektor _v) :
	flaga(_flaga), masa(_masa), r(_r), v(_v), prawy(NULL), R(0)
	{};

	int flaga;
	float masa;
	
	Wektor r;
	Wektor v;
	Wektor f;

	Wektor n;		// a jak, mamy 2002 - normalna do punktu. Ha!

	Wektor dr;
	Wektor dv;

	double R;

	double col;

	Punkt *prawy;
};


class Sprezyna
{
public:
	Sprezyna(Punkt *_p1, Punkt *_p2, float _ks, float _kd, float _d, int _f, int _const_lenght, float _p) :
	  punkt1(_p1), punkt2(_p2), ks(_ks), kd(_kd), dlugosc(_d), flag(_f), const_lenght(const_lenght), p(_p)
	  {
		if(!_d)
			dlugosc=(float)(_p1->r-_p2->r).dlugosc();
	  };

	Sprezyna()
	{}

	Punkt *punkt1;
	Punkt *punkt2;
	
	float ks;      // stala sprezystosci
	float kd;	    // stala wygaszania drgan
	float dlugosc;	// dlugosc sprezyny, jesli 0 to liczy sam defaultowo

	int flag;
	int	const_lenght;	// procentowo jak moze sie zmienic
	float p;              // position on the filament  (0-start, 1-end)

	Sprezyna *prawy;

};


#endif




