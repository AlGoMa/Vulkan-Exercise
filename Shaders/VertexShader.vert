#version 450

layout(binding = 0) uniform UniformData {
    mat4 view;
    mat4 model;
    mat4 proj;
    float time;
} ubo;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec4 color;
layout(location = 1) out vec2 fragCoord;
layout(location = 2) out float iTime;
layout(location = 3) out vec4 vNormal;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition.xyz, 1.0);
    color = inColor;
    fragCoord = inTexCoord;
    vNormal = inNormal;
    iTime = ubo.time;
}
