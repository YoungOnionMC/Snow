
struct PixelShaderInput {
	float4 pixelPosition : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float TexID : ID;
	float4 Color : COLOR;
};

Texture2D textures[32] : register(t0);
SamplerState samplers[1] : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET {
    float4 texColor = input.Color;
    switch(input.TexID) {
        case 0: texColor *= textures[0].Sample(samplers[0], input.TexCoord); break;
        case 1: texColor *= textures[1].Sample(samplers[0], input.TexCoord); break;
        case 2: texColor *= textures[2].Sample(samplers[0], input.TexCoord); break;
        case 3: texColor *= textures[3].Sample(samplers[0], input.TexCoord); break;
        case 4: texColor *= textures[4].Sample(samplers[0], input.TexCoord); break;
        case 5: texColor *= textures[5].Sample(samplers[0], input.TexCoord); break;
        case 6: texColor *= textures[6].Sample(samplers[0], input.TexCoord); break;
        case 7: texColor *= textures[7].Sample(samplers[0], input.TexCoord); break;
        case 8: texColor *= textures[8].Sample(samplers[0], input.TexCoord); break;
        case 9: texColor *= textures[9].Sample(samplers[0], input.TexCoord); break;
        case 10: texColor *= textures[10].Sample(samplers[0], input.TexCoord); break;
        case 11: texColor *= textures[11].Sample(samplers[0], input.TexCoord); break;
        case 12: texColor *= textures[12].Sample(samplers[0], input.TexCoord); break;
        case 13: texColor *= textures[13].Sample(samplers[0], input.TexCoord); break;
        case 14: texColor *= textures[14].Sample(samplers[0], input.TexCoord); break;
        case 15: texColor *= textures[15].Sample(samplers[0], input.TexCoord); break;
        case 16: texColor *= textures[16].Sample(samplers[0], input.TexCoord); break;
        case 17: texColor *= textures[17].Sample(samplers[0], input.TexCoord); break;
        case 18: texColor *= textures[18].Sample(samplers[0], input.TexCoord); break;
        case 19: texColor *= textures[19].Sample(samplers[0], input.TexCoord); break;
        case 20: texColor *= textures[20].Sample(samplers[0], input.TexCoord); break;
        case 21: texColor *= textures[21].Sample(samplers[0], input.TexCoord); break;
        case 22: texColor *= textures[22].Sample(samplers[0], input.TexCoord); break;
        case 23: texColor *= textures[23].Sample(samplers[0], input.TexCoord); break;
        case 24: texColor *= textures[24].Sample(samplers[0], input.TexCoord); break;
        case 25: texColor *= textures[25].Sample(samplers[0], input.TexCoord); break;
        case 26: texColor *= textures[26].Sample(samplers[0], input.TexCoord); break;
        case 27: texColor *= textures[27].Sample(samplers[0], input.TexCoord); break;
        case 28: texColor *= textures[28].Sample(samplers[0], input.TexCoord); break;
        case 29: texColor *= textures[29].Sample(samplers[0], input.TexCoord); break;
        case 30: texColor *= textures[30].Sample(samplers[0], input.TexCoord); break;
        case 31: texColor *= textures[31].Sample(samplers[0], input.TexCoord); break;
    }
    return texColor;
};