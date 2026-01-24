#version 440

layout(location = 0) in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

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
    fragColor = texture(tex, v_texcoord);

    fragColor.w *= pc.fc0.w;
}
