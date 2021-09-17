#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

#if VULKAN
layout(std140, set = 0, binding = 0) uniform Camera
#else
layout(std140, binding = 0) uniform Camera
#endif
{
    mat4 ViewProjection;
} mainCamera;

layout(location = 0) out vec4 v_Color;

void main() {
    gl_Position = mainCamera.ViewProjection * vec4(a_Position, 1.0);
    v_Color = a_Color;
}