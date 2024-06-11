#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
class GUIImage
{
	GE::Texture* texture = nullptr;
	glm::vec2 pos = glm::vec2(0, 0);
public:
	GUIImage(glm::vec2 position, GE::Texture* texture);
	GUIImage(glm::vec2 position, std::string texturePath);
	GUIImage(glm::vec2 position, glm::vec2 dimentions, std::string texturePath);
	~GUIImage();

	GE::Texture* getTexture();
	glm::vec2 getPosition();

	void setTexture(GE::Texture* texture);

	void setPosition(glm::vec2 position);

	void offsetPosition(glm::vec2 offset);

	void setWidthHeight(glm::vec2 widthHeight);

	bool isInside(int x, int y);
};

