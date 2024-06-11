#pragma once
#include <SDL.h>
#include "../../Kerosene renderer/Include/Kerosene Renderer/Camera.h"
#include "../Jett Engine/GameEngine.h"
#include "GameEngineFactory.h"
#include <string>
#include "FpsCameraController.h"

namespace GE {


#pragma region exceptions

	class EngineInitError :
		public std::exception
	{
		const char* message;
	public:
		EngineInitError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};

	class VideoSubsystemInitError :
		public std::exception
	{
		const char* message;
	public:
		VideoSubsystemInitError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};

	class CreateSDLWindowError :
		public std::exception
	{
		const char* message;
	public:
		CreateSDLWindowError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};


	class GlContextCreationError :
		public std::exception
	{
		const char* message;
	public:
		GlContextCreationError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};

	class GLEWInitError :
		public std::exception
	{
		const char* message;
	public:
		GLEWInitError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};

	class EnableVsyncError :
		public std::exception
	{
		const char* message;
	public:
		EnableVsyncError(const char* msg) : message(msg) {}
		const char* what() {
			return message;
		}
	};
#pragma endregion exceptions

class GameEngineInitHelper
{
public:
	int vsynchEnabled = 1;

private:
	GE::GameEngine* gameEngine = nullptr;

	GameEngineInitHelper(GE::GameEngine* ge);

	void Init();

	static const char* CreateErrorC_String(const char* string1, const char* string2);

	void InitSDLVideoSubsystem();

	void SetInitSDLAttributes();

	void CreateSdlWindow();

	void CreateGlContext();

	void InitGLEW();

	void EnableVSync();

	void CreateCamera();

	void CreateFPSController();

	friend GameEngineFactory;
};
}

