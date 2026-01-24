#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec2 v_texcoord;


out gl_PerVertex { vec4 gl_Position; };


layout(push_constant) uniform PC {
    vec4 c0; //0 transformConst
    vec4 c1; //16 transformConst
    vec4 c2; //32 transformConst

    vec4 c3; //48 projection

    vec4 c4; //64 uvCorrection

    vec4 c14; //80 uvTransformConst
    vec4 c15; //96 uvTransformConst
} pc;


void main()
{
    vec4 r0;
    vec2 r1;
    vec4 r12;


    //1
    r0.x = dot(position, pc.c0);
    r0.y = dot(position, pc.c1);
    r0.z = dot(position, pc.c2);
    //r0.w = 740;

    //r1.z = 1;
    //r1.w = 1;

    //dp3 r1.x, v1, c14
    r1.x = dot(texcoord, vec2(pc.c14));
    //r1.x = dot(texcoord, vec2(pc.c14.x, pc.c14.y));

    //dp3 r1.y, v1, c15
    r1.y = dot(texcoord, vec2(pc.c15));
    //r1.y = dot(texcoord, vec2(pc.c15.x, pc.c15.y));


    //add r1.x, r1.x, c14.z
    r1.x += pc.c14.z;

    //add r1.y, r1.y, c15.z
    r1.y += pc.c15.z;

    v_texcoord = r1;

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
    r0.z = r0.z * pc.c3.z;

    //5
    //add r12.z, r0, c3.w
    r12.z = r0.z + pc.c3.w;

    vec4 c254 = {0.00125, 0.00166667, 0, 0};

    //6
    gl_Position =  r12.w * c254 + r12;
}
