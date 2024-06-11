#pragma once
#include "Include/Jett Engine/GameComponent.h"
#include <vector>
#include <memory>
#include <Kerosene Renderer/ShaderUtils.h>
#include "Include/Jett Engine/VisualMesh.h"
#include <random>

namespace GE {
	class GameEngine;
}
class ParticleEmmitter : public DrawableGameComponent
{
	std::unique_ptr<VisualMesh> particle;

	GLuint timeUniformId;

	std::vector<float> timeOffsets;

	GLuint timeOffsetUniformId;

	int particleCount;


	GLint particlePosUniform, particleRotUniform, particleScaleUniform;

	std::vector<glm::vec3> particlePositions;

	std::vector<glm::vec3> particleRotations;

	std::vector<glm::vec3> particleScales;

	glm::vec3 origin = glm::vec3(0, 0, 0);

public:
	ParticleEmmitter(int particleCount);

	void setOrigin(glm::vec3 newOrigin) {
		this->origin = newOrigin;
	}

private:

	void Update() override;


	void Draw() override;

	friend class GE::GameEngine;
};

