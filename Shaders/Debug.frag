#version 450

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 fragCoord;
layout(location = 2) in float iTime;
layout(location = 3) in vec4 vNormal;

layout(location = 0) out vec4 fragColor;


void main( )
{
    fragColor = vec4(1.);
}