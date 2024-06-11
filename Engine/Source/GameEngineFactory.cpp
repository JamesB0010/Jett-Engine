#include "../Include/Jett Engine/GameEngineFactory.h"
#include "../Include/Jett Engine/GameEngineInitHelper.h"

using namespace GE;

bool GameEngineFactory::engineCreatedSuccessfully = true;
const char* GameEngineFactory::creationErrorMessage = "";

GE::GameEngine* GameEngineFactory::CreateGameEngine()
{
	GameEngineFactory::engineCreatedSuccessfully = true;
	GE::GameEngine* engine = new GameEngine;

	GE::GameEngineInitHelper helper(engine);

	try {
	helper.Init();
	}
	catch (GE::EngineInitError err) {
		GameEngineFactory::engineCreatedSuccessfully = false;
		GameEngineFactory::creationErrorMessage = err.what();
	}

	return engine;
}

bool GameEngineFactory::DidEngineCreateSuccessfully()
{
	return GameEngineFactory::engineCreatedSuccessfully;
}

bool GameEngineFactory::DidEngineCreateUnsuccessfully()
{
	return !GameEngineFactory::engineCreatedSuccessfully;
}

const char* GameEngineFactory::GetEngineCreationErrorMsg()
{
	return GameEngineFactory::creationErrorMessage;
}
