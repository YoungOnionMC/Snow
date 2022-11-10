#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec3 v_Position;

layout(std140, binding = 0) uniform Camera {
	mat4 u_ViewProjMatrix;
	mat4 u_InvViewProjMatrix;
};

void main() {
	vec4 position = vec4(a_Position.xy, 1.0, 1.0);
	gl_Position = position;

	v_Position = (u_InvViewProjMatrix * position).xyz;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 unused0;
layout(location = 2) out vec4 unused1;

layout(location = 0) in vec3 v_Position;

layout(binding = 1) uniform samplerCube u_Texture;

layout(push_constant) uniform Uniforms {
	float TextureLod;
	float Intensity;
} u_Uniforms;

void main() {
	Color = textureLod(u_Texture, v_Position, u_Uniforms.TextureLod) * u_Uniforms.Intensity;
	//Color = vec4(1.0, 0.0, 0.0, 1.0);
	unused0 = vec4(0.0);
	unused1 = vec4(0.0);
}
