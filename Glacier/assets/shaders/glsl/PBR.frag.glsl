#version 450 core

const float PI = 3.1415926;
const float Epsilon = 0.00001;

const vec3 FDielectric = vec3(0.04);

struct Light {
	vec3 Direction;
	vec3 Radiance;
	float Multiplier;
};

#if VULKAN
layout(std140, set = 0, binding = 2) uniform Environment
#else
layout(std140, binding = 2) uniform Environment
#endif
{
	Light lights;
	vec3 u_CameraPosition;

	float EnvMapRotation;
} environment;

layout(location = 0) in PixelInput {
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoords;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;
} psInput;

layout(location = 0) out vec4 Color;

#if VULKAN
layout (set = 1, binding = 0) uniform sampler2D u_AlbedoTexture;
#else
layout(binding = 0) uniform sampler2D u_AlbedoTexture;
#endif

#if VULKAN
layout (set = 1, binding = 1) uniform sampler2D u_NormalTexture;
#else
layout(binding = 1) uniform sampler2D u_NormalTexture;
#endif

#if VULKAN
layout (set = 1, binding = 2) uniform sampler2D u_MetalnessTexture;
#else
layout(binding = 2) uniform sampler2D u_MetalnessTexture;
#endif

#if VULKAN
layout (set = 1, binding = 3) uniform sampler2D u_RoughnessTexture;
#else
layout(binding = 3) uniform sampler2D u_RoughnessTexture;
#endif

// Environment maps
#if VULKAN
layout (set = 1, binding = 4) uniform samplerCube u_EnvRadianceTexture;
#else
layout(binding = 4) uniform samplerCube u_EnvRadianceTexture;
#endif

#if VULKAN
layout (set = 1, binding = 5) uniform samplerCube u_EnvIrradianceTexture;
#else
layout(binding = 5) uniform samplerCube u_EnvIrradianceTexture;
#endif

// BRDF LUT

#if VULKAN
layout (set = 1, binding = 6) uniform sampler2D u_BRDFLUTTexture;
#else
layout(binding = 6) uniform sampler2D u_BRDFLUTTexture;
#endif

#if VULKAN
layout (std140, set = 0, binding = 3) uniform Material
#else
layout (std140, binding = 3) uniform Material
#endif
{
	vec3 AlbedoColor;
	float Metalness;
	float Roughness;

	

	float RadiancePrefilter;
	float AlbedoTexToggle;
	float NormalTexToggle;
	float MetalnessTexToggle;
	float RoughnessTexToggle;
} MaterialUniforms;

struct PBRParams {
	vec3 Albedo;
	float Roughness;
	float Metalness;

	vec3 Normal;
	vec3 View;
	float NoV;
} m_Params;


float GGXDiffuse(float cosLh, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;

	float denom = (cosLh * cosLh) * (a2 - 1.0) + 1.0;
	return a2 / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float K) {
	return cosTheta / (cosTheta * (1.0 - K) + K);
}

float gaSchlickGGX(float cosLi, float NoV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return gaSchlickG1(cosLi, k) * gaSchlickG1(NoV, k);
}

float GeometrySchlickGGX(float NoV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NoV;
	float denom = NoV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NoV = max(dot(N, V), 0.0);
	float NoL =	max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NoV, roughness);
	float ggx1 = GeometrySchlickGGX(NoL, roughness);

	return ggx1 * ggx2;
}

float FDLambert() {
	return 1.0 / PI;
}

// Schlick's approx on the Fresnel factor
vec3 FresnelSchlick(vec3 f0, float cosTheta) {
	return f0 + (vec3(1.0) - f0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 FresnelSchlickRoughness(vec3 f0, float cosTheta, float roughness) {
	return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - cosTheta, 5.0);
}

float RadicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;

}

