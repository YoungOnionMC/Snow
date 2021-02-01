#version 450

layout(location = 0) out vec4 Color;

struct PixelInput {
	vec2 TexCoord;
};

layout(location = 0) in PixelInput psInput;
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
	Color = texture(u_Texture, psInput.TexCoord);
}