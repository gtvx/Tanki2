#version 440

layout(location = 0) in vec4 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

layout(push_constant) uniform PC {
    vec4 c0; //0 transformConst
    vec4 c1; //16 transformConst
    vec4 c2; //32 transformConst

    vec4 c4; //48 uvCorrection

    vec4 c14; //64 uvTransformConst
    vec4 c15; //80 uvTransformConst

    vec4 fc0; //96 colorConst
    vec4 fc1; //112 colorConst
} pc;


void main()
{
    //vec4 t1;
    //abs(ft1, v0->z); //abs r1, t0.z
    //sat(ft1, ft1); //mov_sat r1, r1
    //sub(ft1, fc[17], ft1); //add r1, c17, -r1
    //mul(ft0->w, ft0, ft1); //mul r0.w, r0, r1

    fragColor = texture(tex, v_texcoord.xy);
    fragColor.w *= pc.fc0.w;

    float ft1 = abs(v_texcoord.z);
    ft1 = clamp(ft1, 0.0, 1.0);   // saturate
    ft1 = 1 - ft1;

    fragColor.w *= ft1;
}
