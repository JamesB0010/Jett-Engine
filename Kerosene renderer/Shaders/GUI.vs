#version 140

//Inputs to vertex shader
in vec3 vertexPos3D;
in vec2 vUV;

//output
out vec2 uv;

//Uniforms
uniform mat4 projection;

//shader main code
void main()
{
	//convert input vertex to 4D
	vec4 v = projection * vec4(vertexPos3D.xyz, 1.0);
	
	//pass transformed vertex to next stage
	gl_Position = v;

	//as well as uv. dont transform uv
	uv = vUV;
}