// LBM multiphase Shan-Chen model
// code: Maciej Matyka (http://www.matyka.pl/, maciej.matyka@gmail.com)
// used in James Hurbult's art installation (details in talk)
// 1-fluid,0-boundary
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
#include <GL/glew.h>
#include <GL/glut.h>
#include "importppm.h"

const float SCALE = 1.0;
const int SCRWIDTH = PICTURE_WIDTH*SCALE;
const int SCRHEIGHT = PICTURE_HEIGHT*SCALE;
int picturecol[PICTURE_WIDTH][PICTURE_HEIGHT][3] = {0};			// color map for particles
/*--------------------- Mouse ---------------------------------------------------------------------------*/
int mousedown = 0;
float xMouse, yMouse;
/*--------------------- On offs -------------------------------------------------------------------------*/
int moveonoff=0;
int calconoff=1;
/*--------------------- LBM -----------------------------------------------------------------------------*/
float fx=1, fx2=1;
float fy=0, fy2=1;
float angle=0;				// for rotations of the body force vec
float forcex = -0.020;//0.01;		// body force magnitude
float forcey = 0.015;//0.01;		// body force magnitude
/*--------------------- LBM State vector ----------------------------------------------------------------*/
#define NUM_VECTORS 9		// lbm basis vectors (d2q9 model)
GLuint col_SSB;
GLuint cF_SSB;
GLuint cU_SSB;
GLuint cV_SSB;
GLuint c0_SSB;
GLuint c1_SSB;
GLuint cP_SSB;	//psi
GLuint cEXTFX_SSB;	//psi
GLuint cEXTFY_SSB;	//psi
GLuint cR_SSB;	//rho
/*--------------------- Particles -----------------------------------------------------------------------*/
float dt=0.05;
const int NUMP = NX*NY;//PICTURE_WIDTH*PICTURE_HEIGHT;
GLuint particles_SSB;
struct p
{	float x,y; };
struct col
{	float r,g,b,a; };
p particles[NUMP];
/*--------------------- Shader Programs ------------------------------------------------------------------*/
GLuint lbmCS_Program;
GLuint moveparticlesCS_Program;

/*--------------------- Generate buffers------------------------------------------------------------------*/
void GenerateSSB(GLuint &bufid, int width, int height, float a)
{
	glGenBuffers(1,&bufid);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,bufid);
	glBufferData(GL_SHADER_STORAGE_BUFFER,width*height*sizeof(float),NULL,GL_STATIC_DRAW);
	float *temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, width*height*sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int y=0; y<height; y++)
	for(int x=0; x<width; x++)
		temp[x + y*width] = a;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
/*--------------------- Reset positions in particle buffers -----------------------------------------------*/
void resetparticles(void)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,particles_SSB);
	p *parGPU = (p *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUMP*sizeof(p), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i=0;
	
	for(int y=0; y<NY; y++)
	{
		for(int x=0; x<NX; x++)	
		{
			parGPU[i].x = x/float(NX);
			parGPU[i].y = y/float(NY);
			i++;
		}
		if(i>=NUMP) break;
	}
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
/*--------------------- Update obstacle flags -------------------------------------------------------------*/
void updateF()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,cF_SSB);
	int *F_temp = (int *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i = 0, xx,yy,x0,y0;
	
	for(int x=0; x<NX; x++)
	for(int y=0; y<NY; y++)
		F_temp[x+y*NX] = 1;


	for(int i=0; i<40; i++)
	{
		x0 = NX*(rand()/float(RAND_MAX));
		y0 = NY*(rand()/float(RAND_MAX));
		for(int x=0; x<NX; x++)
		for(int y=0; y<NY; y++)
		{
			//xx = x;//-(x0)*NX/2.0;
			//yy = y;//-(y0)*NY/2.0;
			int idx = x+y*NX;
			//if(idx > NX*NY)	break;
			if( sqrt(float((x-x0)*(x-x0)+(y-y0)*(y-y0))) < NX/62 )
				F_temp[idx] = 0;
			//else
			//	F_temp[idx] = 1;
		}	
	}

	//for(int x=0; x<NX; x++)
	//	F_temp[x+0*NX] = F_temp[x+(NY-1)*NX] = 0;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
void updateFpicturecol(void)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,cF_SSB);
	int *F_temp = (int *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i = 0, x0,y0;

	for(int x=0; x<NX; x++)
	for(int y=0; y<NY; y++)
	{
		if(picturecol[x*DX][y*DY][0] != 255)
			F_temp[x+y*NX] = 0;
		else
			F_temp[x+y*NX] = 1;
	}

	for(int x=0; x<NX; x++)
		F_temp[x+0*NX] = F_temp[x+(NY-1)*NX] = 1;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
void clearF(void)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,cF_SSB);
	int *F_temp = (int *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i = 0,x0,y0;

	for(int x=0; x<NX; x++)
	for(int y=0; y<NY; y++)
	{
		F_temp[x+y*NX] = 1;
	}

	for(int x=0; x<NX; x++)
		F_temp[x+0*NX] = F_temp[x+(NY-1)*NX] = 0;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}

