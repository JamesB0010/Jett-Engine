#pragma once
#include <Jett Engine/GameComponent.h>
#include <Jett Engine/GameEngine.h>
#include <Kerosene Renderer/GUIImage.h>
class PlayerOnMinimap : public GameComponent
{
	GUIImage* uiPlayerImage = nullptr;

public:
	PlayerOnMinimap(std::string uiPlayerImage, glm::vec2 dimentions);
	void Update() override;

	GUIImage* getUiImage();
};

