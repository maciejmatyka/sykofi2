// 2012, http://panoramix.ift.uni.wroc.pl/~maq/eng/
#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

#define C_FLD 1
#define C_BND 0

struct pos
{
	vec2 xy;
};

layout(  binding=69 ) buffer ParticlesPos
{
	struct pos Positions[  ];
};

layout( binding=2 ) buffer dcF { int F[  ]; };
layout( binding=3 ) buffer dcU { float dU[  ]; };
layout( binding=4 ) buffer dcV { float dV[  ]; };
layout(location = 0) uniform int NX;
layout(location = 1) uniform int NY;
layout( location=12) uniform float DT;
layout( local_size_x = 1000 ) in;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


float BilinearInterpolationC(float x,float y,float x1,float x2,float y1,float y2,float f11,float f21,float f22,float f12)
{
	float t = (x-x1) / (x2 - x1);
	float u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}


void main()
{
	uint gid = gl_GlobalInvocationID.x;		// move massless particle along 
	vec2 p = Positions[ gid ].xy;			// an instant velocity field
	int i = int(p.x * NX);
	int j = int(p.y * NY);
	int idx = i+j*NX;
/*	if(F[idx] == 0) 
	{
			p.x = 0.01;
			return;	
	}*/

	//				// particle at obstacle?
	float u;// = dU[idx];
	float v;// = dV[idx];

    u = BilinearInterpolationC(p.x*(NX),p.y*(NY), i,i+1, j,j+1, dU[j*NX+i],dU[j*NX+i+1],dU[(j+1)*NX+i+1],dU[(j+1)*NX+i]);
	v = BilinearInterpolationC(p.x*(NX),p.y*(NY), i,i+1, j,j+1, dV[j*NX+i],dV[j*NX+i+1],dV[(j+1)*NX+i+1],dV[(j+1)*NX+i]);

    p.x = p.x + u*DT;
	p.y = p.y + v*DT;
//    p.x = p.x + 0.01*DT;
//	p.y = p.y + 0.01*DT;

//#define A 0.5


    i = int(p.x * NX);
    j = int(p.y * NY);


    if(p.x < 0) p.x += 1;
	if(p.x > 1) p.x -= 1;
	if(p.y > 1) p.y -= 1;
	if(p.y < 0) p.y += 1;

    if(F[ i+j*NX ] == C_BND)
    //do
    //{
{
//        p.x += 0.1;
        p.x = rand(p.xy);   // 0-1
        p.y = rand(p.xy);   // 0-1
}
     //   i = int(p.x * NX);
     //   j = int(p.y * NY);
    //} while( F[ i+j*NX ] == C_BND);


	Positions[ gid ].xy = p;
}
