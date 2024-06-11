#pragma once
#include "GameEngine.h"
namespace GE {
class GameEngineFactory
{
public:
	static GE::GameEngine* CreateGameEngine();
	static bool DidEngineCreateSuccessfully();
	static bool DidEngineCreateUnsuccessfully();
	static const char* GetEngineCreationErrorMsg();
private:
	static bool engineCreatedSuccessfully;
	static const char* creationErrorMessage;
};

}

