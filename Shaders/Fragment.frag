#version 450

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 fragCoord;
layout(location = 2) in float iTime;
layout(location = 3) in vec4 vNormal;

layout(set = 0, binding = 1) uniform  SceneData{   
    vec4 sky;
    vec4 ambientColor;
    vec4 lightColor;
    vec4 astroMiddleColor;
    vec4 astroOutterColor;
    vec4 waveColor;
    vec4 mousePosition;
} sceneData;

layout(location = 0) out vec4 fragColor;

//Original idea taken from s23b in shadertoy.

vec4 makeWave(vec2 uv, vec2 o, vec2 s, vec2 v, vec4 c1, vec4 c2, vec4 ch)
{
    uv *= s;
    uv += o;
    float f = sin(uv.x+iTime * v.x) - uv.y - sin(iTime * v.y);
    
    vec4 c = vec4(0.);

    if (f > 5.)
    {
        c = c2;
    }
    else if (f > .0)
    {
        c = mix(c1, c2, f / 10.);
    }
    else if (f > -.2*s.y / 10.)
    {
        c = mix(vec4(0.0), ch, mod(uv.x, 2.0));
    }

	return c;
}

vec4 blend(vec4 c1, vec4 c2)
{
    return vec4(mix(c1.rgb, c2.rgb, c2.a), max(c1.a, c2.a)); 
}

float hash(float n)
{
    return fract(sin(n) * 123.456);
}

vec4 sun(vec2 uv, vec4 c1, vec4 c2, vec4 ch)
{
    vec4 c = vec4(0.);
    float pos = mix(1.0, -1.0, sceneData.mousePosition.y) + (pow(smoothstep(0.65, 1.0,sceneData.mousePosition.y), 2.));

    float f = length(vec2(uv.x, uv.y - pos)) * 2.0 -  sin(atan(uv.x, uv.y) / cos(45.0) * 10. + iTime * 10.)/ 100.;
    if (f < 1.05 && f >= 1.)
    {
        c = ch;
    }
    else if (f < 1.)
    {
        c = mix(c1, c2, f);
    }
	return c;
}

void main( )
{
	vec2 uv = fragCoord.xy / 0.25;
    
    vec4 bg = mix(sceneData.lightColor, sceneData.sky, smoothstep(0.3, 1.0, clamp(uv.y, 0., 1.)));
    
    bg = blend (bg, sun(uv, sceneData.astroMiddleColor, sceneData.astroOutterColor, vec4(1.)));
    
    for (float i = .0; i <= 1.0; i += mix(.1, .03, sceneData.mousePosition.y))
    {
    	bg = blend(bg, makeWave(
            uv,
            vec2(hash(i) * 10., i * 10.),
            vec2(10. * (1.25 - i), 5. / i),
            vec2( i*7. + hash(i), i*3.),
            vec4(atan(sceneData.waveColor.rgb), 1.0), sceneData.waveColor, vec4(1.)));
    }
    
    fragColor = bg;
}