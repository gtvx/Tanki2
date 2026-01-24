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

    vec4 fc0; //112 colorConst
} pc;


void main()
{
    v_texcoord = texcoord;

    vec4 r0;
    vec4 r12;

    //1
    r0.x = dot(position, pc.c0);
    r0.y = dot(position, pc.c1);
    r0.z = dot(position, pc.c2);
    //r0.w = 0;

    //MaterialVertex begin

    //2
    //mov r12.xw, r0.xzzz
    r12.x = r0.x;
    r12.w = r0.z;

    //3
    //mov r12.y, -r0
    r12.y = r0.y;

    //4
    //mul r0.z, r0, c3
    r0.z *= pc.c3.z;

    //5
    //add r12.z, r0, c3.w
    r12.z = r0.z + pc.c3.w;

    vec4 c254 = {0.00125, 0.00166667, 0, 0};

    //6
    gl_Position =  r12.w * c254 + r12;

    //MaterialVertex end
}
