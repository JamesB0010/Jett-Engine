#include "ParticleEmmitter.h"
#include "Include/Jett Engine/GameEngine.h"

ParticleEmmitter::ParticleEmmitter(int particleCount) {
	using namespace GE;

	std::random_device rd;
	std::mt19937 gen(rd());

	this->particleCount = particleCount;
	std::vector<glm::vec3> vertexPositions = {
		glm::vec3(-1, 1, 0),
		glm::vec3(-1,-1,0),
		glm::vec3(1,-1,0),

		glm::vec3(1,1,0),
		glm::vec3(1,-1,0),
		glm::vec3(-1,1,0)
	};

	for (int i = 0; i < this->particleCount; ++i) {
		std::uniform_real_distribution<> distr(-5, 5);
		this->particlePositions.push_back(glm::vec3(distr(gen), distr(gen), distr(gen)));
		distr = std::uniform_real_distribution<>(0, 10000);
		timeOffsets.push_back(distr(gen));

		distr = std::uniform_real_distribution<>(0, 360);
		this->particleRotations.push_back(glm::vec3(distr(gen), distr(gen), distr(gen)));

		distr = std::uniform_real_distribution<>(0.5, 2);
		double randNumber = distr(gen);
		this->particleScales.push_back(glm::vec3(randNumber, randNumber, randNumber));
	}

	particle = std::unique_ptr<VisualMesh>(VisualMesh::CreateHeapVisMeshFromVertexPosVectorAndTexture(vertexPositions, "../../Assets/Images/SmokeCard2.png", GameEngine::camera, loadShaderSourceCode("../../../Kerosene renderer/Shaders/Particle.vs"), loadShaderSourceCode("../../../Kerosene renderer/Shaders/Particle.fs")));

	this->timeUniformId = glGetUniformLocation(this->particle.get()->getProgramId(), "TimePassed");

	this->timeOffsetUniformId = glGetUniformLocation(this->particle.get()->getProgramId(), "TimeOffset");
}

void ParticleEmmitter::Update()
{
}

void ParticleEmmitter::Draw()
{
	for (int i = 0; i < this->particleCount; ++i) {
		this->particle.get()->setPos(this->origin + this->particlePositions[i]);
		this->particle.get()->setRotation(this->particleRotations[i]);
		this->particle.get()->setScale(this->particleScales[i]);
		glUseProgram(this->particle.get()->getProgramId());
		glUniform1ui(this->timeUniformId, Time::timeSinceStart);
		glUniform1f(this->timeOffsetUniformId, this->timeOffsets[i]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(0);
		this->particle.get()->Draw();
		glDisable(GL_BLEND);
	}
}
