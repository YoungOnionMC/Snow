#version 450

layout (location = 0) out vec4 Color;

layout (location = 0) in VertexOutput {
    vec2 TexCoord;
    float TexID;
    vec4 Color;
} psInput;

layout (location = 1) in LightOutput {
    vec3 LightColor;
} lightInput;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = psInput.Color;
    switch(int(psInput.TexID)) {
    case 0: texColor *= texture(u_Textures[0], psInput.TexCoord); break;
    case 1: texColor *= texture(u_Textures[1], psInput.TexCoord); break;
    case 2: texColor *= texture(u_Textures[2], psInput.TexCoord); break;
    case 3: texColor *= texture(u_Textures[3], psInput.TexCoord); break;
    case 4: texColor *= texture(u_Textures[4], psInput.TexCoord); break;
    case 5: texColor *= texture(u_Textures[5], psInput.TexCoord); break;
    case 6: texColor *= texture(u_Textures[6], psInput.TexCoord); break;
    case 7: texColor *= texture(u_Textures[7], psInput.TexCoord); break;
    case 8: texColor *= texture(u_Textures[8], psInput.TexCoord); break;
    case 9: texColor *= texture(u_Textures[9], psInput.TexCoord); break;
    case 10: texColor *= texture(u_Textures[10], psInput.TexCoord); break;
    case 11: texColor *= texture(u_Textures[11], psInput.TexCoord); break;
    case 12: texColor *= texture(u_Textures[12], psInput.TexCoord); break;
    case 13: texColor *= texture(u_Textures[13], psInput.TexCoord); break;
    case 14: texColor *= texture(u_Textures[14], psInput.TexCoord); break;
    case 15: texColor *= texture(u_Textures[15], psInput.TexCoord); break;
    case 16: texColor *= texture(u_Textures[16], psInput.TexCoord); break;
    case 17: texColor *= texture(u_Textures[17], psInput.TexCoord); break;
    case 18: texColor *= texture(u_Textures[18], psInput.TexCoord); break;
    case 19: texColor *= texture(u_Textures[19], psInput.TexCoord); break;
    case 20: texColor *= texture(u_Textures[20], psInput.TexCoord); break;
    case 21: texColor *= texture(u_Textures[21], psInput.TexCoord); break;
    case 22: texColor *= texture(u_Textures[22], psInput.TexCoord); break;
    case 23: texColor *= texture(u_Textures[23], psInput.TexCoord); break;
    case 24: texColor *= texture(u_Textures[24], psInput.TexCoord); break;
    case 25: texColor *= texture(u_Textures[25], psInput.TexCoord); break;
    case 26: texColor *= texture(u_Textures[26], psInput.TexCoord); break;
    case 27: texColor *= texture(u_Textures[27], psInput.TexCoord); break;
    case 28: texColor *= texture(u_Textures[28], psInput.TexCoord); break;
    case 29: texColor *= texture(u_Textures[29], psInput.TexCoord); break;
    case 30: texColor *= texture(u_Textures[30], psInput.TexCoord); break;
    case 31: texColor *= texture(u_Textures[31], psInput.TexCoord); break;
    }

    vec4 c = vec4(lightInput.LightColor, 1.0);
    Color = c;
    //Color = texColor;
}