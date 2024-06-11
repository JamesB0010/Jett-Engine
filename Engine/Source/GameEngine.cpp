#include "../Include/Jett Engine/GameEngine.h"

GE::Camera* GE::GameEngine::camera = nullptr;

std::unordered_map<int, std::vector<InputCallback*>> GE::GameEngine::mouseUpEvents;

std::unordered_map<SDL_Scancode, std::vector<InputCallback*>> GE::GameEngine::keyUpEvents;

namespace GE {
	void GameEngine::AddMouseUpEventListener(int button, InputCallback* inputCallback)
	{
		auto it = GameEngine::mouseUpEvents.find(button);

		if (it == GameEngine::mouseUpEvents.end()) {
			GameEngine::mouseUpEvents[button] = std::vector<InputCallback*>();
		}

		GameEngine::mouseUpEvents[button].push_back(inputCallback);
	}
	void GameEngine::AddKeyUpEventListener(SDL_Scancode key, InputCallback* inputCallback)
	{
		auto it = GameEngine::keyUpEvents.find(key);

		if (it == GameEngine::keyUpEvents.end()) {
			GameEngine::keyUpEvents[key] = std::vector<InputCallback*>();
		}

		GameEngine::keyUpEvents[key].push_back(inputCallback);
	}

	void GameEngine::SetFpsCamMouseOwnership(bool newOwnership)
	{
		GameEngine::gameEngine->fpsController->setMouseOwnership(newOwnership);
	}

	std::weak_ptr<ObjectPicker> GameEngine::getObjectPicker()
	{
		return std::weak_ptr<ObjectPicker>(GameEngine::gameEngine->objectPicker);
	}

	void GameEngine::DeleteObject(GameObject* objectToDelete)
	{
		auto it = std::find(GameEngine::gameEngine->gameObjects.begin(), GameEngine::gameEngine->gameObjects.end(), objectToDelete);
		GameEngine::gameEngine->gameObjects.erase(it);
		delete objectToDelete;
	}

	GameEngine::GameEngine() {
	}

	SkyboxRenderer* GameEngine::createSkybox(const char* filepathToDirectory, const char* image1, const char* image2, const char* image3, const char* image4, const char* image5, const char* image6)
	{
		std::string filepathStr(filepathToDirectory);
		this->skybox = new SkyboxRenderer(
			filepathStr + image1,
			filepathStr + image2,
			filepathStr + image3,
			filepathStr + image4,
			filepathStr + image5,
			filepathStr + image6
		);
		return this->skybox;
	}


	GameEngine::~GameEngine() {

	}

	//A game would call this function to check if the user closed the window 
	//resulting in a SDL_QUIT message
	bool GameEngine::KeepRunning()
	{
		bool quitGame = this->CheckForQuitEvent();
		if (quitGame)
			return false;

		//got this far means no quit event so keep running
		return true;
	}
	bool GameEngine::CheckForQuitEvent()
	{
		//Need to do this keep the event queue up to date
		SDL_PumpEvents();

		SDL_Event evt;
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			//if user quit program, then return true meaning
			//quit event found so quit the game
			return true;
		}

