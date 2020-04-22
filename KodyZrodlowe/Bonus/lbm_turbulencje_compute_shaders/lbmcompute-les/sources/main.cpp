//
//Smagorinski LES Lattice Boltzmann solver
//code: Maciej Matyka (http://www.matyka.pl/, maciej.matyka@gmail.com) - main, shaders, base lbm code
//code: Nicolas Delbosc (Leeds, mnnd@leeds.ac.uk) - Smagorinski LES code
//support: Dragos Chirila - Smagorinski LES
//03.2015 (Wroclaw)
//34
//note: possible speed improvement by rendering velocity directly to texture (try it).
//licence: don't seriously use, no responsibility from author side, you are welcome to learn from it & have fun
//compile windows:
//    MSVisualC++ project included
//compile linux:
//    g++ main.cpp -lglut -L /usr/lib64/ -lGLEW -lGLU LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64
//let us know where does this end if anywhere, contact: maciej.matyka@gmail.com
//
//
// 1-fluid,0-boundary

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
#include <GL/glew.h>
#include <GL/glut.h>

const int NX = 280;		// solver grid resolution
const int NY = 160;
char* filetobuf(char *file); // (copied from Wikipedia)

const float SCALE = 1;
const int SCRWIDTH = 1280*SCALE;		// screen size
const int SCRHEIGHT = 720*SCALE;		// screen size y

/*--------------------- Mouse ---------------------------------------------------------------------------*/
int mousedown = 0;
float xMouse, yMouse;
/*--------------------- On offs -------------------------------------------------------------------------*/
int moveonoff=1;
int calconoff=1;
int clearonoff = 1;
/*--------------------- LBM -----------------------------------------------------------------------------*/
float fx=1, fx2=1;
float fy=0, fy2=1;
float angle=0;				// for rotations of the body force vec
float force = -0.000007;// 5;		// body force magnitude
/*--------------------- LBM State vector ----------------------------------------------------------------*/
#define NUM_VECTORS 9		// lbm basis vectors (d2q9 model)
GLuint col_SSB;
GLuint cF_SSB;
GLuint cU_SSB;
//GLuint cU_SSB_COPY;
GLuint cV_SSB;
GLuint c0_SSB;
GLuint c1_SSB;
GLuint color_SSB;
GLuint color_SSB_COPY;
int F_cpu[NX*NY];
float *color_CPU = new float[NX*NY];

/*--------------------- Particles -----------------------------------------------------------------------*/
float dt = 0.1;// 05;
const int NUMP = 690000;
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
	glBufferData(GL_SHADER_STORAGE_BUFFER,width*height*sizeof(float),NULL,GL_DYNAMIC_DRAW);
	float *temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, width*height*sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int y=0; y<height; y++)
	for(int x=0; x<width; x++)
		temp[x + y*width] = a;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
void GenerateCOPY(GLuint &bufid, int width, int height, float a)
{
	glGenBuffers(1, &bufid);
	glBindBuffer(GL_COPY_READ_BUFFER, bufid);
	glBufferData(GL_COPY_READ_BUFFER, width*height*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glUnmapBuffer(GL_COPY_READ_BUFFER);
}
/*--------------------- Reset positions in particle buffers -----------------------------------------------*/
void resetparticles(void)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,particles_SSB);
	p *parGPU = (p *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUMP*sizeof(p), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i=0;
	for(i=0; i<NUMP; i++)
	{
		parGPU[i].x = (float)rand() / (float)RAND_MAX;
		parGPU[i].y = (float)rand() / (float)RAND_MAX;
	}
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
}
/*--------------------- Update obstacle flags -------------------------------------------------------------*/
void updateF(void)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,cF_SSB);
	int *F_temp = (int *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	
	int i = 0, xx,yy;

	
	
	for(int x=0; x<NX; x++)
	for (int y = 0; y<NY; y++)
	{
		xx = x - (xMouse)*NX / 2.0;
		yy = y - (yMouse)*NY / 2.0;
		int idx = x + y*NX;
		if (idx > NX*NY)	break;
		if (sqrt(float((xx - NX / 2)*(xx - NX / 2) + (yy - NY / 2)*(yy - NY / 2))) < NX / 14)
		{
			F_cpu[idx] = 0;
			F_temp[idx] = 0;
		}
		else
		{
			F_cpu[idx] = 1;
			F_temp[idx] = 1;

		}
	}	

	for (int x = 0; x<NX; x++)
		F_temp[x + 0 * NX] = F_temp[x + (NY - 1)*NX] = 0;


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
	log[len] = '\0';	cout << log << endl;
	moveparticlesCS_Program = glCreateProgram();
	glAttachShader(moveparticlesCS_Program, moveparticlesCS_Shader);
	glLinkProgram(moveparticlesCS_Program);
	glUseProgram(moveparticlesCS_Program);
	glUniform1i(0, NX);
	glUniform1i(1, NY);	
	glUseProgram(0);
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
	updateF();
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	


	GenerateSSB(color_SSB, NX, NY, 0.0);
	GenerateSSB(cU_SSB, NX,NY, 0.0);
	GenerateSSB(cV_SSB, NX,NY, 0.0);

	GenerateCOPY(color_SSB_COPY, NX, NY, 0.0);

	float w[]={(4.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/9.0),(1.0/36.0),(1.0/36.0),(1.0/36.0),(1.0/36.0)};
	
	glGenBuffers(1,&c0_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,c0_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NX*NY*sizeof(float)*NUM_VECTORS,NULL,GL_STATIC_DRAW);
	float *temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(float)*NUM_VECTORS, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int k=0; k<NUM_VECTORS; k++)
	for(int y=0; y<NY; y++)
	for(int x=0; x<NX; x++)
		temp[k + x*NUM_VECTORS + y*NX*NUM_VECTORS] = w[k];

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	
	glGenBuffers(1,&c1_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,c1_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NX*NY*sizeof(float)*NUM_VECTORS,NULL,GL_STATIC_DRAW);
	temp = (float *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(float)*NUM_VECTORS, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int k=0; k<NUM_VECTORS; k++)
	for(int y=0; y<NY; y++)
	for(int x=0; x<NX; x++)
		temp[k + x*NUM_VECTORS + y*NX*NUM_VECTORS] = w[k];
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	


	glGenBuffers(1,&col_SSB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,col_SSB);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NUMP*sizeof(struct col),NULL,GL_STATIC_DRAW);
	struct col *colors = (struct col *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUMP*sizeof(struct col), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	i = 0;
	for (int i = 0; i < NUMP; i++)
	{
		float r = rand() / (float)RAND_MAX;
		float g = r;// rand() / (float)RAND_MAX;
		float b = r;// rand() / (float)RAND_MAX;
		colors[i].r = r;// *(float)i / (float)NUMP;
		colors[i].g = g ;
		colors[i].b = b;
		colors[i].a = 0.2;
		i++;
	}

//		}
//		if(i>=NUMP) break;
//	}
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );	

	/*---------------------- Some bindings ------------------------------------------------------------------*/

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  2,  cF_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  3,  cU_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  4,  cV_SSB );			
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  5,  color_SSB);		// nx*ny
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
float time_=0;

