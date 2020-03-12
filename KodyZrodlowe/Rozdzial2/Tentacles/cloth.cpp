

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "wektory.h"
#include "punkty.h"
#include "cloth.h"

using namespace std;

float SPHERERAD = 0.2;

// NormalTriangle - zwraca normalna do trojkata
Wektor NormalTriangle(Wektor p1, Wektor p2, Wektor p3)
{
	Wektor n=(p1-p2)%(p1-p3);
	n.normuj(); 
	return n;
}

// funkcja wylicza normalne dla wszystkich punktow w scenie jako srednia 
// normalnych przylegajacych scian

// normalne przechowuje w polu punkt->dr
void Cloth::CalculateVertexNormals(void)
{
	Wektor n;
	// na razie bez brzegow
	for(i=1;i<nx;i++)
	for(j=0;j<ny;j++)
	{
	//   n = PointsCPY[i-1][j].r - PointsCPY[i][j].r;
    	n = Points[i-1][j]->r - Points[i][j]->r;

        float t = n.x;
		n.x=-n.y;
		n.y=n.z;
		n.z = t;
			n.normuj();
        /*NormalTriangle(Points[i][j]->r,Points[i][j-1]->r,Points[i+1][j-1]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i+1][j-1]->r,Points[i+1][j]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i+1][j]->r,Points[i+1][j+1]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i+1][j+1]->r,Points[i][j+1]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i][j+1]->r,Points[i-1][j+1]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i-1][j+1]->r,Points[i-1][j]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i-1][j]->r,Points[i-1][j-1]->r);
		n = n + NormalTriangle(Points[i][j]->r,Points[i-1][j-1]->r,Points[i][j-1]->r);

//if(n.x==n.y==0)
			// exit(0);
//	n =     NormalTriangle(Points[i][j]->r,Points[i][j-1]->r,Points[i+1][j-1]->r);

//		n = n * 0.1;
		n.normuj();
		*/
		
		
		
		Points[i][j]->n = n;
	}

	// brzegi bez rogow
	for(i=1;i<nx-1;i++)
		Points[i][0]->n = Points[i][1]->n;

	for(i=1;i<nx-1;i++)
		Points[i][ny-1]->n = Points[i][ny-2]->n;

	for(j=1;j<ny-1;j++)
		Points[0][j]->n = Points[1][j]->n;

	for(j=1;j<ny-1;j++)
		Points[nx-1][j]->n =  Points[nx-2][j]->n;


	// na rogach

	Points[0][0]->n = Points[1][0]->n + Points[0][1]->n;
	Points[0][0]->n.normuj();

	Points[0][ny-1]->n = Points[0][ny-2]->n + Points[1][ny-1]->n;
	Points[0][ny-1]->n.normuj();

	Points[nx-1][ny-1]->n = Points[nx-1][ny-2]->n + Points[nx-2][ny-1]->n;
	Points[nx-1][ny-1]->n.normuj();

	Points[nx-1][0]->n = Points[nx-1][1]->n + Points[nx-2][0]->n;
	Points[nx-1][0]->n.normuj();

}



void Cloth::MakeFlexSprings(void)
{

}


void Cloth::MakeStructuralSprings(void)
{    
    for(i=0;i<nx-1;i++)
	for(j=0;j<ny;j++)
	   springs.push_back(Sprezyna(Points[i][j],Points[i+1][j],ks,kd,0,S_RYSL,lenght_constrain,i/(float)nx));

    for(i=0;i<nx-2;i++)
	for(j=0;j<ny;j++)
	   springs.push_back(Sprezyna(Points[i][j],Points[i+2][j],ks,kd,0,S_NRYS,lenght_constrain,i/(float)nx));


/*    for(i=0;i<nx-2;i++)
	for(j=0;j<ny-1;j++)
	   springs.push_back(Sprezyna(Points[i][j],Points[i][j+1],ks,kd,0,S_RYSL,lenght_constrain));
*/




    /*for(i=0;i<nx;i++)
	for(j=0;j<ny-1;j++)
	   springs.push_back(Sprezyna(Points[i][j],Points[i][j+1],ks,kd,0,S_RYSL,lenght_constrain));*/

	   
	   
  /*  for(i=0;i<nx;i++)
	for(j=1;j<ny;j++)
	   springs.push_back(Sprezyna(Points[i][j],Points[i][j-1],ks,kd,0,S_RYSL,lenght_constrain));*/

    /*for(i=0;i<nx-1;i++)
	for(j=1;j<ny;j++)
       springs.push_back(Sprezyna(Points[i][j],Points[i+1][j-1],ks,kd,0,S_RYSL,lenght_constrain)); */
}


/*

	for(i=0;i<NX-1;i++)
	for(j=0;j<NY;j++)
		StructuralSpringsX[i][j] = AlokujSprezyne(s_kd,0,s_ks,Surface[i][j],Surface[i+1][j],S_OK|S_RYSL);	
*/

