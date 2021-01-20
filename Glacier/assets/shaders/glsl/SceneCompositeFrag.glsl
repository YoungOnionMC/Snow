#version 450 core

layout(location = 0) out vec4 Color;

layout(location = 0) in PixelInput {
	vec2 TexCoord;
} psInput;

layout(binding = 0) uniform sampler2DMS u_Texture;

layout(std140, binding = 4) uniform CompositeBuffer {
	float Exposure;
	int Samples;
	bool BloomEnable;
	float BloomThreshold;
} compositeBuffer;

const float uFar = 1.0;
const float gamma = 2.2;
const float pureWhite = 1.0;

vec4 SampleTexture(sampler2D tex, vec2 texCoord) {
	return texture(tex, texCoord);
}

vec4 MultiSampleTexture(sampler2DMS tex, vec2 tc) {
	ivec2 texSize = textureSize(tex);
	ivec2 texCoord = ivec2(tc * texSize);
	vec4 result = vec4(0.0);
	for(int i = 0; i < compositeBuffer.Samples; i++) {
		result += texelFetch(tex, texCoord, i);
	}

	result /= float(compositeBuffer.Samples);
	return result;
}

void main() {
	

	vec4 msColor = MultiSampleTexture(u_Texture, psInput.TexCoord);

	vec3 color = msColor.rgb;

	if(compositeBuffer.BloomEnable) {
		vec3 bloomColor = MultiSampleTexture(u_Texture, psInput.TexCoord).rgb;
		color += bloomColor;
	}

	color *= compositeBuffer.Exposure;

	float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

	vec3 mappedColor = (mappedLuminance / luminance) * color;

	Color = vec4(pow(mappedColor, vec3(1.0/gamma)), 1.0);
}