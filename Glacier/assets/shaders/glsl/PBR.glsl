#type vertex
#version 450 core


layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec3 a_Tangent;
layout (location = 3) in vec3 a_Binormal;
layout (location = 4) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjMatrix;
    mat4 u_InverseViewProjMatrix;
    mat4 u_ProjMatrix;
    mat4 u_ViewMatrix;
};

layout(push_constant) uniform Transform {
    mat4 Transform;
} u_Renderer;

struct VertexOutput {
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    mat3 WorldTransform;
    vec3 Binormal;

    mat3 CameraView;

    vec3 ViewPosition;
};

layout(location = 0) out VertexOutput Output;

void main() {

    Output.WorldPosition = vec3(u_Renderer.Transform * vec4(a_Position, 1.0));
    Output.Normal = mat3(u_Renderer.Transform) * a_Normal;
    Output.TexCoord = vec2(a_TexCoord.x, 1.0-a_TexCoord.y);
    Output.WorldNormals = mat3(u_Renderer.Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
    Output.WorldTransform = mat3(u_Renderer.Transform);
    Output.Binormal = a_Binormal;

    Output.CameraView = mat3(u_ViewMatrix);

    Output.ViewPosition = vec3(u_ViewMatrix * vec4(Output.WorldPosition, 1.0));

    gl_Position = u_ViewProjMatrix * u_Renderer.Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

const float PI = 3.141592;
const float Epsilon = 0.00001;


const vec3 Fdielectric = vec3(0.04);

struct VertexOutput {
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    mat3 WorldTransform;
    vec3 Binormal;

    mat3 CameraView;

    vec3 ViewPosition;
};

layout (location = 0) in VertexOutput Input;

layout (location = 0) out vec4 Color;
layout (location = 1) out vec4 o_ViewNormals;
layout (location = 2) out vec4 o_ViewPositions;

layout (set = 0, binding = 5) uniform sampler2D u_AlbedoTexture;
layout (set = 0, binding = 6) uniform sampler2D u_NormalTexture;
layout (set = 0, binding = 7) uniform sampler2D u_MetalnessTexture;
layout (set = 0, binding = 8) uniform sampler2D u_RoughnessTexture;

layout(set = 1, binding = 11) uniform sampler2D u_BRDFLUTTexture;

layout(push_constant) uniform Material {
    layout(offset = 64) vec3 AlbedoColor;
    float Metalness;
    float Roughness;
    float Emission;

    float EnvMapRotation;

    bool UseNormalMap;
} u_MaterialUniforms;

struct PBRParamaters {
    vec3 Albedo;
    float Roughness;
    float Metalness;

    vec3 Normal;
    vec3 View;
    float NdotV;
};

PBRParamaters m_Params;

void main() {
    m_Params.Albedo = texture(u_AlbedoTexture, Input.TexCoord).rgb * u_MaterialUniforms.AlbedoColor;
    float alpha = texture(u_AlbedoTexture, Input.TexCoord).a;
    m_Params.Metalness = texture(u_MetalnessTexture, Input.TexCoord).r * u_MaterialUniforms.Metalness;
    m_Params.Roughness = texture(u_RoughnessTexture, Input.TexCoord).r * u_MaterialUniforms.Roughness;
    m_Params.Roughness = max(m_Params.Roughness, 0.05);

    m_Params.Normal = normalize(Input.Normal);
    if(u_MaterialUniforms.UseNormalMap) {
        m_Params.Normal = normalize(texture(u_NormalTexture, Input.TexCoord).rgb * 2.0f - 1.0f);
        m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
    }

    Color = vec4(m_Params.Albedo, 1.0);
}