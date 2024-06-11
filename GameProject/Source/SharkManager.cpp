#include "../Include/SharkManager.h"
#include <Jett Engine/GameEngine.h>

SharkManager::SharkManager()
{
	this->totalNumberOfSharks = static_cast<size_t>(this->numberOfSharksRightOfBoat) + static_cast<size_t>(this->numberOfSharksLeftOfBoat);
	this->sharks = new GameObject*[totalNumberOfSharks];
}

SharkManager::~SharkManager()
{
	delete[] this->sharks;
}

void SharkManager::AddSharksToEngine(GE::GameEngine* gameEngine)
{
	const glm::vec3* sharkPositions = new glm::vec3[this->totalNumberOfSharks]{
		glm::vec3(-70, -25, 25),
		glm::vec3(-70, -25, 41),
		glm::vec3(-70, -25, 56),
		glm::vec3(-70, -25, 69),

		glm::vec3(30, -25, 25),
		glm::vec3(30, -25, 41),
		glm::vec3(30, -25, 56),
		glm::vec3(30, -25, 69)

	};
	glm::vec3 sharkRotation = glm::vec3(-110, -30, 90);

	//spawn sharks on the right of the boat
	for (int i = 0; i < this->numberOfSharksRightOfBoat; ++i) {
		this->AddSharkToEngineAtPosition(i, gameEngine, sharkPositions[i], sharkRotation);
	}

	sharkRotation = glm::vec3(-110, 30, -90);

	for (int i = 0; i < numberOfSharksLeftOfBoat; ++i) {
		this->AddSharkToEngineAtPosition(this->numberOfSharksRightOfBoat + i, gameEngine, sharkPositions[numberOfSharksRightOfBoat + i], sharkRotation);
	}

	delete[] sharkPositions;
}

void SharkManager::AddSharkToEngineAtPosition(int sharkNumber, GE::GameEngine* engine, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	GameObject* gameObject = new GameObject("Shark: " + sharkNumber);

	VisualMesh* visualMeshComponent = VisualMesh::CreateHeapVisualMeshFromDaeWithIncludedTextureReference("../../Assets/Assets/Shark/Shark.dae", engine->camera, position);
	visualMeshComponent->setRotation(rotation);
	visualMeshComponent->setScale(scale);

	gameObject->AddComponent<VisualMesh>(visualMeshComponent);

	engine->AddGameObject(gameObject);

	this->sharks[sharkNumber] = gameObject;
}

void SharkManager::Start()
{
	this->PuppetShark();
}

void SharkManager::Update()
{
	this->timeSinceLastSpawn += Time::deltaTime / 1000;
	if (this->timeSinceLastSpawn >= this->nextSpawnTime) {
		this->PuppetShark();
		this->timeSinceLastSpawn = 0;
		this->nextSpawnTime = this->nextSpawnTime > 0.5 ? this->nextSpawnTime - 3 : 0.5;
	}
}

void SharkManager::PuppetShark()
{
	int sharkToMove = std::rand() % this->totalNumberOfSharks;
	int attempts = 1000;
	while (this->sharks[sharkToMove]->FindComponent<VisualMesh>()->getPosition().y != -25.0f == true) {
		if (attempts <= 0)
			return;
		sharkToMove = std::rand() % this->totalNumberOfSharks;
		attempts -= 1;
	}


	VisualMesh* visualMeshComp = this->sharks[sharkToMove]->FindComponent<VisualMesh>();


	glm::vec3 sharkPosition = visualMeshComp->getPosition();



	GlobalLerpProcessor::AddLerpPackage(
		new Vector3LerpPackage(
			sharkPosition,
			glm::vec3(sharkPosition.x, sharkPosition.y + 30, sharkPosition.z),
			[](glm::vec3 value) {},
			[visualMeshComp](glm::vec3 value) {
				visualMeshComp->setPos(value);
			},

			[](LerpPackage* pkg) {
				pkg->ResetTiming();
				pkg->swapStartAndTarget();
				pkg->current = 0.0f;
				((Vector3LerpPackage*)pkg)->startCallback = [pkg](glm::vec3) {
					pkg->destroyWhenFinished = true;
					pkg->finalCallback = [](LerpPackage* pkg) {};
					};
				GlobalLerpProcessor::AddLerpPackage(pkg);
			},
			7.0f,
			false
		)
	);

}
