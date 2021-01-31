
struct PixelShaderInput {
    float4 pixelPosition : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

cbuffer CompositeBuffer : register(b4) {
    float Exposure;
    int Samples;
    bool BloomEnable;
    float BloomThreshold;
};

Texture2DMS<float4> u_MultiSampledTexture : register(t0);

float4 SampleTexture(Texture2D tex, SamplerState sampleState , float2 texCoord) {
    return tex.Sample(sampleState, texCoord);
}

float4 MultiSampleTexture(Texture2DMS<float4> tex, float2 texCoord) {
    vector<uint, 2> size; 
    uint samples;
    tex.GetDimensions(size.x, size.y, samples);
    vector<uint, 2> tc = (texCoord * size);
    float4 result = float4(0.0, 0.0, 0.0, 0.0);
    for(int i = 0; i < Samples; i++) {
        result += tex.Load(tc, i);
    }
    result /= float(Samples);
    return result;

}

static const float uFar = 1.0;
static const float gamma = 2.2;
static const float pureWhite = 1.0;

float4 main(in PixelShaderInput psi) : SV_TARGET {
    float4 msColor = MultiSampleTexture(u_MultiSampledTexture, psi.TexCoord);

    float3 color;
    color = msColor.rgb;

    if(BloomEnable) {
        float3 bloomColor = MultiSampleTexture(u_MultiSampledTexture, psi.TexCoord).rgb;
        color += bloomColor;
    }

    color *= Exposure;

    float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
    float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

    float3 mappedColor = (mappedLuminance / luminance) * color;

    float3 oneOverGamma = float3(1.0/gamma, 1.0/gamma, 1.0/gamma);

    float3 balancedColor = float3(0.0, 0.0, 0.0);
    balancedColor = pow(abs(mappedColor), oneOverGamma).rgb;
        
    return float4(balancedColor.rgb, 1.0);
}