#version 330 core
out vec4 FragColor;

struct Fog {
    float maxDist;
    float minDist;
    vec3 color;
};

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Fog fog;
uniform vec3 viewPos;

vec4 CalcFog(Fog fogSettings, vec4 color);

void main()
{
    FragColor = CalcFog(fog, vec4(1.0));
}

vec4 CalcFog(Fog fogSettings, vec4 color)
{
    float distance = length(fs_in.FragPos - viewPos);

    float fogFactor = clamp((distance - fogSettings.minDist) / (fogSettings.maxDist - fogSettings.minDist), 0.0, 1.0);

    vec3 foggedColor = mix(color.rgb, fogSettings.color, fogFactor);

    return vec4(foggedColor, color.a);
}