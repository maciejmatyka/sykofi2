// http://panoramix.ift.uni.wroc.pl/~maq/eng/
#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

/*-------------------- LBM model data -------------------------------------------------------------------------*/

#define NUM_VECTORS 9
#define tau 0.50001 //0.6
#define w (1.f/tau)
#define wb 1
#define w3 1
#define w4 1
//#define omega (1.0/tau)		// viscosity, etc.
//#define nu ((1.0/3.0)* (tau-1.0/2.0)) //112.6666
//#define nu ((2.0*tau-1.0)/6.0)//((1.0/3.0)* (tau-1.0/2.0)) //112.6666
//#define C 0.06

/*
const int ex[9] = {0,  1,0,-1, 0,  1,-1,-1, 1};
const int ey[9] = {0,  0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9] = {4.0/9.0, 1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0, 1.0/36.0,1.0/36.0,1.0/36.0,1.0/36.0};
*/
#define C_FLD 1
#define C_BND 0

layout( binding=0 ) buffer df0 { float df[  ]; };
layout( binding=1 ) buffer df1 { float df_tmp[  ]; };
layout( binding=2 ) buffer dcF { int F[  ]; };
layout( binding=3 ) buffer dcU { float U[  ]; };
layout( binding=4 ) buffer dcV { float V[  ]; };
layout( binding=5 ) buffer col { float COL[  ]; };
layout(location = 0) uniform int NX;
layout(location = 1) uniform int NY;
layout(location = 2) uniform float devFx;
layout(location = 3) uniform float devFy;

layout( local_size_x = 10, local_size_y = 10, local_size_z = 1 ) in;

int per(int x, int nx)		// periodic bnd's
{
  if(x<0) x = nx;
  else if(x>nx) x = 0;
  return x;
}

//macros for memory access
#define I2D(x,y,nx,ny) ((x) + (y)*(nx))
#define Idf2D(i,x,y,nx,ny) ((x+y*nx)*NUM_VECTORS + (i))
#define df2D(    i,x,y)  (df[    Idf2D(i,x,y,NX,NY)])
#define dftmp2D( i,x,y)  (df_tmp[Idf2D(i,x,y,NX,NY)])

