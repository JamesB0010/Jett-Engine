#version 140
#extension GL_ARB_explicit_attrib_location : enable

#define eulersNumber 2.718281

layout(location = 0) in vec4 position;

layout(location = 1) in vec2 Uv;

out vec2 UvCoord;

out vec3 Normal;

uniform mat4 viewMat;

uniform mat4 projMat;

uniform mat4 transformMat;

uniform uint TimePassed;

uniform vec2 WaveDirections[1];

float globalDesiredSpeed = 0.05;
float globalDesiredSpeedIncreaseRate = 1.05;

float globalWavelength = 50;
float globalWavelengthDecreaseRate = 0.9;

float globalAmplitude = 2;
float globalAmplitudeDecayRate = 0.7;

struct Wave{
	vec2 direction;
	vec3 origin;
	float amplitude;
	float wavelength;
	float frequency;
	float speed;
};

Wave CreateWave(vec2 direction, vec3 origin, float amplitude, float wavelength, float desiredSpeed){
	Wave wave;
	wave.direction = direction;
	wave.origin = origin;
	wave.amplitude = amplitude;
	wave.wavelength = wavelength;
	wave.frequency = 2 / wave.wavelength;
	wave.speed = desiredSpeed * wave.frequency;
	
	return wave;
};

float GetWaveCoord(Wave w, vec2 direction){
	return w.origin.x * direction.x + w.origin.z * direction.y;
};

float GetTime(Wave w){
	return TimePassed * w.speed;
};

float GetWaveYOffset(Wave w){
	float xz = GetWaveCoord(w, w.direction);
	float t = GetTime(w);
	return w.amplitude * pow(eulersNumber, sin(xz * w.frequency + t) - 1);
};

float GetWaveDerivative(Wave w){
	float xz = GetWaveCoord(w, w.direction);
	float t = GetTime(w);
	return w.frequency * w.amplitude * pow(eulersNumber, sin(xz * w.frequency + t) - 1) * cos(xz * w.frequency + t);

};

float GetWavePartialDerivative(float direction, float derivative){
	return direction * derivative;
};

const int numberOfWaves = 32;
float SumOfSines(Wave waves[numberOfWaves]){
	float yOffset = 0;
	float derivativeX = 0;
	float derivativeZ = 0;
	for(int i = 0; i < numberOfWaves; ++i){
		yOffset += GetWaveYOffset(waves[i]);
	
		float derivative = GetWaveDerivative(waves[i]);
		derivativeX += GetWavePartialDerivative(waves[i].direction.x, derivative);
		derivativeZ += GetWavePartialDerivative(waves[i].direction.y, derivative);
	};
	
	vec3 tangent = vec3(1.0,0.0,derivativeX);
	vec3 biNormal = vec3(0.0,1.0,derivativeZ);
	Normal = cross(tangent, biNormal);
	
	return yOffset;
};


void main(){
	Wave waves[numberOfWaves];
	for(int i = 0; i < numberOfWaves; ++i){
		waves[i] = CreateWave(WaveDirections[0], vec3(position.x, position.y, position.z), globalAmplitude, globalWavelength, globalDesiredSpeed);
		globalDesiredSpeed *= globalDesiredSpeedIncreaseRate;
		globalAmplitude *= globalAmplitudeDecayRate;
		globalWavelength *= globalWavelengthDecreaseRate;
	};
	
	
	//waves[1] = CreateWave(normalize(vec2(-1, 0.4)), vec3(position.x, position.y, position.z), 1.5, 30.0, 0.0066);
	//waves[2] = CreateWave(normalize(vec2(-1, -0.7)), vec3(position.x, position.y, position.z), 0.5, 15, 0.0075);

	float yOffset = SumOfSines(waves);

	vec4 offsettedPos = position;
	offsettedPos.y += yOffset;

	gl_Position = projMat * viewMat * transformMat * offsettedPos;
	UvCoord = Uv;
}