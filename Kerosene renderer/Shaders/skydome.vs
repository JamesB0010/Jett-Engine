#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 vertexPos3D;

in vec2 vUV;

out vec2 uv;

uniform mat4 viewMat;

uniform mat4 projMat;

uniform mat4 transformMat;


void main(){
gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D.x, vertexPos3D.y, vertexPos3D.z, 1);
uv = vUV;
}