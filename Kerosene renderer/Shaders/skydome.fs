#version 140
in vec2 uv;
uniform sampler2D sampler;
out vec4 fragmentColour;
void main()
{
    vec3 texColor = texture(sampler, uv).rgb;

    fragmentColour = vec4(texColor, 1);
}