void Cloth::CreatePoints(Wektor r0, Wektor r1, float mass)
{
// nx,ny
// x0,y0,z0,x1,y1,z1

	Wektor dr,r;

	dr = (r1-r0);
	dr.x /= (float)nx;
	dr.y /= (float)ny;
	dr.z /= (float)((nx+ny)/2);

	r=r0;
	
	float dx=0;
	
//    dr = dr;// * (1.0/nx);

    float d1=SPHERERAD;

	for(j=0;j<ny;j++)
	{
        float delta = 0.027;//+0.05*(rand()/(float)RAND_MAX);
        
        r.x = 0;//r0.x;
        r.y = 0;//r0.y;
        r.z = 0;//r0.z;
      
        dr.x = (rand()/(float)RAND_MAX-0.5);
        dr.y = (rand()/(float)RAND_MAX-0.5);
        dr.z = (rand()/(float)RAND_MAX-0.5);
        dr.normuj();
        r.x+=dr.x*d1;
        r.y+=dr.y*d1;
        r.z+=dr.z*d1;
        
		for(i=0;i<nx;i++) // entire row
		{
			Points[i][j] = new Punkt(P_NORM, mass,r,Wektor(0.05*(rand()/(float)RAND_MAX-0.5),0.05*(rand()/(float)RAND_MAX-0.5),0.05*(rand()/(float)RAND_MAX-0.5)));
			r.x+=dr.x * delta;
			r.y+=dr.y * delta;
			r.z+=dr.z * delta;
		}

		/*if(!(j%2))
           dx=dr.x/2.0;
        else
            dx = 0;      
		r.x=r0.x+dx;
		r.y=r.y+dr.y;
		r.z=r.z+dr.z;
		*/
	}



/* dump 

	FILE *fp;
	char work[100];
	fp=fopen("file.txt","w");

	j=10;
	for(i=0;i<nx;i++)
	{
		sprintf(work,"%f\n",Points[i][j]->r.x);
		fputs(work,fp);
	}
		fclose(fp);
*/
        makeCopyPoints();
}


Wektor LightWektor = Wektor(0.2,-1.9,-0.6);
	
// liczy oswietlenie deLamberta (ciekawe czy tak sie pisze to nazwisko?)
float deLambert(Wektor n)
{

		return ((n * LightWektor)/2.0 + 0.63);	// + oswietlenie tla
}


GLfloat ambient_light[ 4 ] =
{
    0.6, 0.6, 0.6, 1.0
};

