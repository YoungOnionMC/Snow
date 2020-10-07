#version 450

layout (location = 0) out vec4 Color;

layout (location = 0) in VertexOutput {
    vec2 TexCoord;
    float TexID;
    vec4 Color;
} psInput;

layout (binding = 0) uniform sampler2D u_Texture[32];

void main() {
    vec4 texColor = psInput.Color;
    int texID = int(psInput.TexID);
    switch(texID) {
    case 0: texColor = texture(u_Texture[0], psInput.TexCoord); break;
    case 1: texColor = texture(u_Texture[1], psInput.TexCoord); break;
    case 2: texColor = texture(u_Texture[2], psInput.TexCoord); break;
    case 3: texColor = texture(u_Texture[3], psInput.TexCoord); break;
    case 4: texColor = texture(u_Texture[4], psInput.TexCoord); break;
    case 5: texColor = texture(u_Texture[5], psInput.TexCoord); break;
    case 6: texColor = texture(u_Texture[6], psInput.TexCoord); break;
    case 7: texColor = texture(u_Texture[7], psInput.TexCoord); break;
    case 8: texColor = texture(u_Texture[8], psInput.TexCoord); break;
    case 9: texColor = texture(u_Texture[9], psInput.TexCoord); break;
    case 10: texColor = texture(u_Texture[10], psInput.TexCoord); break;
    case 11: texColor = texture(u_Texture[11], psInput.TexCoord); break;
    case 12: texColor = texture(u_Texture[12], psInput.TexCoord); break;
    case 13: texColor = texture(u_Texture[13], psInput.TexCoord); break;
    case 14: texColor = texture(u_Texture[14], psInput.TexCoord); break;
    case 15: texColor = texture(u_Texture[15], psInput.TexCoord); break;
    case 16: texColor = texture(u_Texture[16], psInput.TexCoord); break;
    case 17: texColor = texture(u_Texture[17], psInput.TexCoord); break;
    case 18: texColor = texture(u_Texture[18], psInput.TexCoord); break;
    case 19: texColor = texture(u_Texture[19], psInput.TexCoord); break;
    case 20: texColor = texture(u_Texture[20], psInput.TexCoord); break;
    case 21: texColor = texture(u_Texture[21], psInput.TexCoord); break;
    case 22: texColor = texture(u_Texture[22], psInput.TexCoord); break;
    case 23: texColor = texture(u_Texture[23], psInput.TexCoord); break;
    case 24: texColor = texture(u_Texture[24], psInput.TexCoord); break;
    case 25: texColor = texture(u_Texture[25], psInput.TexCoord); break;
    case 26: texColor = texture(u_Texture[26], psInput.TexCoord); break;
    case 27: texColor = texture(u_Texture[27], psInput.TexCoord); break;
    case 28: texColor = texture(u_Texture[28], psInput.TexCoord); break;
    case 29: texColor = texture(u_Texture[29], psInput.TexCoord); break;
    case 30: texColor = texture(u_Texture[30], psInput.TexCoord); break;
    case 31: texColor = texture(u_Texture[31], psInput.TexCoord); break;
    }

    Color = texColor;
}