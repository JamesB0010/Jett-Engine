#include "../Include/PlayerOnMinimap.h"

PlayerOnMinimap::PlayerOnMinimap(std::string uiPlayerImage, glm::vec2 dimentions)
{
	this->uiPlayerImage = new GUIImage(glm::vec2(0, 0), uiPlayerImage);
	this->uiPlayerImage->setWidthHeight(dimentions);
}

void PlayerOnMinimap::Update()
{
	int maxX = 220;
	int minX = -280;

	int minZ = -250;
	int maxZ = 250;
	
	glm::vec3 playerPosition = GE::GameEngine::camera->getPos();
	glm::vec2 uiPosition = GE::mapToNewRange(glm::vec2(playerPosition.x, playerPosition.z), glm::vec2(minX, minZ), glm::vec2(maxX, maxZ), glm::vec2(0, 0), glm::vec2(200, 200));

	uiPosition.x -= this->uiPlayerImage->getTexture()->getWidth() / 2;
	uiPosition.y -= this->uiPlayerImage->getTexture()->getHeight() / 2;

	uiPosition.x = uiPosition.x > 160 ? 160 : uiPosition.x;
	uiPosition.x = uiPosition.x < -40 ? -40 : uiPosition.x;

	uiPosition.y = uiPosition.y > 150 ? 150 : uiPosition.y;
	uiPosition.y = uiPosition.y < -36 ? -36 : uiPosition.y;

	this->uiPlayerImage->setPosition(uiPosition);
}

GUIImage* PlayerOnMinimap::getUiImage()
{
	return this->uiPlayerImage;
}
