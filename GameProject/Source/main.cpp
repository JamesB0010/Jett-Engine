#include <Jett Engine/GameEngine.h>
#include <Jett Engine/JettEngineProjectFunctions.h>
#include <Jett Engine/GameEngineFactory.h>
#include "../Include/main.h"


int main(int argc, char* argv[]) {
	std::srand(std::time(nullptr));
	GE::GameEngine* gameEngine = GE::GameEngineFactory::CreateGameEngine();

	if (GE::GameEngineFactory::DidEngineCreateUnsuccessfully()) {
		std::cout << GE::GameEngineFactory::GetEngineCreationErrorMsg();
		return EXIT_FAILURE;
	}
	
	LoadLevelMeshes(gameEngine, "../../Assets/Scenes/SpaceShipSonyLvlEditor.lvl");
//LoadLevelMeshes(gameEngine, "../../Assets/Scenes/LightingTestLevel.lvl");
	AddGameObjects(gameEngine);
	
	//SetupScene(&gameEngine, "../../Assets/Scenes/ChibiBattle.lvl");

	gameEngine->Start();

	MainLoop(gameEngine);

	gameEngine->Shutdown();

	return EXIT_SUCCESS;
}

