#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexID;
layout(location = 3) in vec3 a_Color;

layout (location = 0) out VertexOutput {
	vec2 TexCoord;
	float TexID;
    vec3 Color;
} vsOutput;

#if VULKAN
layout(set = 0, binding = 0) uniform Camera 
#else
layout(binding = 0) uniform Camera 
#endif
{
	mat4 ViewProjection;
} mainCamera;

void main() {
	gl_Position = mainCamera.ViewProjection * vec4(a_Position, 1.0);
	vsOutput.TexCoord = a_TexCoord;
	vsOutput.TexID = a_TexID;
    vsOutput.Color = a_Color;
}

#type pixel
#version 450

layout(location = 0) out vec4 Color;

layout (location = 0) in VertexOutput {
	vec2 TexCoord;
	float TexID;
    vec3 Color;
} psInput;

#if VULKAN
layout(set = 1, binding = 0) uniform sampler2D u_Textures[32];
#else
layout(binding = 0) uniform sampler2D u_Textures[32];
#endif

void main() {
	float alpha = 0.0f;
    switch(int(psInput.TexID)) {
    case 0: alpha = texture(u_Textures[0], psInput.TexCoord).r; break;
    case 1: alpha = texture(u_Textures[1], psInput.TexCoord).r; break;
    case 2: alpha = texture(u_Textures[2], psInput.TexCoord).r; break;
    case 3: alpha = texture(u_Textures[3], psInput.TexCoord).r; break;
    case 4: alpha = texture(u_Textures[4], psInput.TexCoord).r; break;
    case 5: alpha = texture(u_Textures[5], psInput.TexCoord).r; break;
    case 6: alpha = texture(u_Textures[6], psInput.TexCoord).r; break;
    case 7: alpha = texture(u_Textures[7], psInput.TexCoord).r; break;
    case 8: alpha = texture(u_Textures[8], psInput.TexCoord).r; break;
    case 9: alpha = texture(u_Textures[9], psInput.TexCoord).r; break;
    case 10: alpha = texture(u_Textures[10], psInput.TexCoord).r; break;
    case 11: alpha = texture(u_Textures[11], psInput.TexCoord).r; break;
    case 12: alpha = texture(u_Textures[12], psInput.TexCoord).r; break;
    case 13: alpha = texture(u_Textures[13], psInput.TexCoord).r; break;
    case 14: alpha = texture(u_Textures[14], psInput.TexCoord).r; break;
    case 15: alpha = texture(u_Textures[15], psInput.TexCoord).r; break;
    case 16: alpha = texture(u_Textures[16], psInput.TexCoord).r; break;
    case 17: alpha = texture(u_Textures[17], psInput.TexCoord).r; break;
    case 18: alpha = texture(u_Textures[18], psInput.TexCoord).r; break;
    case 19: alpha = texture(u_Textures[19], psInput.TexCoord).r; break;
    case 20: alpha = texture(u_Textures[20], psInput.TexCoord).r; break;
    case 21: alpha = texture(u_Textures[21], psInput.TexCoord).r; break;
    case 22: alpha = texture(u_Textures[22], psInput.TexCoord).r; break;
    case 23: alpha = texture(u_Textures[23], psInput.TexCoord).r; break;
    case 24: alpha = texture(u_Textures[24], psInput.TexCoord).r; break;
    case 25: alpha = texture(u_Textures[25], psInput.TexCoord).r; break;
    case 26: alpha = texture(u_Textures[26], psInput.TexCoord).r; break;
    case 27: alpha = texture(u_Textures[27], psInput.TexCoord).r; break;
    case 28: alpha = texture(u_Textures[28], psInput.TexCoord).r; break;
    case 29: alpha = texture(u_Textures[29], psInput.TexCoord).r; break;
    case 30: alpha = texture(u_Textures[30], psInput.TexCoord).r; break;
    case 31: alpha = texture(u_Textures[31], psInput.TexCoord).r; break;
    }

    Color = vec4(psInput.Color, alpha);
}