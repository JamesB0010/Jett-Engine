#include "../Include/Jett Engine/FpsCameraController.h"
#include "../Include/Jett Engine/GameEngine.h"

void FpsCameraController::OnMouseUp()
{
	this->ownMouse = true;
}

void FpsCameraController::OnEscUp()
{
		this->ownMouse = false;
}

void FpsCameraController::OnTabUp()
{
	//disable player Ui
	GE::GameEngine::GetGameEngine()->ToggleUi();
}

FpsCameraController::FpsCameraController(GE::Camera* cam) {
	this->theCam = cam;
	this->oldTicks = SDL_GetTicks();

	//Get the window dimentions in order to determine centre
	SDL_Window* window = SDL_GL_GetCurrentWindow();


	int w, h;

	SDL_GetWindowSize(window, &w, &h);

	//Set mouse to middle of window from which to work out
	//a deflection
	SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);

	MemberFuncButtonListener<FpsCameraController>* mouseLeftClickListener = new MemberFuncButtonListener<FpsCameraController>(this, &FpsCameraController::OnMouseUp);
	GE::GameEngine::AddMouseUpEventListener(SDL_BUTTON_LEFT, mouseLeftClickListener);


	MemberFuncKeyListener<FpsCameraController>* escClickListener = new MemberFuncKeyListener<FpsCameraController>(this, &FpsCameraController::OnEscUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_ESCAPE, escClickListener);

	MemberFuncKeyListener<FpsCameraController>* tabClickListener = new MemberFuncKeyListener<FpsCameraController>(this, &FpsCameraController::OnTabUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_TAB, tabClickListener);
}

void FpsCameraController::Update()
{
	if (!enabled)
		return;
	const float mouseSens = 0.1f; //Adjust accordingly - should be part of the engine variables
	const float moveDist = 25.0f;

	//Get time now
	Uint32 newTicks = SDL_GetTicks();

	//calculate elapsed time for determining move amount later
	Uint32 diffTicks = newTicks - oldTicks;

	//get window width and height in order to work out midpoint
	//Used to determine deflection
	SDL_Window* window = SDL_GL_GetCurrentWindow();

	int w, h;

	SDL_GetWindowSize(window, &w, &h);

	//Get current mouse x and y
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	//Calculate the deflection - note inversion on y as this is flipped
	int diffX = mouse_x - (w / 2);
	int diffY = (h / 2) - mouse_y;

	if (this->ownMouse) {
		//Update the yaw and pitch based on the mouse deflection
		theCam->setYaw(theCam->getYaw() + diffX * mouseSens);
		theCam->setPitch(theCam->getPitch() + diffY * mouseSens);
	}

	//read keyboard directly
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	//move in direction based on time elapsed since last update
	if (keyboardState[SDL_SCANCODE_W])
		theCam->setPos(theCam->getPos() + theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));

	if (keyboardState[SDL_SCANCODE_S])
		theCam->setPos(theCam->getPos() - theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));

	if (keyboardState[SDL_SCANCODE_A])
		theCam->setPos(theCam->getPos() - glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));

	if (keyboardState[SDL_SCANCODE_D])
		theCam->setPos(theCam->getPos() + glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));

	if (keyboardState[SDL_SCANCODE_Q])
		theCam->setPos(theCam->getPos() - theCam->getUpDir() * (moveDist * diffTicks / 1000.0f));

	if (keyboardState[SDL_SCANCODE_E])
		theCam->setPos(theCam->getPos() + theCam->getUpDir() * (moveDist * diffTicks / 1000.0f));
		
		
	if (this->ownMouse == true) 
		//Set mouse to middle of window
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);

	this->oldTicks = newTicks;
}
