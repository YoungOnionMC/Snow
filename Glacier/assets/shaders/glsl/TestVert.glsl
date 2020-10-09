#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_TexID;
layout (location = 3) in vec4 a_Color;

layout (location = 0) out VertexOutput {
    vec2 TexCoord;
    float TexID;
    vec4 Color;
} vsOutput;

void main() {
    gl_Position = vec4(a_Position, 1.0);
    vsOutput.TexCoord = a_TexCoord;
    vsOutput.TexID = a_TexID;
    vsOutput.Color = a_Color;
}