#version 140
in vec2 uv;
uniform samplerCube skySampler;
in vec3 CamPosition;
out vec4 fragmentColour;

in vec3 viewVector;

in vec3 reflectedVector;

void main()
{
    vec3 V = normalize(viewVector);
    vec3 R = normalize(reflectedVector);
    vec3 reflectionColor = texture(skySampler, R).rgb;
    fragmentColour = vec4(reflectionColor, 1);
}