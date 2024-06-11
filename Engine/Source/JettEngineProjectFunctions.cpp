#include "../Include/Jett Engine/JettEngineProjectFunctions.h"

void MainLoop(GE::GameEngine* ge) {
	while (ge->KeepRunning()) {
		//update game state
		ge->Update();

		//draw
		ge->Draw();
	}
}