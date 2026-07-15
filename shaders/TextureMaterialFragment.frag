#version 440

layout(location = 0) in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

layout(push_constant) uniform PC {
    layout(offset = 192)
    vec4 fc0; //colorConst
    vec4 fc1; //colorConst
} pc;


void main()
{
    fragColor = texture(tex, v_texcoord);

    fragColor.w *= pc.fc0.w;
}