vec2 Hammersley(uint i, uint N) {
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 RotateVectorAboutY(float angle, vec3 vec) {
	angle = radians(angle);
	mat3x3 rotationMatrix = {vec3(cos(angle), 0.0, sin(angle)),
							vec3(0.0 ,1.0, 0.0),
							vec3(-sin(angle), 0.0, cos(angle))};
	return rotationMatrix * vec;
}

vec3 Lighting(vec3 F0) {
	vec3 result = vec3(0.0);

	vec3 Li = -(environment.lights.Direction);
	//vec3 LRadiance = environment.lights.Radiance;
	vec3 LRadiance = environment.lights.Radiance * environment.lights.Multiplier;
	vec3 Lh = normalize(Li + m_Params.View);

	float cosLi = max(0.0, dot(m_Params.Normal, Li));
	float cosLh = max(0.0, dot(m_Params.Normal, Lh));

	vec3 F = FresnelSchlick(F0, max(0.0, dot(Lh, m_Params.View)));
	float D = GGXDiffuse(cosLh, m_Params.Roughness);
	float G = gaSchlickGGX(cosLi, m_Params.NoV, m_Params.Roughness);

	vec3 Kd = mix(vec3(1.0) - F, vec3(0.0), m_Params.Metalness);
	vec3 diffuseBRDF = Kd * m_Params.Albedo;

	vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * m_Params.NoV);

	result = (diffuseBRDF + specularBRDF) * LRadiance * cosLi;
	return result;
}

vec3 IBL(vec3 F0, vec3 Lr) {
	vec3 irradiance = texture(u_EnvIrradianceTexture, m_Params.Normal).rgb;
	vec3 F = FresnelSchlickRoughness(F0, m_Params.NoV, m_Params.Roughness);
	vec3 Kd = mix(vec3(1.0) - F, vec3(0.0), m_Params.Metalness);
	vec3 diffuseIBL = Kd *  m_Params.Albedo * irradiance;

	int envRadianceTexLevels = textureQueryLevels(u_EnvRadianceTexture);
	float NV = clamp(m_Params.NoV, 0.0, 1.0);
	vec3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	vec3 specularIrradiance = textureLod(u_EnvRadianceTexture, RotateVectorAboutY(environment.EnvMapRotation, Lr), (m_Params.Roughness) * envRadianceTexLevels).rgb;

	vec2 specularBRDF = texture(u_BRDFLUTTexture, vec2(m_Params.NoV, m_Params.Roughness)).rg;
	vec3 specularIBL = specularIrradiance * (F * specularBRDF.x + specularBRDF.y);

	return diffuseIBL + specularIBL;

	//return texture(u_BRDFLUTTexture, m_Params.Normal.xz).rgb;
	//return vec3(1.0, 0.0, 0.0);
}

void main() {
	
	m_Params.Albedo = MaterialUniforms.AlbedoTexToggle > 0.5 ? texture(u_AlbedoTexture, psInput.TexCoords).rgb : MaterialUniforms.AlbedoColor;
	m_Params.Metalness = MaterialUniforms.MetalnessTexToggle > 0.5 ? texture(u_MetalnessTexture, psInput.TexCoords).r : MaterialUniforms.Metalness;
	m_Params.Roughness = MaterialUniforms.RoughnessTexToggle > 0.5 ? texture(u_RoughnessTexture, psInput.TexCoords).r : MaterialUniforms.Roughness;
	m_Params.Roughness = max(m_Params.Roughness, 0.05);

	m_Params.Normal = normalize(psInput.Normal);
	if(MaterialUniforms.NormalTexToggle > 0.5) {
		m_Params.Normal = normalize(2.0 * texture(u_NormalTexture, psInput.TexCoords).rgb - 1.0);
		m_Params.Normal = normalize(psInput.WorldNormals * m_Params.Normal);
	}
	m_Params.View = normalize(environment.u_CameraPosition - psInput.WorldPosition);
	m_Params.NoV = max(dot(m_Params.Normal, m_Params.View), 0.0);

	vec3 Lr = 2.0 * m_Params.NoV * m_Params.Normal - m_Params.View;

	vec3 F0 = mix(FDielectric, m_Params.Albedo, m_Params.Metalness);

	vec3 lightContribution = Lighting(F0);
	vec3 iblContribution = IBL(F0, Lr);

	Color = vec4(lightContribution + iblContribution, 1.0f);

	//Color = vec4((m_Params.View), 1.0);
}