//Kernel using the Cascaded LBM algorithm
void main()
{
  int x = int(gl_GlobalInvocationID.x);
  int y = int(gl_GlobalInvocationID.y);
  //=== STREAMING STEP (PERIODIC) =======================
  int xplus  = ((x==NX-1) ? (0) : (1+x));
  int xminus = ((x==0) ? (NX-1) : (x-1));
  int yplus  = ((y==NY-1) ? (0) : (1+y));
  int yminus = ((y==0) ? (NY-1) : (y-1));
  //Store the distribution functions in registers
  float f0 = df2D(0, x     ,y     );
  float f1 = df2D(1, xminus,y     );
  float f2 = df2D(2, x     ,yminus);
  float f3 = df2D(3, xplus ,y     );
  float f4 = df2D(4, x     ,yplus );
  float f5 = df2D(5, xminus,yminus);
  float f6 = df2D(6, xplus ,yminus);
  float f7 = df2D(7, xplus ,yplus );
  float f8 = df2D(8, xminus,yplus );

  //if solid node, bounce-back
  if( F[I2D(x,y,NX,NY)] != C_FLD )
  {
    dftmp2D(0, x,y) = f0;
    dftmp2D(1, x,y) = f3;
    dftmp2D(2, x,y) = f4;
    dftmp2D(3, x,y) = f1;
    dftmp2D(4, x,y) = f2;
    dftmp2D(5, x,y) = f7;
    dftmp2D(6, x,y) = f8;
    dftmp2D(7, x,y) = f5;
    dftmp2D(8, x,y) = f6;
    COL[I2D(x,y,NX,NY)] = 0;
    return;
  }
  //=== COMPUTE MOMENTS =================================
  //density
  float rho = f0+f1+f2+f3+f4+f5+f6+f7+f8;
  //velocity
  float vx = 1/rho*(f1-f3+f5-f6-f7+f8);
  float vy = 1/rho*(f2-f4+f5+f6-f7-f8);

  vx += 0.5 * devFx;
  vy += 0.5 * devFy;

  //with the following "hack" the cascaded LBM will be ALWAYS stable!
  float norm = sqrt(vx*vx+vy*vy);
#define NORM 0.42f
  if(norm>NORM)
  {
    vx *= NORM/norm;
    vy *= NORM/norm;
    norm = NORM;
  }

  U[I2D(x,y,NX,NY)] = vx;
  V[I2D(x,y,NX,NY)] = vy;

  //COL[I2D(x,y,NX,NY)] = 5*norm;

    //curlx[i+j*NX] = (U_host[i+jp*NX] - U_host[i+j*NX]);
    //curly[i+j*NX] = (V_host[ip+j*NX] - V_host[i+j*NX]);
    //c = curlx[i+j*NX]*curlx[i+j*NX] + curly[i+j*NX]*curly[i+j*NX];


//    float c 
    COL[I2D(x,y,NX,NY)]= 3000*((U[I2D(x,y+1,NX,NY)] - U[I2D(x,y,NX,NY)])*(U[I2D(x,y+1,NX,NY)] - U[I2D(x,y,NX,NY)]) + (V[I2D(x+1,y,NX,NY)] - V[I2D(x,y,NX,NY)])*(V[I2D(x+1,y,NX,NY)] - V[I2D(x,y,NX,NY)]));

    COL[I2D(x,y,NX,NY)] = (0.5*COL[I2D(x,y,NX,NY)] + 2*norm)/2;

  //trace of the pressure tensor
  float T = 1/rho*((((-vx)*(-vx)))*f0+(((1-vx)*(1-vx)))*f1+(((-vx)*(-vx)))*f2+(((-1-vx)*(-1-vx)))*f3+(((-vx)*(-vx)))*f4+(((1-vx)*(1-vx)))*f5+(((-1-vx)*(-1-vx)))*f6+(((-1-vx)*(-1-vx)))*f7+(((1-vx)*(1-vx)))*f8)+1/rho*((((-vy)*(-vy)))*f0+(((-vy)*(-vy)))*f1+(((1-vy)*(1-vy)))*f2+(((-vy)*(-vy)))*f3+(((-1-vy)*(-1-vy)))*f4+(((1-vy)*(1-vy)))*f5+(((1-vy)*(1-vy)))*f6+(((-1-vy)*(-1-vy)))*f7+(((-1-vy)*(-1-vy)))*f8);
  //normal stress difference
  float N = 1/rho*((((-vx)*(-vx)))*f0+(((1-vx)*(1-vx)))*f1+(((-vx)*(-vx)))*f2+(((-1-vx)*(-1-vx)))*f3+(((-vx)*(-vx)))*f4+(((1-vx)*(1-vx)))*f5+(((-1-vx)*(-1-vx)))*f6+(((-1-vx)*(-1-vx)))*f7+(((1-vx)*(1-vx)))*f8)-(1/rho*((((-vy)*(-vy)))*f0+(((-vy)*(-vy)))*f1+(((1-vy)*(1-vy)))*f2+(((-vy)*(-vy)))*f3+(((-1-vy)*(-1-vy)))*f4+(((1-vy)*(1-vy)))*f5+(((1-vy)*(1-vy)))*f6+(((-1-vy)*(-1-vy)))*f7+(((-1-vy)*(-1-vy)))*f8));
  //off diagonal element of the stress tensor
  float Pxy = 1/rho*(((-vx)*(-vy))*f0+((1-vx)*(-vy))*f1+((-vx)*(1-vy))*f2+((-1-vx)*(-vy))*f3+((-vx)*(-1-vy))*f4+((1-vx)*(1-vy))*f5+((-1-vx)*(1-vy))*f6+((-1-vx)*(-1-vy))*f7+((1-vx)*(-1-vy))*f8);
  //fisrt third order moment
  float Qyxx = 1/rho*((((-vx)*(-vx))*(-vy))*f0+(((1-vx)*(1-vx))*(-vy))*f1+(((-vx)*(-vx))*(1-vy))*f2+(((-1-vx)*(-1-vx))*(-vy))*f3+(((-vx)*(-vx))*(-1-vy))*f4+(((1-vx)*(1-vx))*(1-vy))*f5+(((-1-vx)*(-1-vx))*(1-vy))*f6+(((-1-vx)*(-1-vx))*(-1-vy))*f7+(((1-vx)*(1-vx))*(-1-vy))*f8);
  //second third order moment
  float Qxyy = 1/rho*(((-vx)*((-vy)*(-vy)))*f0+((1-vx)*((-vy)*(-vy)))*f1+((-vx)*((1-vy)*(1-vy)))*f2+((-1-vx)*((-vy)*(-vy)))*f3+((-vx)*((-1-vy)*(-1-vy)))*f4+((1-vx)*((1-vy)*(1-vy)))*f5+((-1-vx)*((1-vy)*(1-vy)))*f6+((-1-vx)*((-1-vy)*(-1-vy)))*f7+((1-vx)*((-1-vy)*(-1-vy)))*f8);
  //fourth order moment
  float A = 1/rho*((((-vx)*(-vx))*((-vy)*(-vy)))*f0+(((1-vx)*(1-vx))*((-vy)*(-vy)))*f1+(((-vx)*(-vx))*((1-vy)*(1-vy)))*f2+(((-1-vx)*(-1-vx))*((-vy)*(-vy)))*f3+(((-vx)*(-vx))*((-1-vy)*(-1-vy)))*f4+(((1-vx)*(1-vx))*((1-vy)*(1-vy)))*f5+(((-1-vx)*(-1-vx))*((1-vy)*(1-vy)))*f6+(((-1-vx)*(-1-vx))*((-1-vy)*(-1-vy)))*f7+(((1-vx)*(1-vx))*((-1-vy)*(-1-vy)))*f8);

  //=== RELAX TOWARD EQUILIBRIUM ========================
  Pxy = (1-w)*Pxy;
  N = (1-w)*N;
  T = (1-wb)*T+0.66666666666667*wb;
  Qxyy = (1-w3)*Qxyy;
  Qyxx = (1-w3)*Qyxx;
  A = (1-w4)*A+0.11111111111111*w4;

  dftmp2D(0, x,y) = rho*(1-(vx*vx+vy*vy)+4*vx*vy*Pxy-(((vx*vx-vy*vy)/2)*N)+((-2+vx*vx+vy*vy)/2)*T+2*vx*Qxyy+2*vy*Qyxx+A);
  dftmp2D(1, x,y) = rho/2*(vx*vx+vx*(1-vy*vy)-((2*vy+4*vx*vy)*Pxy)+((1+vx+vx*vx-vy*vy)/2)*N+((1-vx-(vx*vx+vy*vy))/2)*T-((1+2*vx)*Qxyy)-2*vy*Qyxx-A);
  dftmp2D(2, x,y) = rho/2*(vy*vy+vy*(1-vx*vx)-((2*vx+4*vx*vy)*Pxy)+((-1-vy+vx*vx-vy*vy)/2)*N+((1-vy-(vx*vx+vy*vy))/2)*T-((1+2*vy)*Qyxx)-2*vx*Qxyy-A);
  dftmp2D(3, x,y) = rho/2*(vx*vx+(-vx)*(1-vy*vy)-((-2*vy+4*vx*vy)*Pxy)+((1-vx+vx*vx-vy*vy)/2)*N+((1-(-vx)-(vx*vx+vy*vy))/2)*T-((-1+2*vx)*Qxyy)-2*vy*Qyxx-A);
  dftmp2D(4, x,y) = rho/2*(vy*vy+(-vy)*(1-vx*vx)-((-2*vx+4*vx*vy)*Pxy)+((-1-(-vy)+vx*vx-vy*vy)/2)*N+((1-(-vy)-(vx*vx+vy*vy))/2)*T-((-1+2*vy)*Qyxx)-2*vx*Qxyy-A);
  dftmp2D(5, x,y) = rho/4*(vx*vy+vx*vy*vy+vy*vx*vx+(1+4*vx*vy+2*vy+2*vx)*Pxy+((-vx*vx+vy*vy-vx+vy)/2)*N+((vx*vx+vy*vy+vx+vy)/2)*T+(1+2*vx)*Qxyy+(1+2*vy)*Qyxx+A);
  dftmp2D(6, x,y) = rho/4*((-vx)*vy+(-vx)*vy*vy+vy*vx*vx+(-1+4*vx*vy-2*vy+2*vx)*Pxy+((-vx*vx+vy*vy-(-vx)+vy)/2)*N+((vx*vx+vy*vy-vx+vy)/2)*T+(-1+2*vx)*Qxyy+(1+2*vy)*Qyxx+A);
  dftmp2D(7, x,y) = rho/4*(vx*vy+(-vx)*vy*vy+(-vy)*vx*vx+(1+4*vx*vy-2*vy-2*vx)*Pxy+((-vx*vx+vy*vy-(-vx)-vy)/2)*N+((vx*vx+vy*vy-vx-vy)/2)*T+(-1+2*vx)*Qxyy+(-1+2*vy)*Qyxx+A);
  dftmp2D(8, x,y) = rho/4*((-vx)*vy+vx*vy*vy+(-vy)*vx*vx+(-1+4*vx*vy+2*vy-2*vx)*Pxy+((-vx*vx+vy*vy-vx-vy)/2)*N+((vx*vx+vy*vy+vx-vy)/2)*T+(1+2*vx)*Qxyy+(-1+2*vy)*Qyxx+A);
}
//BGK Implementation copied from CUDA
/*
void main()
{
  int x = int(gl_GlobalInvocationID.x);
  int y = int(gl_GlobalInvocationID.y);
  //=== STREAMING STEP (PERIODIC) =======================
  int xplus  = ((x==NX-1) ? (0) : (1+x));
  int xminus = ((x==0) ? (NX-1) : (x-1));
  int yplus  = ((y==NY-1) ? (0) : (1+y));
  int yminus = ((y==0) ? (NY-1) : (y-1));
  //Store the distribution functions in registers
  float f0 = df2D(0, x     ,y     );
  float f1 = df2D(1, xminus,y     );
  float f2 = df2D(2, x     ,yminus);
  float f3 = df2D(3, xplus ,y     );
  float f4 = df2D(4, x     ,yplus );
  float f5 = df2D(5, xminus,yminus);
  float f6 = df2D(6, xplus ,yminus);
  float f7 = df2D(7, xplus ,yplus );
  float f8 = df2D(8, xminus,yplus );

  //if solid node, bounce-back
  if( F[I2D(x,y,NX,NY)] == C_BND )
  {
    dftmp2D(0, x,y) = f0;
    dftmp2D(1, x,y) = f3;
    dftmp2D(2, x,y) = f4;
    dftmp2D(3, x,y) = f1;
    dftmp2D(4, x,y) = f2;
    dftmp2D(5, x,y) = f7;
    dftmp2D(6, x,y) = f8;
    dftmp2D(7, x,y) = f5;
    dftmp2D(8, x,y) = f6;
    COL[I2D(x,y,NX,NY)] = 0;
    return;
  }
  //=== COMPUTE MOMENTS =================================
  //density
  float rho = f0+f1+f2+f3+f4+f5+f6+f7+f8;
  //velocity
  float vx = 1/rho*(f1-f3+f5-f6-f7+f8);
  float vy = 1/rho*(f2-f4+f5+f6-f7-f8);

  vx += 0.5 * devFx;
  vy += 0.5 * devFy;

  COL[I2D(x,y,NX,NY)] = 5*sqrt(vx*vx + vy*vy);     

  //=== COMPUTE EQUILIBRIUM DISTRIBUTION FUNCTION =======
  float f0eq = 4.f/9.f*rho*(1-(1.5f*(vx*vx+vy*vy)));
  float f1eq = 1.f/9.f*rho*(1+3*vx+4.5f*vx*vx-(1.5f*(vx*vx+vy*vy)));
  float f2eq = 1.f/9.f*rho*(1+3*vy+4.5f*vy*vy-(1.5f*(vx*vx+vy*vy)));
  float f3eq = 1.f/9.f*rho*(1+3*(-vx)+4.5f*((-vx)*(-vx))-(1.5f*(vx*vx+vy*vy)));
  float f4eq = 1.f/9.f*rho*(1+3*(-vy)+4.5f*((-vy)*(-vy))-(1.5f*(vx*vx+vy*vy)));
  float f5eq = 1.f/36.f*rho*(1+3*(vx+vy)+4.5f*((vx+vy)*(vx+vy))-(1.5f*(vx*vx+vy*vy)));
  float f6eq = 1.f/36.f*rho*(1+3*(-vx+vy)+4.5f*((-vx+vy)*(-vx+vy))-(1.5f*(vx*vx+vy*vy)));
  float f7eq = 1.f/36.f*rho*(1+3*(-vx-vy)+4.5f*((-vx-vy)*(-vx-vy))-(1.5f*(vx*vx+vy*vy)));
  float f8eq = 1.f/36.f*rho*(1+3*(vx-vy)+4.5f*((vx-vy)*(vx-vy))-(1.5f*(vx*vx+vy*vy)));

  //=== RELAX TOWARD EQUILIBRIUM ========================
  dftmp2D(0, x,y) = (1-1.f/tau)*f0+1.f/tau*f0eq;
  dftmp2D(1, x,y) = (1-1.f/tau)*f1+1.f/tau*f1eq;
  dftmp2D(2, x,y) = (1-1.f/tau)*f2+1.f/tau*f2eq;
  dftmp2D(3, x,y) = (1-1.f/tau)*f3+1.f/tau*f3eq;
  dftmp2D(4, x,y) = (1-1.f/tau)*f4+1.f/tau*f4eq;
  dftmp2D(5, x,y) = (1-1.f/tau)*f5+1.f/tau*f5eq;
  dftmp2D(6, x,y) = (1-1.f/tau)*f6+1.f/tau*f6eq;
  dftmp2D(7, x,y) = (1-1.f/tau)*f7+1.f/tau*f7eq;
  dftmp2D(8, x,y) = (1-1.f/tau)*f8+1.f/tau*f8eq;
}
*/
