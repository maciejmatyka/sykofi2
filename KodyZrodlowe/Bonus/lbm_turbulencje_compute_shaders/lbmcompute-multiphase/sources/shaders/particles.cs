// 2012, http://panoramix.ift.uni.wroc.pl/~maq/eng/
#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

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
layout( location=12) uniform float DT=0.01;
layout( local_size_x = 1000 ) in;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	uint gid = gl_GlobalInvocationID.x;		// move massless particle along 
	vec2 p = Positions[ gid ].xy;			// an instant velocity field
	int i = int(p.x * NX);
	int j = int(p.y * NY);
	int idx = i+j*NX;
	if(F[idx] == 0) 
	{
			p.x = 0;
			return;	
	}
		//				// particle at obstacle?
	float u = dU[idx];
	float v = dV[idx];
	p.x = p.x + u*DT;
	p.y = p.y + v*DT;
	if(p.x > 1) {p.x -=1; p.y=rand(p);}					// boundary
	if(p.x < 0) p.x += 1;
	if(p.y > 1) p.y -= 1;
	if(p.y < 0) p.y += 1;
	Positions[ gid ].xy = p;
}
