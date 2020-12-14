#version 450

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct PixelInput {
	vec2 TexCoord;
};

layout(location = 0) out PixelInput psInput;

void main() {
	gl_Position = vec4(a_Position, 0.0, 1.0);
	psInput.TexCoord = a_TexCoord;
}