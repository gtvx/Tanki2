#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec2 oT0;
layout(location = 1) out vec2 oT6;

out gl_PerVertex { vec4 gl_Position; };

layout(std140, binding = 2) uniform buf {
    vec4 c3; //0 0 projection
    vec4 c17; //4 16 fragmentConst
    vec4 c18; //8 32 fragmentConst
    vec4 c254; //12 48
} ubuf;

layout(push_constant) uniform PC {
    vec4 c0; //0 transformConst
    vec4 c1; //16 transformConst
    vec4 c2; //32 transformConst

    vec4 c4; //48 uvCorrection

    vec4 c14; //64 uvTransformConst
    vec4 c15; //80 uvTransformConst
} pc;



void main()
{
    vec4 r0;

    //mul(v0, va1, vc[4]);
    //mul oT0, v1, c4

    oT0 = texcoord.xy * pc.c4.xy;


    vec2 r1;
    vec4 r12;


    //r0 = position;

    //1
    r0.x = dot(position, pc.c0);
    r0.y = dot(position, pc.c1);
    r0.z = dot(position, pc.c2);
    r0.w = 0;

    vec4 v1 = vec4(texcoord.x, texcoord.y, 0, 0);
    vec4 c14 = pc.c14;
    vec4 c15 = pc.c15;


    //mov(vt1, vc[4]);
    //mov r1, c4
    //r1 = pc.c4;


    //dp3(vt1.x, va1, vc[14]);
    //dp3 r1.x, v1, c14
    r1.x = (v1.x * c14.x) + (v1.y * c14.y) + (v1.z * c14.z);

    //dp3(vt1.y, va1, vc[15]);
    //dp3 r1.y, v1, c15
    r1.y = (v1.x * c15.x) + (v1.y * c15.y) + (v1.z * c15.z);

    //add r1.x, r1.x, c14.z
    r1.x = r1.x + c14.z;

    //add r1.y, r1.y, c15.z
    r1.y = r1.y + c15.z;

    //mov oT6, r1
    oT6.xy = r1.xy;


    //2
    //mov r12.xw, r0.xzzz
    r12.x = r0.x;
    //r12.y = r0.z;
    //r12.z = r0.z;
    r12.w = r0.z;

    //3
    //mov r12.y, -r0
    r12.y = r0.y;

    //4
    //mul r0.z, r0, c3
    r0.z = r0.z * ubuf.c3.z;

    //5
    //add r12.z, r0, c3.w
    r12.z = r0.z + ubuf.c3.w;

    //6
    gl_Position =  r12.w * ubuf.c254 + r12;
}
