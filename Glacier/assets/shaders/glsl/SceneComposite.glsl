#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct PixelInput {
	vec2 TexCoord;
};

layout(location = 0) out PixelInput Output;

void main() {
	vec4 position = vec4(a_Position.xy, 0.0, 1.0);
	Output.TexCoord = a_TexCoord;
	gl_Position = position;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct PixelInput {
	vec2 TexCoord;
};

layout (location = 0) in PixelInput Output;

layout (binding = 0) uniform sampler2D u_Texture;

layout (push_constant) uniform Uniforms {
	float Exposure;
} u_Uniforms;

void main() {
	vec4 color = texture(u_Texture, Output.TexCoord);
	color *= u_Uniforms.Exposure;

	o_Color = color + vec4(0.9, 1.0, 0.7, 1.0);
}