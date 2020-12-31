

struct VertexShaderInput {
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
	float TexID : ID;
	float4 Color : COLOR;
};

struct PixelShaderInput {
	float4 pixelPosition : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float TexID : ID;
	float4 Color : COLOR;
};

cbuffer Camera : register(b0) {
	float4x4 ViewProjection;
};

PixelShaderInput main(in VertexShaderInput input) {
	PixelShaderInput psi;
	float4 position = float4(input.Position, 1.0f);
	psi.pixelPosition = mul(ViewProjection, position);
	psi.TexCoord = input.TexCoord;
	psi.TexID = input.TexID;
	psi.Color = input.Color;
	return psi;
}
