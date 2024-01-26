#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;


layout(location = 0) out vec4 v_Color;

layout(std140, binding = 0) uniform Camera {
	mat4 ViewProjection;
};

layout(push_constant) uniform Transform {
    mat4 Transform;
} u_Renderer;

void main() {
	vec4 position = ViewProjection * u_Renderer.Transform * vec4(a_Position, 1.0);

	v_Color = a_Color;
	gl_Position = position;
}

#type pixel
#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec4 v_Color;

void main() {
	color = v_Color;
}