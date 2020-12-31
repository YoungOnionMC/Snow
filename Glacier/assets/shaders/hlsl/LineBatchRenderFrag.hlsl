struct PixelShaderInput {
    float4 PixelPosition : SV_POSITION;
    float4 Color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET {
    return input.Color;
}