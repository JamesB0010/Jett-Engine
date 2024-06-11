#pragma once
#include <Jett Engine/GameComponent.h>
#include <glm/glm.hpp>
#include <Jett Engine/GameObject.h>
#include <Jett Engine/GlobalLerpProcessor.h>
#include <Jett Engine/Vector3LerpPackage.h>

namespace GE {
	class GameEngine;
}
class SharkManager: public GameComponent
{
	const int numberOfSharksRightOfBoat = 4;
	const int numberOfSharksLeftOfBoat = 4;

	size_t totalNumberOfSharks;

	GameObject** sharks = nullptr;

	float timeSinceLastSpawn = 0;
	float nextSpawnTime = 15.0f;

public:
	SharkManager();

	~SharkManager();

	void AddSharksToEngine(GE::GameEngine* gameEngine);
	void AddSharkToEngineAtPosition(int sharkNumber, GE::GameEngine* engine, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale = glm::vec3(8,8,8));
	void Start() override;
	void Update() override;
	void PuppetShark();
};