void Cloth::DisplayGL(int mode)
{
// mode - flaga, ale to póŸniej, na razie lecimy jak leci 
		Wektor n;
		double nasycenie;

	switch(mode)
	{
	case 0:
		glDisable(GL_LIGHTING);
		glColor3f(1,1,1);
		glPointSize(2);

	// rysuj punkty
		glBegin(GL_POINTS);
		for(j=0;j<ny;j++)
		for(i=0;i<nx;i++)
		{
				glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);
		}
		glEnd();
	
	
		break;	

	case 1:
			// rysuj sprezyny
		glLineWidth(1);
		glDisable(GL_LIGHTING);
		glEnable (GL_BLEND); 
        glBlendFunc (GL_ONE, GL_ONE); 
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	//glEnable(GL_LIGHTING);
		glDisable (GL_BLEND);   
/*		for(i=0;i<springs.size();i++)
        if(springs[i].flag & S_RYSL)	
		{
            float c = springs[i].p;
            //(springs[i].punkt1->r-Wektor(STARTX,0,0)).dlugosc();
//            c/=0.8;
            c=c;
            glColor4f(c*1.25,c*1.35,c*1.05,0.8);
            glLineWidth((1-c)*13);
    		glBegin(GL_LINE_STRIP);
    		    c = ((springs[i].punkt1->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*3;
                glColor4f(c*1.25,c*1.35,c*1.05,0.8);
            
    			glVertex3f(springs[i].punkt1->r.x,springs[i].punkt1->r.y,springs[i].punkt1->r.z);

    		    c = ((springs[i].punkt2->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*3;
                glColor4f(c*1.25,c*1.35,c*1.05,0.8);
	       		glVertex3f(springs[i].punkt2->r.x,springs[i].punkt2->r.y,springs[i].punkt2->r.z);
    		glEnd();
		}*/
		
		
/*      	for(j=0;j<ny;j++)    
        for(i=0;i<nx;i++)
       	{
//               PointsCPY[i][j] = (*Points[i][j]);    
        }
         glLineWidth(2.0);
         */

      	for(j=0;j<ny;j++)    
        for(i=0;i<nx-1;i++)
        {
            float c = PointsCPY[i][j].r.dlugosc();
    
    	    c = ((Points[i][j]->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*3;
//    	    c = ((Points[i][j]->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*200*Points[i][j]->v.dlugosc();
    	    
            glLineWidth(5-(c)*6);          
//            glLineWidth(4-(c)*4);          
            
    		glBegin(GL_LINE_STRIP);
                glColor4f(c*1.25+0.2*j/float(ny),c*1.35+0.3*j/float(ny),c*0.85+0.4*j/float(ny),0.5+0.2*j/float(ny));          
    			glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);

    		    c = ((Points[i+1][j]->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*3;
//    	    c = ((Points[i][j]->r-Wektor(STARTX,0,0)).dlugosc()-0.2)*200*Points[i][j]->v.dlugosc();
                glColor4f(c*1.25+0.2*j/float(ny),c*1.35+0.3*j/float(ny),c*0.85+0.4*j/float(ny),0.5+0.2*j/float(ny));
	       		glVertex3f(Points[i+1][j]->r.x,Points[i+1][j]->r.y,Points[i+1][j]->r.z);
    		glEnd();
		}
		

		break;

	case 2:
		// oswietlenie d'Lamberta, korzysta z CalculateVertexNormal

//		CalculateVertexNormals();




		glDisable(GL_LIGHTING);

		glBegin(GL_TRIANGLES);
			for(j=0;j<ny-1;j++)				// wiatr na trojkaty 1
			for(i=0;i<nx-1;i++)
			{

				nasycenie =deLambert(Points[i][j]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);

				nasycenie = deLambert(Points[i+1][j]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i+1][j]->r.x,Points[i+1][j]->r.y,Points[i+1][j]->r.z);

				nasycenie = deLambert(Points[i][j+1]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i][j+1]->r.x,Points[i][j+1]->r.y,Points[i][j+1]->r.z);
			}


			for(j=0;j<ny;j++)				// wiatr na trojkaty 2
			for(i=1;i<nx;i++)
			{
				nasycenie = deLambert(Points[i][j]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);

				nasycenie = deLambert(Points[i][j+1]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i][j+1]->r.x,Points[i][j+1]->r.y,Points[i][j+1]->r.z);

				nasycenie = deLambert(Points[i-1][j+1]->n);
				glColor3f(nasycenie,nasycenie,nasycenie);
				glVertex3f(Points[i-1][j+1]->r.x,Points[i-1][j+1]->r.y,Points[i-1][j+1]->r.z);
			}
		glEnd();

		break;



	case 3:		// teksturowanie



		CalculateVertexNormals();

		glShadeModel(GL_SMOOTH);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_CULL_FACE);
		glColor4f(0.5,0.5,0.5,0.1);
		glBindTexture(GL_TEXTURE_2D,cloth_txt);	

   
			for(j=0;j<ny-1;j++)				// wiatr na trojkaty 1
			for(i=0;i<nx-1;i++)
			{
		glBegin(GL_QUADS);
				nasycenie = deLambert(Points[i][j]->n);
				glNormal3f(Points[i][j]->n.x, Points[i][j]->n.y, Points[i][j]->n.z);
				glColor4f(nasycenie,nasycenie,nasycenie,1);
				glTexCoord2f((float)i/(float)(nx-1),1-(float)j/(float)(ny-1));
				glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);

				nasycenie = deLambert(Points[i+1][j]->n);
				glColor4f(nasycenie,nasycenie,nasycenie,1);
				glNormal3f(Points[i+1][j]->n.x, Points[i+1][j]->n.y, Points[i+1][j]->n.z);
				glTexCoord2f((float)(i+1)/(float)(nx-1),1-(float)j/(float)(ny-1));
				glVertex3f(Points[i+1][j]->r.x,Points[i+1][j]->r.y,Points[i+1][j]->r.z);

				nasycenie = deLambert(Points[i+1][j+1]->n);
				glColor4f(nasycenie,nasycenie,nasycenie,1);
				glNormal3f(Points[i+1][j+1]->n.x, Points[i+1][j+1]->n.y, Points[i+1][j+1]->n.z);
				glTexCoord2f((float)(i+1)/(float)(nx-1),1-(float)(j+1)/(float)(ny-1));
				glVertex3f(Points[i+1][j+1]->r.x,Points[i+1][j+1]->r.y,Points[i+1][j+1]->r.z);

				nasycenie = deLambert(Points[i][j+1]->n);
				glColor4f(nasycenie,nasycenie,nasycenie,1);
				glNormal3f(Points[i][j+1]->n.x, Points[i][j+1]->n.y, Points[i][j+1]->n.z);
				glTexCoord2f((float)(i)/(float)(nx-1),1-(float)(j+1)/(float)(ny-1));
				glVertex3f(Points[i][j+1]->r.x,Points[i][j+1]->r.y,Points[i][j+1]->r.z);
		glEnd();
		}
    } 

	
		glDisable(GL_BLEND);
		glDepthFunc(GL_LESS);
		glDisable(GL_TEXTURE_2D);
		
		
		// rysuj normalne
	/*	glLineWidth(1);
		glColor4f(1,1,1,1);
		// normals
		float s = -0.03;
			for(j=0;j<ny-1;j++)				// wiatr na trojkaty 1
			for(i=0;i<nx-1;i++)
			{
			   glBegin(GL_LINES);
					glVertex3f(Points[i][j]->r.x,Points[i][j]->r.y,Points[i][j]->r.z);
					glVertex3f(Points[i][j]->r.x+Points[i][j]->n.x*s,Points[i][j]->r.y+Points[i][j]->n.y*s,Points[i][j]->r.z+Points[i][j]->n.z*s);
				glEnd();
			}*/
	
}


