// 2012, http://panoramix.ift.uni.wroc.pl/~maq/eng/
#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

/*-------------------- LBM model data -------------------------------------------------------------------------*/

#define NUM_VECTORS 9
#define tau 0.8
#define omega (1.0/tau)		// viscosity, etc.
const int ex[9] = {0,  1,0,-1, 0,  1,-1,-1, 1};
const int ey[9] = {0,  0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9] = {4.0/9.0, 1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0, 1.0/36.0,1.0/36.0,1.0/36.0,1.0/36.0};
#define C_FLD 1
#define C_BND 0

layout( binding=0 ) buffer df0 { float f0[  ]; };
layout( binding=1 ) buffer df1 { float f1[  ]; };
layout( binding=2 ) buffer dcF { int F[  ]; };
layout( binding=3 ) buffer dcU { float U[  ]; };
layout( binding=4 ) buffer dcV { float V[  ]; };
layout( binding=5 ) buffer dcP { float P[  ]; };
layout( binding=6 ) buffer dcEXTFX { float EXTFX[  ]; };
layout( binding=7 ) buffer dcEXTFY { float EXTFY[  ]; };
layout( binding=8 ) buffer dcR { float R[  ]; };
layout( binding=9 ) buffer dcC { float C[  ]; };

layout(location = 0) uniform int NX;
layout(location = 1) uniform int NY;
layout(location = 2) uniform float devFx;
layout(location = 3) uniform float devFy;

layout( local_size_x = 40, local_size_y = 20, local_size_z = 1 ) in;

int per(int x, int nx)		// periodic bnd's
{
	if(x<0) x = nx;
		else if(x>nx) x = 0;
	return x;
}

void main()					
{
	int i = int(gl_GlobalInvocationID.x);
	int j = int(gl_GlobalInvocationID.y);
	int idx = i+j*NX;
	float feq;	
	float rho = 0;
	float u = 0;
	float v = 0;
	if( F[ idx ] == C_FLD )
	{	
		for(int k=0; k<9; k++)			// calculate density and velocity
		{
			rho = rho + f0[idx*NUM_VECTORS+k];
			u = u + f0[idx*NUM_VECTORS+k]*ex[k];
			v = v + f0[idx*NUM_VECTORS+k]*ey[k];
		}
		u /= rho;
		v /= rho;
		U[ idx ] = u;
		V[ idx ] = v;
		R[ idx ] = rho;

		//C[ idx*4+0 ]=(0.8+0.2*i/float(NX))*5.1*exp(-0.1*(1-0.0009*(R[idx-1])));
		//C[ idx*4+1 ]=(1+0.2*j/float(NY))*(1-0.0009*(R[idx-1]));
		//C[ idx*4+2 ]=1-0.0009*(R[idx-1]);

     
  
//red      
//        C[ idx*4+0 ]=(1-0.001*(R[idx-1]));
//		C[ idx*4+1 ]=(1-0.003*(R[idx-1]));
//		C[ idx*4+2 ]=(1-0.003*(R[idx-1]));


//red      
        C[ idx*4+0 ]=(1-0.001*(R[idx-1]));
		C[ idx*4+1 ]=(1-0.002*(R[idx-1]));
		C[ idx*4+2 ]=(1-0.003*(R[idx-1]));


		P[ idx ] = 4.*exp( -190. / rho);

		float G=-120;
		// compute external multiphase force
		EXTFX[idx] = 0;
		EXTFY[idx] = 0;
		for(int k=1; k<9; k++)	// no 0 direction (no self interaction!)
		{
			int ip = i+ex[k];
			int jp = j+ey[k];
			ip=per(ip,NX-1);
			jp=per(jp,NY-1);
			int idxp =  ip+jp*NX;
			EXTFX[ idx ] = EXTFX[idx] + ex[k]*(w[k] * P[idxp] );
			EXTFY[ idx ] = EXTFY[idx] + ey[k]*(w[k] * P[idxp] );
		}

		EXTFX[ idx ] = EXTFX[ idx ] * P[idx] * (-G);
		EXTFY[ idx ] = EXTFY[ idx ] * P[idx] * (-G);

		float fx,fy;

		fx=(devFx + EXTFX[idx])*(tau/rho);
		fy=(devFy + EXTFY[idx])*(tau/rho);

		

		u = u + fx;
		v = v + fy;
		for(int k=0; k<9; k++)		// collision + streaming (the main solver is here, really)
		{
			int ip = i+ex[k];
			int jp = j+ey[k];
			ip=per(ip,NX-1);
			jp=per(jp,NY-1);
			int idxp =  ip+jp*NX;
			if( F[ idxp ] == C_BND )
				f1[ idx*NUM_VECTORS + inv[k] ] = (1-omega) * f0[idx*NUM_VECTORS+k] + omega * w[k] * rho * (1.0f - (3.0f/2.0f) * (u*u + v*v) + 3.0f * (ex[k] * u + ey[k]*v) + (9.0f/2.0f) * (ex[k] * u + ey[k]*v) * (ex[k] * u + ey[k]*v));
			else
				f1[ idxp*NUM_VECTORS + k] = (1-omega) * f0[idx*NUM_VECTORS+k] + omega * w[k] * rho * (1.0f - (3.0f/2.0f) * (u*u + v*v)+ 3.0f * (ex[k] * u + ey[k]*v) + (9.0f/2.0f) * (ex[k] * u + ey[k]*v) * (ex[k] * u + ey[k]*v));
		}
	}
}

