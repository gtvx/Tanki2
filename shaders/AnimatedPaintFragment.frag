#version 440

layout(location = 0) in vec2 texture_0;
layout(location = 1) in vec2 texture_6;

layout(binding = 1) uniform sampler2D sampler_0; //textureResource
layout(binding = 4) uniform sampler2D sampler_4; //spriteSheetResource
layout(binding = 6) uniform sampler2D sampler_6; //lightMapResource

layout(location = 0) out vec4 fragColor;



layout(push_constant) uniform PC {
    vec4 c0; //0 transformConst
    vec4 c1; //16 transformConst
    vec4 c2; //32 transformConst

    vec4 c3; //48 projection

    vec2 c4; //64 uvCorrection

    vec4 c14; //80 uvTransformConst
    vec4 c15; //96 uvTransformConst

    vec4 fc23; //112 fragConst
    vec4 fc24; //128 fragConst
} pc;



float rcp(float v)
{
    return 1.0 / v;
}


vec4 c28 = vec4(0, 1, 0, 0);


void main()
{

    vec4 c23 = pc.fc23;
    vec4 c24 = pc.fc24;

    vec4 r0;
    vec4 r1;
    vec4 r2;
    vec4 r3;
    vec4 r4;
    vec4 r5;
    vec4 r6;
    vec4 r8;

//    dcl t0
//    dcl t6
//    dcl_2d s0
//    dcl_2d s4
//    dcl_2d s6
//    def c28, 0, 1, 0, 0


//0:     rcp r8.x, c24.x
//1:     rcp r8.y, c24.y
//2:     rcp r8.z, c24.z
//3:     rcp r8.w, c24.w

     r8.x = rcp(c24.x);
     r8.y = rcp(c24.y);
     r8.z = rcp(c24.z);
     r8.w = rcp(c24.w);



//4:     mul r4, t6, r8
     //r4 = vec4(texture_6.x, texture_6.y, 1, 1) * r8;
     r4.x = texture_6.x * r8.x;
     r4.y = texture_6.y * r8.y;
     r4.z = 0;
     r4.w = 0;



//5:     frc r4, r4
     r4 = fract(r4);


//6:     mul r4, r4, c24
     r4 = r4 * c24;


//7:     add r4.x, r4.x, c24.z
      r4.x = r4.x + c24.z;

//8:     add r4.y, r4.y, c24.w
      r4.y = r4.y + c24.w;

//9:     texld r0, r4, s4
      r0 = texture(sampler_4, vec2(r4.x, r4.y));

//9:     texld r1, t0, s6
      r1 = texture(sampler_6, texture_0);

//9:     mul r2, r0, r1
      r2 = r0 * r1;

//10:     mul r2, c23.w, r2
      r2 = c23.w * r2;

//11:     add r3, c23.z, -r0
      r3 = c23.z + -r0;

//12:     add r4, c23.z, -r1
      r4 = c23.z + -r1;

//13:     mul r3, c23.w, r3
       r3 = c23.w * r3;

//14:     mul r3, r3, r4
        r3 = r3 * r4;


//15:     add r3, c23.z, -r3
         r3 = c23.z + -r3;

//16:     setp_ge p0, r1.x, c23.z
//17:     (!p0) mov r4.x, c28.x
//19:     (p0) mov r4.x, c28.y
         r4.x = r1.x >= c23.z ? c28.y : c28.x;


//21:     add r4.y, c23.z, -r4.x
          r4.y = c23.z + -r4.x;

//22:     mul r5, r4.x, r3
          r5 = r4.x * r3;

//23:     mul r6, r4.y, r2
          r6 = r4.y * r2;

//24:     add r0, r5, r6
          r0 = r5 + r6;

//25:     mov r0.w, r1.w
          r0.w = r1.w;

//26:     texld r1, t0, s0
          r1 = texture(sampler_0, texture_0);

//26:     add r2.w, c23.z, -r1.w
          r2.w = c23.z + -r1.w;

//27:     mul r0.x, r0.x, r2.w
          r0.x = r0.x * r2.w;

//28:     mul r0.y, r0.y, r2.w
          r0.y = r0.y * r2.w;

//29:     mul r0.z, r0.z, r2.w
          r0.z = r0.z * r2.w;

//30:     add r0.xyz, r0.xyzz, r1.xyzz
          r0.x = r0.x + r1.x;
          r0.y = r0.y + r1.y;
          r0.z = r0.z + r1.z;
          r0.w = 1;

//31:     mov r1, c23.x
//32:     mov r2, c23.x
//33:     mov r3, c23.x
//34:     mov r4, c23.x
//35:     mov r5, c23.x
//36:     mov r6, c23.x

//37:     mov oC0, r0
        fragColor = r0;

}
