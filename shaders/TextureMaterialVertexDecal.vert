#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec4 v_texcoord;


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

    vec4 fc0; //96 colorConst
    vec4 fc1; //112 colorConst

    vec4 decal_1; //c11 128
    vec4 decal_2; //c12 144
} pc;


void main()
{
    v_texcoord.xy = texcoord.xy;


    vec4 r0;
    vec4 r12;

    //1
    r0.x = dot(position, pc.c0);
    r0.y = dot(position, pc.c1);
    r0.z = dot(position, pc.c2);
    //r0.w = 0;

    //MaterialVertex begin

    vec4 r1;
    vec4 r2;

 //8
    //mul(vt1->x, vt1, vc[11]->w); //mul r1.x, r1, c11.w
    //mul(vt1->y, vt1, vc[12]->w); //mul r1.y, r1, c12.w
    //sub(vt1, vt1, vc[11]); //add r1, r1, -c11
    //dp3(v0->z, vt1, vc[12]); //dp3 oT0.z, r1, c12

    r1 = r0;
    r1.x = r1.x * pc.decal_1.w;  //mul(vt1->x, vt1, vc[11]->w);
    r1.y = r1.y * pc.decal_2.w; //mul r1.y, r1, c12.w
    r1.xyz = r1.xyz - pc.decal_1.xyz; //add r1, r1, -c11
    v_texcoord.z = dot(r1.xyz, pc.decal_2.xyz); //dp3(v0->z, vt1, vc[12]); //dp3 oT0.z, r1, c12
//8


//9
    float r1_z = ubuf.c3.w / r0.z; //div(vt1.z, vc[3].w, vt0);
    r1_z = r1_z + ubuf.c3.z; //add(vt1.z, vt1, vc[3]);
    r1_z = r1_z * ubuf.c3.x; //mul(vt1.z, vt1, vc[3].x);
    r1_z = r1_z - ubuf.c3.y; //sub(vt1.z, vt1, vc[3].y);
    r1_z = r1_z / ubuf.c3.x; //div(vt1.z, vt1, vc[3].x);
    r1_z = r1_z - ubuf.c3.z; //sub(vt1.z, vt1, vc[3]);
    r1_z = ubuf.c3.w / r1_z; //div(vt1.z, vc[3].w, vt1);
    r2 = pc.c4; //mov(vt2, vc[4]);
    r2.xyz = normalize(r0.xyz); //nrm(vt2.xyz, vt0.xyz);
    r1_z = r0.z - r1_z; //sub(vt1.z, vt0, vt1);
    r1_z = r1_z / r2.z; //div(vt1.z, vt1, vt2);
    r2.z = r2.z * r1_z; //mul(vt2, vt2, vt1.z);
    r0 = r0 - r2; //sub(vt0, vt0, vt2);
//9

    //2
    //mov r12.xw, r0.xzzz
    r12.x = r0.x;
    r12.w = r0.z;

    //3
    //mov r12.y, -r0
    r12.y = r0.y;

    //4
    //mul r0.z, r0, c3
    r0.z *= ubuf.c3.z;

    //5
    //add r12.z, r0, c3.w
    r12.z = r0.z + ubuf.c3.w;

    //6
    gl_Position =  r12.w * ubuf.c254 + r12;

    //MaterialVertex end
}
