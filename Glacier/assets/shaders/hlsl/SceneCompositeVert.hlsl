
struct VertexShaderInput {
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct PixelShaderInput {
    float4 pixelPosition : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

PixelShaderInput main(in VertexShaderInput input) {
    PixelShaderInput psi;
    psi.pixelPosition = float4(input.Position.xy, 0.0, 1.0);
    psi.TexCoord = input.TexCoord;
    return psi;
}