#version 140

in vec3 vertexPos3D;

in vec2 vUV; 

out vec2 uv;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 transformMat;

uniform uint TimePassed;

uniform float TimeOffset;

mat4 particleOffset(){
	const float maxDistanceFromStart = 10.0;
    const float speed = 1000.0;
    float yOffset = mod((TimePassed + TimeOffset) / speed, maxDistanceFromStart);

    mat4 returnVal = mat4(1.0);
    returnVal[3][1] = yOffset;
    return returnVal;
}

void main(){

	gl_Position = projMat * viewMat * transformMat * particleOffset() * vec4(vertexPos3D, 1.0);
    uv = vUV;
}