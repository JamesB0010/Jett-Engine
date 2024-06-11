#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <Kerosene Renderer/TriangleRenderer.h>
#include <Kerosene Renderer/Camera.h>
#include <Kerosene Renderer/Model.h>
#include "FPSCounter.h"
#include <Kerosene Renderer/ModelRenderer.h>
#include <Kerosene Renderer/Texture.h>
#include <Kerosene Renderer/SkyboxRenderer.h>
#include <Kerosene Renderer/Terrain.h>
#include "FpsCameraController.h"
#include "GlobalLerpProcessor.h"
#include "Vector3LerpPackage.h"
#include "GameObject.h"
#include "VisualMesh.h"
#include "SkysphereComponent.h"
#include <Kerosene Renderer/FontRenderer.h>
#include <Kerosene Renderer/GUIRenderer.h>
#include "Time.h"
#include <chrono>
#include "../../InputEventsCallbackBinds.h"
#include "../../ObjectPicker.h"
#include "../../ObjectDeleter.h"
#include <memory>
#include "../../ContentDrawer.h"
#include <Kerosene Renderer/FrameBuffer.h>
#include <Kerosene Renderer/PostProcessGrey.h>

namespace GE {
	class GameEngineInitHelper;
	class GameEngine {
		//SDL Window objects represent the frame
		SDL_Window* window = nullptr;

		//SDL GL context represents OpenGl in the program
		SDL_GLContext glContext;

		//Fps camera controller
		FpsCameraController* fpsController;

		std::vector<GameObject*> gameObjects;

		//terrain
		Terrain* terrain;
		Texture* terrainTexture;
		ModelRenderer* modelRendererTerrain;

		SkyboxRenderer* skybox;

		GameObject* GUI = nullptr;

		GameObject* EditorUi = nullptr;

		std::vector<GUIImage*> contentDrawerImages;

		static GameEngine* gameEngine;

		std::shared_ptr<ObjectPicker> objectPicker = nullptr;

		std::unique_ptr<ObjectDeleter> objectDeleter = nullptr;

		std::unique_ptr<ContentDrawer> contentDrawer = nullptr;

		FrameBuffer* fb;

		Model* quadModel;
		ModelRenderer* quadModelRenderer;

		FrameBuffer* shadowFrameBuffer;

		Model* quadModelShadow;
		ModelRenderer* shadowQuadRenderer;

		PostProcessGreyscale* ppg;

		GameObject* sky = nullptr;
	public:
		static Camera* camera;

		void AddSky(GameObject* sky) {
			this->sky = sky;
		};

		static std::unordered_map<int, std::vector<InputCallback*>> mouseUpEvents;

		static std::unordered_map<SDL_Scancode, std::vector<InputCallback*>> keyUpEvents;

		static void AddMouseUpEventListener(int button, InputCallback* inputCallback);

		static void AddKeyUpEventListener(SDL_Scancode key, InputCallback* inputCallback);

		static void SetFpsCamMouseOwnership(bool newOwnership);

		static std::weak_ptr<ObjectPicker> getObjectPicker();

		static void DeleteObject(GameObject* objectToDelete);

		static void StaticAddGameObject(GameObject* object) {
			GameEngine::gameEngine->AddGameObject(object);
		}

		SDL_Event event;

		GameEngine();

		void Awake();

		void Start();

		SkyboxRenderer* createSkybox(const char* filepathToDirectory, const char* image1, const char* image2, const char* image3, const char* image4, const char* image5, const char* image6);

		virtual ~GameEngine();

		bool KeepRunning(); //Indicates if user has closed window and, hence, game

		bool CheckForQuitEvent(); //keep running helper

		void Update();

		void Draw();

		void PrepareWindowForDrawing();

		void ClearScreen();

		void Shutdown();

		void setWindowTitle(const char* new_title);

		void AddGameObject(GameObject* object);

		void OnLCTRLUp();

		static glm::vec2 getWindowDimentions();

		Camera* getCamera();

		void AttachToContentDrawer(GUIImage* image, std::string cahcedModelFilename) {
			this->contentDrawerImages.push_back(image);
			int index = this->contentDrawerImages.size() - 1;
			this->contentDrawer.get()->AddIndexAndModelFileName(index, cahcedModelFilename);
		}

		static GUIImage* getContentDrawerImage(int index) {
			return GameEngine::gameEngine->contentDrawerImages[index];
		}

		static std::vector<GUIImage*> getContentDrawerImages() {
			return GameEngine::gameEngine->contentDrawerImages;
		}



		static FpsCameraController* getFpsCamera() {
			return GameEngine::gameEngine->fpsController;
		}
		void AddGUI(GameObject* GUI);

		void AddEditorGui(GameObject* GUI) {
			this->EditorUi = GUI;
		}

		static GameEngine* GetGameEngine();

		void ToggleUi();

		static std::vector<GameObject*> findVisualMeshGameObjects();

		friend class GameEngineInitHelper;
	};

	//helpful function part of GE namespace
	void DisplayInfoMessage(const char* msg);


	glm::vec2 mapToNewRange(glm::vec2 value, glm::vec2 startMin, glm::vec2 startMax, glm::vec2 newMin, glm::vec2 newMax);
}