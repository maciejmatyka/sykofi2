
// Ising model
// Maciej Matyka, 2014
// g++ ising.cpp -lGL -lglut -lGLU -lm

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define NX 640
#define NY 480
#define WIDTH NX
#define HEIGHT NY
int S[NX][NY];
float J = 1.0;
float beta = 1.0;	// 1/T
void changeSize(int w, int h) 
{
	float ratio = 1.0 * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}
void renderScene(void) 
{
   int i,j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1,1,1);
	glPointSize(5.0);
	float dx = 1.0/NX;
	float dy = 1.0/NY;
	for(i=0; i<NX; i++)
	for(j=0; j<NY; j++)
	{
		  float x,y;
		  x = i/(float)NX;
		  y = j/(float)NY;
		  
		  
		  if(S[i][j]==-1) 
			glColor4f(1,1,1,1);
		  else
			glColor4f(0,0,0,1);
		  
		  glBegin(GL_QUADS);								
			  glVertex2f(x, y);
			  glVertex2f(x+dx,y);
			  glVertex2f(x+dx,y+dy);
			  glVertex2f(x,y+dy);
		  glEnd();
	   }
   
	glutSwapBuffers();
}

//http://www.physics.ohio-state.edu/~braaten/statphys/Ising_MatLab.pdf
int energy(int i, int j)
{
  int e=0;
    
  int ip = (i+1) % NX;
  int jp = (j+1) % NY;
  int im = (i-1+NX) % NX;
  int jm = (j-1+NY) % NY;
  
  e = e - J*S[i][j] * ( S[im][j] + S[ip][j] + S[i][jm] + S[i][jp] ); 
  
}

void step(void)
{
   int i,j;
   float e0,e1,de,r;
   
   i = NX*(rand()/(float)RAND_MAX);
   j = NY*(rand()/(float)RAND_MAX);
 
   
   //http://en.wikipedia.org/wiki/Ising_model
   e0 = energy(i,j);
   S[i][j] = -S[i][j];
   e1 = energy(i,j);
   
   de = e1-e0;

   if(de > 0)
   {
      r = rand()/(float)RAND_MAX;
      if( r > exp(-beta * de) )
	S[i][j] = -S[i][j];
	    //reject
   } 
}

void idleFunction(void)
{
   // step
   // 

  for(int k=0; k<NX*NY; k++)
    step();
  
 glutPostRedisplay();
}

void init(void)
{
   int i,j;
   for(i=0; i<NX; i++)
	 for(j=0; j<NY; j++)
	   {
		  float r = rand()/(float)RAND_MAX;
		  if(r>0.5)
			  S[i][j] = 1;
		  else
			S[i][j] = -1;
	   }  
}

void Keyboard(unsigned char key, int x, int y)
{
   
     switch (key)
	 {
		
	  case 27:             // ESCAPE key
			  exit (0);
			  break;
	  default:
		break;
	 }
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow("GLUT");
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunction);
	glutKeyboardFunc(Keyboard);   
    init();
   
	// enter GLUT event processing cycle
	// 
	// 
	// 
	glutMainLoop();
}
