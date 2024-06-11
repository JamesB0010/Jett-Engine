#pragma once
#include <Jett Engine/GameEngine.h>
#include <Jett Engine/LvlFileReader.h>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <string>
#include <cmath>
#include <Jett Engine/SkysphereComponent.h>
#include <Jett Engine/VisualMesh.h>
#include "SharkManager.h"
#include <Jett Engine/GameComponent.h>
#include <Kerosene Renderer/GUIRenderer.h>
#include "PlayerOnMinimap.h"
#include "UIFpsCounter.h"
#include <Kerosene Renderer/RenderModes.h>
#include <Jett Engine/OceanVisualMesh.h>
#include "../../Engine/ParticleEmmitter.h"
#include <Kerosene Renderer/reflectiveModelRenderer.h>
#include <Kerosene Renderer/SkyboxRenderer.h>

void LoadLevelMeshes(GE::GameEngine* gameEngine, std::string scenePath) {

	LvlFileReader levelReader(scenePath);

	levelReader.SpecifyObjectRenderingMode("Assets/PalmTree/PalmTree.dae", renderModes::instanced);

	levelReader.AddGameObjectsToEngine(gameEngine);
}

void AddGameObjects(GE::GameEngine* gameEngine) {

	gameEngine->createSkybox("../../Assets/Skybox/SeaSkybox/",
			"Right.png",
			"Left.png",
			"Bottom.png",
			"Top.png",
			"Front.png",
			"Back.png"
		);


	//Add Skysphere
	/*GameObject* skySphere = new GameObject("Sky Sphere");
	SkysphereComponent* component = new SkysphereComponent("../../Assets/SkySphere/skydomeTexture.jpg", gameEngine->camera);
	skySphere->AddComponent<SkysphereComponent>(component);
	gameEngine->AddSky(skySphere);*/


	//Add shark manager
	SharkManager* sharkManager = new SharkManager();
	sharkManager->AddSharksToEngine(gameEngine);
	GameObject* sharkManagerObject = new GameObject("Shark manager");
	sharkManagerObject->AddComponent(sharkManager);
	gameEngine->AddGameObject(sharkManagerObject);

	//add ocean
	OceanVisualMesh* oceanMesh = OceanVisualMesh::heapAllocateNewOceanVisualMesh(gameEngine->camera);
	GameObject* oceanObject = new GameObject("Ocean");
	oceanObject->AddComponent<OceanVisualMesh>(oceanMesh);
	gameEngine->AddGameObject(oceanObject);
	oceanMesh->setPos(glm::vec3( - 500, -14, 500));

	//GameObject* testObject = new GameObject("test obj");
	//VisualMesh* visMesh = new VisualMesh("../../Assets/Assets/Chest.obj", "../../Assets/Assets/chest.png", gameEngine->camera);
	//testObject->AddComponent<VisualMesh>(visMesh);
	//gameEngine->AddGameObject(testObject);


	//Add Gui Renderer
	GUIRenderer* guiRenderer = new GUIRenderer(gameEngine->getWindowDimentions());
	GameObject* guiRendererObj = new GameObject("GUI renderer");
	guiRendererObj->AddComponent<GUIRenderer>(guiRenderer);
	gameEngine->AddGUI(guiRendererObj);



	//Create things to be added to gui
	//center text
	GUIText* centerText = new GUIText(glm::vec2(gameEngine->getWindowDimentions().x / 2, 0), "Awesome Pirate Game!", "../../Assets/Fonts/Oswald-Regular.ttf", 20);
	centerText->offsetPosition(glm::vec2(-centerText->getTexture()->getWidth() / 2, 0));

	//minimap background
	GUIImage* minimap = new GUIImage(glm::vec2(0, 0), glm::vec2(200, 200), "../../Assets/Terrain/MinimapTexture.png");

	//Player
	GameObject* playerOnMinimap = new GameObject("Player on Minimap");
	PlayerOnMinimap* playerOnMinimapScript = new PlayerOnMinimap("../../Assets/Ui/Images/PiratePlayer.png", glm::vec2(75,75));
	playerOnMinimap->AddComponent<PlayerOnMinimap>(playerOnMinimapScript);
	gameEngine->AddGameObject(playerOnMinimap);

	//fps counter
	GameObject* fpsCounter = new GameObject("Fps counter: 00000");
	GUIText* fpsText = new GUIText(glm::vec2(gameEngine->getWindowDimentions().x, 0), "Fps: 140", "../../Assets/Fonts/Oswald-Regular.ttf", 20);
	fpsText->offsetPosition(glm::vec2(-fpsText->getTexture()->getWidth() - 20, 0));
	UiFpsCounter* counterComponent = new UiFpsCounter(fpsText);
	fpsCounter->AddComponent<UiFpsCounter>(counterComponent);
	gameEngine->AddGameObject(fpsCounter);



	//actually add the things
	guiRenderer->AddImageToGUI(minimap);
	guiRenderer->AddImageToGUI(playerOnMinimapScript->getUiImage());
	guiRenderer->AddTextToGUI(centerText);
	guiRenderer->AddTextToGUI(fpsText);



	//add particle system
	GameObject* particleObject = new GameObject("Particle system");
	ParticleEmmitter* emmitter = new ParticleEmmitter(200);
	particleObject->AddComponent<ParticleEmmitter>(emmitter);
	emmitter->setOrigin(glm::vec3(0, 0, 67.5));
	gameEngine->AddGameObject(particleObject);

//add reflective sphere

	std::string sourceCodeVertex = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/reflective.vs");
	std::string* sourceCodeVertexHeap = new std::string;
	*sourceCodeVertexHeap = sourceCodeVertex;
	const GLchar* vshaderCode[] = { sourceCodeVertexHeap->c_str() };

	std::string sourceCode = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/reflective.fs");
	std::string* sourceCode2 = new std::string;
	*sourceCode2 = sourceCode;
	const GLchar* fShadeCode[] = { sourceCode2->c_str() };
	ModelRenderer* mr = new reflectiveModelRenderer();

	GE::Texture* tex = new GE::Texture("../../Assets/Assets/Sphere/Untitled.png");
	mr->Init(vshaderCode, fShadeCode);
	VisualMesh* chromeSphereVisMesh = new VisualMesh("../../Assets/Assets/Sphere/Sphere.dae", "../../Assets/Assets/Sphere/Untitled.png",gameEngine->camera, glm::vec3(-60,40, -50), mr);
	chromeSphereVisMesh->setScale(glm::vec3(5.0f,5.0f,5.0f));
	chromeSphereVisMesh->SetModelRendererTexture(tex);
	GameObject* chromeSphereObject = new GameObject("Chrome Sphere");
	chromeSphereObject->AddComponent<VisualMesh>(chromeSphereVisMesh);
	gameEngine->AddGameObject(chromeSphereObject);
	delete sourceCode2;
	delete sourceCodeVertexHeap;
}