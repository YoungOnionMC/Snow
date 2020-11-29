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

layout (std140, binding = 0) uniform Camera {
    mat4 ViewProjection;
} camera;

layout (location = 1) out LightOutput {
    vec3 LightColor;
} lightOutput;

layout (std140, binding = 1) uniform Light {
    vec3 Albedo;
} light;



void main() {
    gl_Position = camera.ViewProjection * vec4(a_Position, 1.0);
    vsOutput.TexCoord = a_TexCoord;
    vsOutput.TexID = a_TexID;
    vsOutput.Color = a_Color;
    lightOutput.LightColor = light.Albedo;
}