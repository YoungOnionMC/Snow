struct VertexShaderInput {
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct PixelShaderInput {
    float4 PixelPosition : SV_POSITION;
    float4 Color : COLOR;
};

cbuffer Camera : register(b0) {
    float4x4 ViewProjection;
}

PixelShaderInput main(VertexShaderInput input) {
    PixelShaderInput psi;
    float4 position = float4(input.Position, 1.0f);
	psi.PixelPosition = mul(ViewProjection, position);
    psi.Color = input.Color;
    return psi;
}