void render(void) 
{


	// computation (!)

	#define NUMR 20 //40
	//unsigned long int t1 = GetTickCount();
	if(calconoff)
	for(int i=0; i<NUMR; i++)
	{
		glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  c,  c0_SSB );	
		glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  1-c,  c1_SSB );	
		c=1-c;
		glUseProgram(lbmCS_Program);
		glUniform1f(2, fx2*force);				// set body force in the shader
		glUniform1f(3, fy2*force);	
		glDispatchCompute( NX / 10 , NY / 10, 1);
		glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
	}
	glUseProgram(moveparticlesCS_Program);
	glDispatchCompute(NUMP / 1000, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUniform1f(12, dt);

	// render 
	glClearColor(1,1,1,1);
	
	if (clearonoff)
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable( GL_POINT_SMOOTH );
	
	glColor4f(0.8,0.1,0,0.1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);



	// Nicolas: rendering of velocity magnitude	from SSB buffer	
	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, cU_SSB);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, NX*NY*sizeof(float), cU_CPU);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, color_SSB);
	glBindBuffer(GL_COPY_READ_BUFFER, color_SSB_COPY);
	glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_COPY_READ_BUFFER, 0, 0, NX*NY*sizeof(float));
	glBindBuffer(GL_COPY_READ_BUFFER, color_SSB_COPY);
	glGetBufferSubData(GL_COPY_READ_BUFFER, 0, NX*NY*sizeof(float), color_CPU);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);

	for (int x = 0; x < NX; x++)
	{
		for (int y = 0; y < NY; y++)
		{
			int idx = x + y * NX;
			float c = color_CPU[idx];
			if (F_cpu[idx] == 0)	{ c = 0.24; }
			glColor4f(c, c, c, 1);
			float x1 = (float)x / (float)NX;
			float y1 = (float)y / (float)NY;
			float dx = 1.0 / (float)NX;
			float dy = 1.0 / (float)NY;
			glBegin(GL_QUADS); 
				glVertex2f(x1, y1);
				glVertex2f(x1 + dx, y1);
				glVertex2f(x1 + dx, y1 + dy);
				glVertex2f(x1, y1 + dy);
			glEnd();
			//}
		}
	}
	glEnd();



	// Nicolas: particles rendering
	glPointSize(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_SSB);
	glVertexPointer(2, GL_FLOAT, 0, (void *)0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, col_SSB);
	glColorPointer(4, GL_FLOAT, 0, (void *)0);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_POINTS, 0, NUMP);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glutSwapBuffers();
}

void idleFunction(void)
{
	
}

void timerFunction(int data)
{
	if(mousedown)
		updateF();

	time_ = time_ + dt;
	glutPostRedisplay();
	glutTimerFunc(10, timerFunction, -1);
}
void key(unsigned char key, int a, int b)
{
	if(int(key)==27) exit(0);
	if(key=='m')	moveonoff = 1-moveonoff;
	if(key=='c')	calconoff = 1-calconoff;
	if (key == 'v') clearonoff = 1-clearonoff;
	if(key=='d')	dt = -dt;
	if(key==' ')	{ resetparticles(); }
	if(key=='+') 	{force*=(-1);}
	if(key=='-') 	{force*=0.98;}

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
