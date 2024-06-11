#version 140

in vec2 uv;

out vec4 fragmentColour;

uniform sampler2D sceneTexture;

uniform bool apply_greyscale;


float VignetteStrength = 0.8;

float VignetteZoom = -0.2;

void main()
{
    //Create a vector which points from the middle of the screen to the current pixel
    vec2 vecFromCenter = uv - 0.5;

    //find the magnitude of the vector
    float magFromCenter = length(vecFromCenter);

    //multiply mag by vignette strength 
    magFromCenter *= VignetteStrength;

    //add VignetteZoom
    magFromCenter += VignetteZoom;

    //Get colour for fragment from texture
    vec4 sceneColour = texture(sceneTexture, uv);

    //modify scene color by mag from center 
    vec4 finalColor = sceneColour * (1- magFromCenter);

    fragmentColour = mix(sceneColour, finalColor, apply_greyscale);
}