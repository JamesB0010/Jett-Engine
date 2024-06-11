#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
in vec3 Normal;

in vec2 UvCoord;

uniform sampler2D U_Sampler;

uniform vec3 ViewDir;

vec3 sunDirection = vec3(0, 1, 0);

vec4 desiredBaseColor = vec4(0,0.1,0.5,1);

float specularSize = 1;

float FindLightingIntensity(vec3 normalizedNormal){
    float lighting = dot(normalizedNormal, sunDirection);
    return max(lighting * 20, 0.3);
};

vec4 FindSpecularColor(vec3 normalizedNormal){
    float maxSpec = 0.3;
    vec3 reflectDir = reflect(-ViewDir, normalizedNormal);
    float specularIntensity = max(dot(reflectDir, sunDirection), 0.0) * specularSize;
    specularIntensity = min(specularIntensity, maxSpec);
    

    return vec4(specularIntensity);
};


void main()
{
    desiredBaseColor = texture(U_Sampler, UvCoord);
    vec3 normalizedNormal = normalize(Normal);

    //color = desiredBaseColor * FindLightingIntensity(normalizedNormal) + FindSpecularColor(normalizedNormal);
    color = desiredBaseColor + FindSpecularColor(normalizedNormal);
    color.w = 1.0;

    color = clamp(color, 0, 1);
    color = desiredBaseColor;
}