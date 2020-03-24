#ifndef __I___CLOTH_H_
#define __I___CLOTH_H_

#include <vector>
using namespace std;
#include <math.h>
#include <GL/glut.h>
#include "wektory.h"
#include "punkty.h"


class Cloth
{
public:
	Cloth(int _nx, int _ny, float _ks, float _kd, int _lenght_constrain, 
		Wektor r0, Wektor r1, float mass) :
		nx(_nx), ny(_ny), ks(_ks), kd(_kd), lenght_constrain(_lenght_constrain)
	{
		Points = new Punkt**[nx];
		for(i=0; i < nx; i++)
			Points[i] = new Punkt*[ny+1];
			
    	PointsCPY = new Punkt*[nx];
		for(i=0; i < nx; i++)
			PointsCPY[i] = new Punkt[ny+1];

		CreatePoints(r0,r1,mass);
	
	   // fix part of cloth
       for(j=0;j<ny;j+=1)
	   {
	    	Points[0][j]->flaga |=P_ZAW;
	   }
        makeCopyPoints();
        
    	MakeStructuralSprings();
		MakeFlexSprings();
	
		STARTX=0;
   		ROTATEX=0;	
	}

	~Cloth()
	{
		for(i=0; i<nx; i++)
			delete [] Points[i];
     		delete [] Points;
	}

	void BindTextureOnIt(unsigned int _tekstura)
	{cloth_txt = _tekstura;}

	void CreatePoints(Wektor r0,Wektor r1, float mass);
	void makeCopyPoints()
	{
            for(i=0;i<nx;i++)
        	for(j=0;j<ny;j++)    
        	{
                PointsCPY[i][j] = (*Points[i][j]);    
            }
    }
    
    void releasePoints()
    {
             for(i=0;i<nx;i++)
        	for(j=0;j<ny;j++)    
        	{
                PointsCPY[i][j].flaga &= (~P_ZAW);
                Points[i][j]->flaga &= (~P_ZAW);
            }
    }

    void updatePointsSTARTX(void)
    {
            for(i=0;i<nx;i++)
        	for(j=0;j<ny;j++) 
            if(Points[i][j]->flaga & P_ZAW && i<2)   
        	{
               (*Points[i][j]) = PointsCPY[i][j];
              // Points[i][j]->r.x += STARTX;
              
                // rotate
                
                float x = Points[i][j]->r.x;
                float y = Points[i][j]->r.y;
                float z = Points[i][j]->r.z;
                
                float xp = x*cos(ROTATEX) - y*sin(ROTATEX);
                float yp = x*sin(ROTATEX) + y*cos(ROTATEX);
                float zp = z;
                
                
                xp += STARTX;
                Points[i][j]->r = Wektor(xp,yp,zp);
              
            }
    }
	
	void MakeStructuralSprings(void);
	void MakeFlexSprings(void);
	void CalculateVertexNormals(void);


	void DisplayGL(int mode);

public:
	int nx;
	int ny;
	float ks;
	float kd;

	unsigned int cloth_txt;				// nr tekstury opengl (0 - brak)

	int i;							// licznik petli
	int j;
	int lenght_constrain;			// procentowo dopuszczalne rozciagniecie 

	Punkt ***Points;
	Punkt **PointsCPY;
/*	Sprezyna ***Springs1;			// strukturalne x
	Sprezyna ***Springs2;			// strukturalne y
	Sprezyna ***Springs3;			// (na krzyz)
	Sprezyna ***Springs4;			// (na krzyz)
	Sprezyna ***Springs5;			// strukturalne 2 * x		- do nich nie robimy korekty
	Sprezyna ***Springs6;			// strukturalne 2 * y*/
						// mozna jeszcze dodac inne
	vector<Sprezyna> springs;
	
	float STARTX;
	float ROTATEX;

};









#endif
