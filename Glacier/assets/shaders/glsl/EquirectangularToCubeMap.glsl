#type compute
#version 450 core

const float PI = 3.1415926;

#if VULKAN
layout(set = 1, binding = 0) uniform sampler2D u_EquirectangularTexture;
#else
layout(binding = 0) uniform sampler2D u_EquirectangularTexture;
#endif

#if VULKAN
layout(set = 1, binding = 0, rgba16f) restrict writeonly uniform imageCube CubeMap;
#else
layout(binding = 0, rgba16f) restrict writeonly uniform imageCube CubeMap;
#endif

vec3 GetCubeMapTexCoord() {
    vec2 st = gl_GlobalInvocationID.xy / vec2(imageSize(CubeMap));
    vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

    vec3 ret;
    if(gl_GlobalInvocationID.z == 0)        ret = vec3(  1.0, uv.y,  -uv.x);
    else if(gl_GlobalInvocationID.z == 1)   ret = vec3( -1.0, uv.y,   uv.x);
    else if(gl_GlobalInvocationID.z == 2)   ret = vec3( uv.x,  1.0,  -uv.y);
    else if(gl_GlobalInvocationID.z == 3)   ret = vec3( uv.x, -1.0,   uv.y);
    else if(gl_GlobalInvocationID.z == 4)   ret = vec3( uv.x,  uv.y,   1.0);
    else if(gl_GlobalInvocationID.z == 5)   ret = vec3(-uv.x,  uv.y,  -1.0);
    return normalize(ret);
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() {
    vec3 cubeTexCoord = GetCubeMapTexCoord();

    float phi = atan(cubeTexCoord.z, cubeTexCoord.x);
    float theta = acos(cubeTexCoord.y);
    vec2 uv = vec2(phi / (2.0 * PI) + 0.5, theta / PI);

    vec4 color = texture(u_EquirectangularTexture, uv);
    imageStore(CubeMap, ivec3(gl_GlobalInvocationID), color);
}