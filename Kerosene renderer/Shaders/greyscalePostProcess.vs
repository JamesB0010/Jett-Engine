#version 140

in vec3 vertexPos;

in vec2 vUV; 

out vec2 uv;

void main(){

	gl_Position = vec4(vertexPos, 1);
    uv = vUV;
}