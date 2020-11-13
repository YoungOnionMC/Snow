#version 450 core

layout(location = 0) out vec4 Color

layout(location = 0) in vec4 v_Color

void main() {
    Color = v_Color;
}