void init_shaders(void)
{
	char log[12048];	
	int len = 0;
	len=0;
	GLchar *lbmCS_Source;
 	GLuint lbmCS_Shader;
	lbmCS_Source = filetobuf("sources/shaders/lbm.cs");
	lbmCS_Shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(lbmCS_Shader, 1, (const GLchar**)&lbmCS_Source, 0);
	glCompileShader(lbmCS_Shader);
	glGetShaderInfoLog(lbmCS_Shader, 12047, &len, log);
	//log[len] = '\0';	cout << "!! -----> " << log << endl;
	lbmCS_Program = glCreateProgram();
	glAttachShader(lbmCS_Program, lbmCS_Shader);
	glLinkProgram(lbmCS_Program);
	glUseProgram(lbmCS_Program);
	glUniform1i(0, NX);
	glUniform1i(1, NY);	
	glUseProgram(0);

	/*---------------------- Initialise particles shader and buffers on GPU ---------------------------------*/
	GLchar *moveparticlesCS_Source;
 	GLuint moveparticlesCS_Shader;
	moveparticlesCS_Source = filetobuf("sources/shaders/particles.cs");
	moveparticlesCS_Shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(moveparticlesCS_Shader, 1, (const GLchar**)&moveparticlesCS_Source, 0);
	glCompileShader(moveparticlesCS_Shader);
	glGetShaderInfoLog(moveparticlesCS_Shader, 1023, &len, log);
	//log[len] = '\0';	cout << log << endl;
	moveparticlesCS_Program = glCreateProgram();
	glAttachShader(moveparticlesCS_Program, moveparticlesCS_Shader);
	glLinkProgram(moveparticlesCS_Program);
	glUseProgram(moveparticlesCS_Program);
	glUniform1i(0, NX);
	glUniform1i(1, NY);	
	glUseProgram(0);
}

