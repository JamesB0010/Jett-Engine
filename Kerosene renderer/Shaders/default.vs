#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 vertexPos3D;

in vec2 vUV;

layout(location = 2) in vec3 VNormal;

out vec2 uv;

out vec3 iNormal;

out vec3 WorldPos;

out vec3 CamPosition;

uniform mat4 viewMat;

uniform mat4 projMat;

uniform mat4 transformMat;

uniform vec3 CameraPosition;

void main(){
gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D.x, vertexPos3D.y, vertexPos3D.z, 1);
uv = vUV;
iNormal = (transformMat * vec4(VNormal.xyz, 0.0)).xyz;
WorldPos = (transformMat * vec4(vertexPos3D,1)).xyz;
CamPosition = (inverse(viewMat) * vec4(CameraPosition, 1.0)).xyz;
}