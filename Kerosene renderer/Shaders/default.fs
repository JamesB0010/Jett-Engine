//some code adapted from unity hlsl shaders tutorial series https://www.youtube.com/watch?v=mL8U8tIiRRg&list=PLImQaTpSAdsCnJon-Eir92SZMl7tPBS4Z&index=2

#version 140
in vec2 uv;
in vec3 iNormal;
in vec3 WorldPos;
uniform sampler2D sampler;
in vec3 CamPosition;
out vec4 fragmentColour;

vec3 lightDir = normalize(vec3(0.8,1,-0.3));
float _Gloss = 0.7;
float ambientLight = 0.08;
void main()
{
    //diffuse lighting
    vec3 N = normalize(iNormal);
    vec3 L = lightDir;
    float lambert = clamp(dot(N,L), 0.0, 1.0);

    //specular lighting
    vec3 view = normalize(CamPosition - WorldPos);
    vec3 HalfwayVec = normalize(L + view);

    float specularLight = clamp(dot(HalfwayVec, N), 0.0,1.0) * (float(lambert > 0));
    float specularExponent = pow(2, _Gloss * 11) + 2;
    specularLight = pow(specularLight, specularExponent) * _Gloss;

    vec3 texColor = texture(sampler, uv).rgb;

    vec3 finalColor = texColor * lambert + specularLight + ambientLight;

    fragmentColour = vec4(finalColor, 1);
}