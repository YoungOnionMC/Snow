#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 ViewProjection;
} mainCamera;

layout(std140, binding = 1) uniform ObjectTransform
{
	mat4 Transform;
} transform;

layout(location = 0) out PixelInput {
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoords;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;
} psInput;

void main() {
	psInput.WorldPosition = vec3(transform.Transform * vec4(a_Position, 1.0));
	psInput.Normal = mat3(transform.Transform) * a_Normal;
	psInput.TexCoords = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	psInput.WorldNormals = mat3(transform.Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
	psInput.WorldTransform = mat3(transform.Transform);
	psInput.Binormal = a_Binormal;

	gl_Position = mainCamera.ViewProjection * transform.Transform * vec4(a_Position, 1.0);
}