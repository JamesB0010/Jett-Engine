#version 140

//Input to fragment shader
in vec2 uv;

//Texture
uniform sampler2D sampler;

//final fragment colour
out vec4 fragmentColour;

//shader body
void main()
{
	//Determine fragment colour from texture based on uv co-ords
	vec4 texColour = texture(sampler, uv);

	fragmentColour = texColour;
}