#version 450

layout(location = 0) out vec4 Color;

struct PixelInput {
	vec2 TexCoord;
};

layout(location = 0) in PixelInput psInput;

#if VULKAN
layout(set = 1, binding = 0) uniform sampler2D u_Texture;
#else
layout(binding = 0) uniform sampler2D u_Texture;
#endif

void main() {
	Color = texture(u_Texture, psInput.TexCoord);
}