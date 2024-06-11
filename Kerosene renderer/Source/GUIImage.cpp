#include "../Include/Kerosene Renderer/GUIImage.h"

GUIImage::GUIImage(glm::vec2 position, GE::Texture* texture)
{
    this->pos = position;
    this->texture = texture;
}

GUIImage::GUIImage(glm::vec2 position, std::string texturePath)
{
    this->texture = new GE::Texture(texturePath);
    this->pos = position;
}

GUIImage::GUIImage(glm::vec2 position, glm::vec2 dimentions, std::string texturePath)
{
    this->pos = position;
    this->texture = new GE::Texture(texturePath);
    this->texture->setWidth(dimentions.x);
    this->texture->setHeight(dimentions.y);
}


GUIImage::~GUIImage()
{
}

GE::Texture* GUIImage::getTexture()
{
    return this->texture;
}

glm::vec2 GUIImage::getPosition()
{
    return this->pos;
}

void GUIImage::setTexture(GE::Texture* texture)
{
    this->texture = texture;
}

void GUIImage::setPosition(glm::vec2 position)
{
    this->pos = position;
}

void GUIImage::offsetPosition(glm::vec2 offset)
{
    this->pos += offset;
}

void GUIImage::setWidthHeight(glm::vec2 widthHeight) {
    this->texture->setWidth(widthHeight.x);
    this->texture->setHeight(widthHeight.y);
}

bool GUIImage::isInside(int x, int y) {
    glm::vec2 topLeft = this->pos;

    glm::vec2 bottomRight = topLeft;
    bottomRight.x += this->texture->getWidth();
    bottomRight.y += this->texture->getHeight();

    bool withinXBounds = x > topLeft.x && x < bottomRight.x;
    bool withinYBounds = y > topLeft.y && y < bottomRight.y;
    if (withinXBounds && withinYBounds)
        return true;

    return false;
}