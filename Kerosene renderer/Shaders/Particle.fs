#version 140

in vec2 uv;

in uint timePassed;

uniform sampler2D sampler;

uniform uint TimePassed;

uniform float TimeOffset;

out vec4 fragmentColour;

void main()
{
fragmentColour = texture(sampler, uv).rgba;
fragmentColour.a = sin((TimePassed + TimeOffset) / 2200.0) + 1;

//make black background transparrent
if((fragmentColour.r + fragmentColour.g + fragmentColour.b) < 0.1){
    discard;
}
}