		if (evt.button.type == SDL_MOUSEBUTTONUP) {
			std::cout << "mouse button pressed" << std::endl;
		}
		//no quit event found
		return false;
	}
	void GameEngine::Awake()
	{
		FontRenderer::InitTTF();
		this->objectPicker = std::make_shared<ObjectPicker>();
		this->objectDeleter = std::make_unique<ObjectDeleter>();
		this->contentDrawer = std::make_unique<ContentDrawer>();

		//Add editor UI

		glm::vec2 windowDimentions = this->getWindowDimentions();

		GUIRenderer* editorUiRenderer = new GUIRenderer(windowDimentions);
		GameObject* editorRendererObj = new GameObject("Editor GUI renderer");
		editorRendererObj->AddComponent<GUIRenderer>(editorUiRenderer);
		this->AddEditorGui(editorRendererObj);

		GUIImage* contentDrawer = new GUIImage(glm::vec2(windowDimentions.x / 2, windowDimentions.y), glm::vec2(650, 150), "../../Assets/Ui/Images/ContentDrawer.jpg");
		contentDrawer->offsetPosition(glm::vec2(-contentDrawer->getTexture()->getWidth() / 2, 0));
		this->AttachToContentDrawer(contentDrawer, "None");
		editorUiRenderer->AddImageToGUI(contentDrawer);

		//add model images to content drawer
		GUIImage* BirdThumbnail = new GUIImage(glm::vec2(20, 505), glm::vec2(100, 100), "../../Assets/Assets/Bird/BirdThumbnail.png");
		editorUiRenderer->AddImageToGUI(BirdThumbnail);
		this->AttachToContentDrawer(BirdThumbnail, "Assets/Bird/Bird.dae");

		GUIImage* SharkThumbnail = new GUIImage(glm::vec2(175, 505), glm::vec2(100, 100), "../../Assets/Assets/Shark/SharkThumbnail.png");
		editorUiRenderer->AddImageToGUI(SharkThumbnail);
		this->AttachToContentDrawer(SharkThumbnail, "Assets/Shark/Shark.dae");

		GUIImage* CannonThumbnail = new GUIImage(glm::vec2(325, 505), glm::vec2(100, 100), "../../Assets/Assets/Cannon/CannonThumbnail.png");
		editorUiRenderer->AddImageToGUI(CannonThumbnail);
		this->AttachToContentDrawer(CannonThumbnail, "Assets/Cannon/CannonModel.dae");

		GUIImage* BoatThumbnail = new GUIImage(glm::vec2(500, 505), glm::vec2(100, 100), "../../Assets/Assets/Boat/BoatThumbnail.png");
		editorUiRenderer->AddImageToGUI(BoatThumbnail);
		this->AttachToContentDrawer(BoatThumbnail, "Assets/Boat/Boat.dae");

		MemberFuncKeyListener<GameEngine>* leftControlListener = new MemberFuncKeyListener<GameEngine>(this, &GameEngine::OnLCTRLUp);
		this->AddKeyUpEventListener(SDL_SCANCODE_LCTRL, leftControlListener);
	}
	void GameEngine::Start()
	{
		GUIText* testText = new GUIText(glm::vec2(this->gameEngine->getWindowDimentions().x / 2, 200), "Editor Text test", "../../Assets/Fonts/Oswald-Regular.ttf", 20);
		testText->offsetPosition(glm::vec2(-testText->getTexture()->getWidth() / 2, 0));
	
		this->terrain = new Terrain(std::string("../../Assets/Terrain/terrain-heightmapHoleInMiddle.png"), 10, 100);
		this->terrainTexture = new Texture("../../Assets/Terrain/terrain-texture.png");
		this->modelRendererTerrain = new ModelRenderer();
		this->modelRendererTerrain->Init();
		this->modelRendererTerrain->setTexture(this->terrainTexture);
		this->modelRendererTerrain->setPos(-280, -17, -250);
		this->modelRendererTerrain->setScale(5, 2, 5);

		for (int i = 0; i < this->gameObjects.size(); i++) {
			this->gameObjects[i]->Start();
		}

		//framebuffer stuff
		fb = new FrameBuffer(getWindowDimentions());

		fb->init();


		quadModel = new Model();

		std::vector<glm::vec3> vertexPositions = {
		glm::vec3(-1, 1, 0),
		glm::vec3(-1,-1,0),
		glm::vec3(1,-1,0),

		glm::vec3(1,1,0),
		glm::vec3(-1,1,0),
		glm::vec3(1,-1,0)
		};

		quadModel->createFromVertexPosVector(vertexPositions);

		quadModelRenderer = new ModelRenderer();

		quadModelRenderer->Init();

		quadModelRenderer->setPos(0.0f, 0.0f, -10.0f);
		quadModelRenderer->setScale(10.0f, 10.0f, 10.0f);

		quadModelRenderer->setTexture(fb->getTexture());

		ppg = new PostProcessGreyscale(getWindowDimentions().x, getWindowDimentions().y);

		ppg->init();

		this->shadowFrameBuffer = new FrameBuffer(getWindowDimentions());

		this->shadowFrameBuffer->init();

		this->quadModelShadow = new Model();

		this->quadModelShadow->createFromVertexPosVector(vertexPositions);

		this->shadowQuadRenderer = new ModelRenderer();

		this->shadowQuadRenderer->Init();

		this->shadowQuadRenderer->setPos(0.0f, 0.0f, -10.0f);
		this->shadowQuadRenderer->setScale(10.0f, 10.0f, 10.0f);

		this->shadowQuadRenderer->setTexture(this->shadowFrameBuffer->getTexture());
	}

	void GameEngine::Update()
	{
		//get inputs
		while (SDL_PollEvent(&this->event)) {
			switch (this->event.type) {
			case SDL_MOUSEBUTTONUP:
			{
				auto it = GameEngine::mouseUpEvents.find(this->event.button.button);

				if (it != GameEngine::mouseUpEvents.end()) {
					//some one is listening
					for (int i = 0; i < it->second.size(); ++i) {
						it->second[i]->runCallback();
					}
				}
			}
			break;
			case SDL_KEYUP:
			{
				auto it = GameEngine::keyUpEvents.find(this->event.key.keysym.scancode);

				if (it != GameEngine::keyUpEvents.end()) {
					//some one is listening
					for (int i = 0; i < it->second.size(); ++i) {
						it->second[i]->runCallback();
					}
				}
			}
			break;
			}
		}

		Time::deltaTime = (SDL_GetTicks() - Time::timeSinceStart);
		Time::timeSinceStart = SDL_GetTicks();

		this->fpsController->Update();

		this->objectPicker->Update();

		GlobalLerpProcessor::Update();

		for (int i = 0; i < this->gameObjects.size(); ++i) {
			this->gameObjects[i]->Update();
		}

	}

	void GameEngine::Draw()
	{
		fb->beginRender();
		//api error here
		glPolygonMode(GL_LINE, GL_FRONT_AND_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		////std::cout << glGetError() << std::endl;

		PrepareWindowForDrawing();


		if (this->sky != nullptr)
			this->sky->RenderComponents();

		if(this->skybox != nullptr)
			this->skybox->draw(camera);
		

		this->modelRendererTerrain->Draw(camera, this->terrain);

		for (int i = 0; i < this->gameObjects.size(); ++i) {
			this->gameObjects[i]->RenderComponents();
		}

		if (this->GUI != nullptr)
			this->GUI->RenderComponents();

		if (this->EditorUi != nullptr)
			this->EditorUi->RenderComponents();

		fb->endRender();

		ppg->apply(fb->getTexture());

		PrepareWindowForDrawing();

		ppg->renderToScreen();

		SDL_GL_SwapWindow(this->window);
	}


	void GameEngine::PrepareWindowForDrawing()
	{
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GameEngine::ClearScreen()
	{
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GameEngine::Shutdown()
	{
		SDL_DestroyWindow(this->window);

		this->window = nullptr;
		if (this->skybox != nullptr) {
		this->skybox->destroy();
		delete skybox;
		}

		FontRenderer::ShutDownTTF();

		SDL_Quit();
	}

	void GameEngine::setWindowTitle(const char* new_title)
	{
		SDL_SetWindowTitle(this->window, new_title);
	}

	void GameEngine::AddGameObject(GameObject* object)
	{
		this->gameObjects.push_back(object);
	}

	glm::vec2 GameEngine::getWindowDimentions()
	{
		int width, height;

		SDL_GetWindowSize(GameEngine::gameEngine->window, &width, &height);
		return glm::vec2(width, height);
	}

	Camera* GameEngine::getCamera() {
		return camera;
	}


	void GameEngine::OnLCTRLUp()
	{
		this->ppg->ToggleEnabled();
	}

	void GameEngine::AddGUI(GameObject* GUI)
	{
		this->GUI = GUI;
	}

	GameEngine* GameEngine::GetGameEngine()
	{
		return GameEngine::gameEngine;
	}

	void GameEngine::ToggleUi()
	{
		this->GUI->FindComponent<GUIRenderer>()->ToggleEnabled();
	}

	std::vector<GameObject*> GameEngine::findVisualMeshGameObjects()
	{
		std::vector<GameObject*> drawableObjects;
		for (GameObject* obj : GameEngine::gameEngine->gameObjects) {
			if (obj->FindComponent<VisualMesh>() != nullptr)
				drawableObjects.push_back(obj);
		}
		return drawableObjects;
	}

	//simple helper function to display program information
	//note it is seperate from the GameEngine class as it doesn't depend on it
	void DisplayInfoMessage(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Jett Engine", msg, nullptr);
	}



	glm::vec2 mapToNewRange(glm::vec2 value, glm::vec2 startMin, glm::vec2 startMax, glm::vec2 newMin, glm::vec2 newMax)
	{
		return (value - startMin) * (newMax - newMin) / (startMax - startMin) + newMin;
	}
}