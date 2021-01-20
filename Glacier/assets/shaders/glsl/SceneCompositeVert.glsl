#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out PixelInput {
	vec2 TexCoord;
} psInput;

void main() {
	psInput.TexCoord = a_TexCoord;
	vec4 position = vec4(a_Position.xy, 0.0, 1.0);
	gl_Position = position;
}
