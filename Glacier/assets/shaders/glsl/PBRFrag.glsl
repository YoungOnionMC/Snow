#version 450

layout(location = 0) out vec4 Color;

layout(location = 1) in PixelInput {
	vec3 WorldNormal;

	vec2 TexCoords;
};

struct Light {
	vec3 Direction;
	float pad;
	vec3 Radiance;
	float Multiplier;
};

layout(std140, binding = 2) uniform Environment {
	Light lights;
} environment;



void main() {
	Color = vec4(environment.lights.Radiance, 1.0f);
}