void updateColors(void)
{
	int i;
	glGenBuffers(1, &col_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, col_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMP*sizeof(struct col), NULL, GL_STATIC_DRAW);
	struct col *colors = (struct col *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUMP*sizeof(struct col), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	i = 0;
	for (int y = 0; y<NY; y++)
	{
		for (int x = 0; x<NX; x++)
		{
			int xi = int(x*DX);
			int yi = int(y*DY);
			//float r = picturecol[xi][yi][0] / float(255);
			//float g = picturecol[xi][yi][1] / float(255);
			//float b = picturecol[xi][yi][2] / float(255);

			float a = 0.0;// 0.8; //(0.3 + 0.1*(rand() / float(RAND_MAX)));
			float r = a;
			float g = a;
			float b = a;
			colors[i].r = r;
			colors[i].g = g;
			colors[i].b = b;
			colors[i].a = 1.0;
			i++;
		}
		if (i >= NUMP) break;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void init_buffers(void)
{
	int i = 0;
	glGenBuffers(1,&particles_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,particles_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NUMP*sizeof(p),NULL,GL_STATIC_DRAW);
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
	resetparticles();
	
	/*---------------------- Initialise LBM vector state as SSB on GPU --------------------------------------*/
	
	glGenBuffers(1,&cF_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,cF_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NX*NY*sizeof(int),NULL,GL_STATIC_DRAW);
	
	//importppmcol(string("data/tunele2.ppm"), picturecol, PICTURE_WIDTH, PICTURE_HEIGHT);
	importppmcol(string("data/tunele2.ppm"), picturecol, PICTURE_WIDTH, PICTURE_HEIGHT);
	updateFpicturecol();
	//updateF();
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	

	GenerateSSB(cU_SSB, NX,NY, 0.0);
	GenerateSSB(cV_SSB, NX,NY, 0.0);
	GenerateSSB(cP_SSB, NX,NY, 0.0);
	GenerateSSB(cEXTFX_SSB, NX,NY, 0.0);
	GenerateSSB(cEXTFY_SSB, NX,NY, 0.0);
	GenerateSSB(cR_SSB, NX,NY, 0.0);
	

	float w[]={(4.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/36.0),(1.0/36.0),(1.0/36.0),(1.0/36.0)};
	const float c1=1,c2=3,c3=9.0/2.0,c4=-3.0/2.0;
	const int ex[9] = {0,  1,0,-1, 0,  1,-1,-1, 1};
	const int ey[9] = {0,  0,1, 0,-1,  1, 1,-1,-1};
	const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
	const int ep1[9] = {0, 2,3, 4, 1,  6, 7, 8, 5};     // normal vectors (left)
	const int ep2[9] = {0, 4,1, 2, 3,  8, 5, 6, 7};     // --             (right)

	float u = 0;
	float v = 0;
	float rho = (150.-rand()/(float)RAND_MAX);

	glGenBuffers(1,&c0_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,c0_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NX*NY*sizeof(float)*NUM_VECTORS,NULL,GL_STATIC_DRAW);
	float *temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(float)*NUM_VECTORS, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int k=0; k<NUM_VECTORS; k++)
	for(int y=0; y<NY; y++)
	for(int x=0; x<NX; x++)
		temp[k + x*NUM_VECTORS + y*NX*NUM_VECTORS] = 
		w[k] * rho * (1.0f - (3.0f/2.0f) * (u*u + v*v) + 3.0f * (ex[k] * u + ey[k]*v) + (9.0f/2.0f) * (ex[k] * u + ey[k]*v) * (ex[k] * u + ey[k]*v));
		
		//w[k]*(1.0-rand()/(float)RAND_MAX);

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
	glGenBuffers(1,&c1_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,c1_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NX*NY*sizeof(float)*NUM_VECTORS,NULL,GL_STATIC_DRAW);
	temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(float)*NUM_VECTORS, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int k=0; k<NUM_VECTORS; k++)
	for(int y=0; y<NY; y++)
	for(int x=0; x<NX; x++)
		temp[k + x*NUM_VECTORS + y*NX*NUM_VECTORS] = 
		//w[k]*(1.0-rand()/(float)RAND_MAX);
		w[k] * rho * (1.0f - (3.0f/2.0f) * (u*u + v*v) + 3.0f * (ex[k] * u + ey[k]*v) + (9.0f/2.0f) * (ex[k] * u + ey[k]*v) * (ex[k] * u + ey[k]*v));
	
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
	
	//importppmcol(string("data/twarz720_2.ppm"), picturecol, PICTURE_WIDTH, PICTURE_HEIGHT);
//	importppmcol(string("data/empty.ppm"), picturecol, PICTURE_WIDTH, PICTURE_HEIGHT);
	//importppmcol(string("data/nvision1280x720.ppm"), picturecol, PICTURE_WIDTH, PICTURE_HEIGHT);
	

	/*for(int x=0; x<PICTURE_WIDTH; x++)
	{
		for(int y=0; y<PICTURE_HEIGHT; y++)
		{
			float c = 55+200*(rand()/float(RAND_MAX));
			picturecol[x][y][0] = c;
			picturecol[x][y][1] = c;
			picturecol[x][y][2] = c;
		}
	}*/

	updateColors();

	/*---------------------- Some bindings ------------------------------------------------------------------*/

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  2,  cF_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  3,  cU_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  4,  cV_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  5,  cP_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  6,  cEXTFX_SSB );		
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  7,  cEXTFY_SSB );		
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  8,  cR_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  9,  col_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  69,  particles_SSB );						// 69
	
}

void init(void)
{
	int i;
	fx2=fx; fy2=fy;		// init force
	/*--------------------- Initialize GLEW -----------------------------------------------------------------*/
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{	
		cout << "Error: " << glewGetErrorString(err) << endl;		
		exit(0);	
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	/*-------------------- Compute shaders programs etc. ----------------------------------------------------*/

	init_shaders();
	init_buffers();
	
}

void redisplay(int w, int h) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}
int c = 0;
float time=0;

