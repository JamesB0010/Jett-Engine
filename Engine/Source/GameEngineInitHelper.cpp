
#include "../Include/Jett Engine/GameEngineInitHelper.h"
GE::GameEngine* GE::GameEngine::gameEngine = nullptr;

GE::GameEngineInitHelper::GameEngineInitHelper(GE::GameEngine* ge) {
	this->gameEngine = ge;
}

void GE::GameEngineInitHelper::Init()
{
	try {
	this->InitSDLVideoSubsystem();
	}
	catch (VideoSubsystemInitError err) {
		throw(new EngineInitError(err.what()));
	}


	this->SetInitSDLAttributes();

	try {
	this->CreateSdlWindow();
	}
	catch (CreateSDLWindowError err) {
		throw(new EngineInitError(err.what()));
	}


	try {
	this->CreateGlContext();
	}
	catch (GlContextCreationError err) {
		throw(new EngineInitError(err.what()));
	}


	try {
	this->InitGLEW();
	}
	catch (GLEWInitError err) {
		throw(new EngineInitError(err.what()));
	}


	try {
	this->EnableVSync();
	}
	catch (EnableVsyncError err) {
		throw(new EngineInitError(err.what()));
	}

	TextureFormatMapper::init();

	CreateCamera();

	GameEngine::gameEngine = this->gameEngine;

	this->gameEngine->Awake();
}



const char* GE::GameEngineInitHelper::CreateErrorC_String(const char* string1, const char* string2)
{
	std::string errorMessage(string1);
	errorMessage += string2;
	return errorMessage.c_str();
}

void GE::GameEngineInitHelper::InitSDLVideoSubsystem()
{
	//Initialise SDL Video Subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		auto err = new GE::VideoSubsystemInitError(CreateErrorC_String("Unable to initialise SDL! SDL error: ", SDL_GetError()));
		throw (err);
	}
}

void GE::GameEngineInitHelper::SetInitSDLAttributes()
{
	//Set the OpenGL version for the program
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//set the type of profile which is core meaning modern OpenGl
	//means no legacy features for backwards compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void GE::GameEngineInitHelper::CreateSdlWindow()
{
	this->gameEngine->window = SDL_CreateWindow("Jett Engine", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (this->gameEngine->window == nullptr) {
		auto err = new GE::CreateSDLWindowError(CreateErrorC_String("Unable to create window! SDL error: ", SDL_GetError()));
		throw(err);
	}
}

void GE::GameEngineInitHelper::CreateGlContext()
{
	//Creates the OpenGL context and links it to the window object
	//Context represents OpenGl for program such as objects and drawings
	this->gameEngine->glContext = SDL_GL_CreateContext(this->gameEngine->window);

	//Check context object was created
	if (this->gameEngine->glContext == nullptr) {
		auto err = new GE::GlContextCreationError(CreateErrorC_String("SDL could not create GL context! SDL error: ", SDL_GetError()));
		throw(err);
	}
}

void GE::GameEngineInitHelper::InitGLEW()
{
	//Initialise GLEW. GLEW solves a problem with OpenGL on windows
	//as default API suppourted by windows is 1.1 (from 1997)
	//GLEW binds latest extensions that can be used
	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::string glewErrStr(reinterpret_cast<const char*>(glewGetErrorString(status)));
		auto err = new GE::GLEWInitError(CreateErrorC_String("Error initialising GLEW! Error: ", glewErrStr.c_str()));
		throw(err);
	}
}

void GE::GameEngineInitHelper::EnableVSync()
{
	//try to turn on VSync. Maybe this could be a parameter to the init method
		//rather than hard coded
	if (SDL_GL_SetSwapInterval(this->vsynchEnabled) != 0) {
		auto err = new EnableVsyncError(CreateErrorC_String("Warning: unable to set VSync! Error: ", SDL_GetError()));
		throw(err);
	}
}

void GE::GameEngineInitHelper::CreateCamera()
{
	CameraViewVectors camViewVectors = {
		glm::vec3(0.0f, 0.0f, 50.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	CameraProjectionData camProjectionData = {
		45.0f,
		640.0f / 480.0f,
		0.01f,
		1000.0f
	};

	this->gameEngine->camera = new Camera(&camViewVectors, &camProjectionData);

	this->CreateFPSController();
}

void GE::GameEngineInitHelper::CreateFPSController()
{
	this->gameEngine->fpsController = new FpsCameraController(this->gameEngine->camera);
}