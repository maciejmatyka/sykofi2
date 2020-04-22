// http://panoramix.ift.uni.wroc.pl/~maq/eng/
#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

/*-------------------- LBM model data -------------------------------------------------------------------------*/

#define NUM_VECTORS 9
#define tau 0.501//0.6
#define omega (1.0/tau)		// viscosity, etc.
//#define nu ((1.0/3.0)* (tau-1.0/2.0)) //112.6666
#define nu ((2.0*tau-1.0)/6.0)//((1.0/3.0)* (tau-1.0/2.0)) //112.6666

#define C 0.06
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

void main()					
{
	int i = int(gl_GlobalInvocationID.x);
	int j = int(gl_GlobalInvocationID.y);
	int idx = i+j*NX;
	float feq[9], fneq[9];	
	float rho = 0;
	float u = 0;
	float v = 0;
    float Pi_x_x, Pi_x_y, Pi_y_y,Q,S,TauS, OMEGAS;
    
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
		u = u + 0.5 * devFx;
		v = v + 0.5 * devFy;

        // Nicolas: compute color
         COL[idx] = 30*(u*u + v*v);     

        //4.
        for(int k=0; k<9; k++)		// collision + streaming (the main solver is here, really)
		{
			int ip = i+ex[k];
			int jp = j+ey[k];
			ip=per(ip,NX-1);
			jp=per(jp,NY-1);
			int idxp =  ip+jp*NX;

            // compute feq


            //Compute the non equilibrium part of the distribution functions
            //fi_neq = fi - fi_eq

            feq[k] = w[k] * rho * (1.0f - (3.0f/2.0f) * (u*u + v*v) + 3.0f * (ex[k] * u + ey[k]*v) + (9.0f/2.0f) * (ex[k] * u + ey[k]*v) * (ex[k] * u + ey[k]*v));
            //fneq[k] = f0[idx*NUM_VECTORS+k]*ex[k] - feq[k];
        }

    
    Pi_x_x = (f0[idx*NUM_VECTORS+1]-feq[1])+(f0[idx*NUM_VECTORS+3]-feq[3])+(f0[idx*NUM_VECTORS+5]-feq[5])+(f0[idx*NUM_VECTORS+6]-feq[6])+(f0[idx*NUM_VECTORS+7]-feq[7])+(f0[idx*NUM_VECTORS+8]-feq[8]);
    Pi_x_y = (f0[idx*NUM_VECTORS+5]-feq[5])-(f0[idx*NUM_VECTORS+6]-feq[6])+(f0[idx*NUM_VECTORS+7]-feq[7])-(f0[idx*NUM_VECTORS+8]-feq[8]);
    Pi_y_y = (f0[idx*NUM_VECTORS+2]-feq[2])+(f0[idx*NUM_VECTORS+4]-feq[4])+(f0[idx*NUM_VECTORS+5]-feq[5])+(f0[idx*NUM_VECTORS+6]-feq[6])+(f0[idx*NUM_VECTORS+7]-feq[7])+(f0[idx*NUM_VECTORS+8]-feq[8]);
    Q = Pi_x_x*Pi_x_x + 2.0*Pi_x_y*Pi_x_y + Pi_y_y*Pi_y_y;
    S = (1.0/6.0) * (sqrt(nu*nu + 18.0 *C*C * sqrt(Q) ) - nu );
    TauS = (6.0*( nu + S ) + 1.0)/2.0;
 //relaxation

/*
Pij = sum of ei*ej*fk_neq where i, j are x or y and k is in 1..8
i.e.,
Pxx = sum of e[k].x * e[k].x * f_neq[k] where e[k] is the direction
of the node k.
Pxy = sum of e[k].x * e[k].y * f_neq[k]
Pyx = Pxy !
Pyy = ...
6) Compute the magnitude of this non equilibrium stress tensor
Q = sqrt(Pxx*Pxx + 2 * Pxy*Pxy + Pyy*Pyy)
7) Compute the magnitude of the stress - tensor itself
S = 1 / 6 * (sqrt(nu*nu + 18 * C*C * Q) - nu)
where nu is the viscosity of the fluid
C  is a constant to adjust(usually around 0.03)
usually try to set it as small as possible while keeping the
simulation stable
8) Compute the relaxation time from the stress tensor
Tau = (6 * (nu + S) + 1) / 2
9) Use it to do the relaxation as usual
 */

    OMEGAS = 1.0/TauS;
    //OMEGAS = 1.0/tau;

		for(int k=0; k<9; k++)		// collision + streaming (the main solver is here, really)
		{
			int ip = i+ex[k];
			int jp = j+ey[k];
			ip=per(ip,NX-1);
			jp=per(jp,NY-1);
			int idxp =  ip+jp*NX;

            // compute feq
			if( F[ idxp ] == C_BND )
				f1[ idx*NUM_VECTORS + inv[k] ] = (1-OMEGAS) * f0[idx*NUM_VECTORS+k] + OMEGAS * feq[k];//omega * feq[k];
			else
				f1[ idxp*NUM_VECTORS + k] = (1-OMEGAS) * f0[idx*NUM_VECTORS+k] + OMEGAS * feq[k];//omega * feq[k];
		}
     
	}



}