void render(void) 
{

	//forcey=0.02*(1+sin(0.5*time));

#define NUMR 45
	if(calconoff)
	{
		glUseProgram(lbmCS_Program);
		glUniform1f(2, forcex);				// set body force in the shader
		glUniform1f(3, forcey);	
		for(int i=0; i<NUMR; i++)
		{
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  c,  c0_SSB );	
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  1-c,  c1_SSB );	
			c=1-c;
			glDispatchCompute( NX / 40 , NY / 20, 1);
			//glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT);
		}
	}

	if(0&&moveonoff)
	{
		glUseProgram(moveparticlesCS_Program);
		glDispatchCompute( NUMP / 1000 , 1, 1);
		//glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
		glUniform1f(10, dt);
	}
			
	// render 
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable( GL_POINT_SMOOTH );
	glPointSize(DX*2);
	//glColor4f(0.2,0.1,0,0.4);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

	// particles
	/*glBindBuffer( GL_ARRAY_BUFFER, particles_SSB );
	glVertexPointer( 2, GL_FLOAT, 0, (void *)0 );
	glEnableClientState( GL_VERTEX_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, col_SSB );
	glColorPointer( 4, GL_FLOAT, 0, (void *)0 );
	glEnableClientState( GL_COLOR_ARRAY );
	glDrawArrays( GL_POINTS, 0, NUMP);
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );*/


	// particles
	glBindBuffer( GL_ARRAY_BUFFER, particles_SSB );
	glVertexPointer( 2, GL_FLOAT, 0, (void *)0 );
	glEnableClientState( GL_VERTEX_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, col_SSB );
	glColorPointer( 4, GL_FLOAT, 0, (void *)0 );
	glEnableClientState( GL_COLOR_ARRAY );
	glDrawArrays( GL_POINTS, 0, NUMP);
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// gravity vector
	/*glColor3f(1,0,0);			
	glBegin(GL_LINES);
	glVertex2f(0.15,0.15);
	glVertex2f(.1*fx2+0.15,.1*fy2+0.15);
	glEnd();*/

	static int step=0;
	step++;

	glutSwapBuffers();
	glutPostRedisplay();
}
void timerFunction(int data)
{
	//if(mousedown)
		//updateF();

	time = time + dt;
	glutTimerFunc(10, timerFunction, -1);
}
void key(unsigned char key, int a, int b)
{
	if(int(key)==27) exit(0);
	if(key=='m')	moveonoff = 1-moveonoff;
	if(key=='f')	clearF();
	if(key=='g')	forcey=-forcey;
	if (key == 'h')	{
		updateFpicturecol();  updateColors();
	}
	if(key=='c')	calconoff = 1-calconoff;
	if(key=='d')	dt = -dt;
	if(key==' ')	{ resetparticles(); }
	if(key=='+') 	{forcex*=(-1);}
	if(key=='-') 	{forcey*=0.98;}
	if(key=='r')
	{
		angle += 2*3.14/180.0;
		fx2 = fx * cos(angle) - fy * sin(angle);
		fy2 = fx * sin(angle) + fy * cos(angle);
	}
}
/*--------------------- Mouse ---------------------------------------------------------------------------*/
void Mouse (int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mousedown = 1;
			xMouse =  2.0 * ((float)x/(float)SCRWIDTH - 0.5);
			yMouse = - 2.0 * ((float)y/(float)SCRHEIGHT - 0.5);
		}
		else if (state == GLUT_UP)
	       	mousedown = 0;
     }
}
void Motion (int x, int y)
{
	if (mousedown)
	{
		xMouse = 2.0 * ((float)x/(float)SCRWIDTH - 0.5);
		yMouse = -2.0 * ((float)y/(float)SCRHEIGHT - 0.5);
		glutPostRedisplay();
 	}
}
/*--------------------- Boring File I/O ---------------------------------------------------------------------------*/
char* filetobuf(char *file) // (copied from Wikipedia)
{
	FILE *fptr;    
	long length;    
	char *buf;    
	fptr = fopen(file, "r");     
	if (!fptr)         
		return NULL;    
	fseek(fptr, 0, SEEK_END);     
	length = ftell(fptr);     
	buf = (char *)malloc(length+1);
	for(int i=0;i<length+1;i++) buf[i]=0;
	fseek(fptr, 0, SEEK_SET);     
	fread(buf, length, 1, fptr);     
	fclose(fptr);     
	buf[length] = 0;     
	return buf; 
} 
/*--------------------- Main loop ---------------------------------------------------------------------------*/
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(0,100);
	glutInitWindowSize(SCRWIDTH,SCRHEIGHT);
	glutCreateWindow("Bomb");
	init();
	glutDisplayFunc(render);
	glutKeyboardFunc(key);
	glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
	glutReshapeFunc(redisplay);
	glutTimerFunc(15, timerFunction, -1);
	glutMainLoop();
}
