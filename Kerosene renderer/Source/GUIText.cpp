#include "../Include/Kerosene Renderer/GUIText.h"

std::unordered_map<FontSizePair, FontRenderer*> GUIText::fontRenderers = std::unordered_map<FontSizePair, FontRenderer*>();


GUIText::GUIText(glm::vec2 position, std::string text, std::string font, int size) {
	FontRenderer* fontRenderer = nullptr;

	FontSizePair fontAndSize(font, size);

	auto it = GUIText::fontRenderers.find(fontAndSize);
	if (it == GUIText::fontRenderers.end()) {
		//requested font at the requested size does not exist
		try {
			fontRenderer = new FontRenderer(fontAndSize.font, fontAndSize.fontSize);
		}
		catch (FontRenderCreationError err) {
			std::cerr << err.what() << std::endl;
		}
		GUIText::fontRenderers[fontAndSize] = fontRenderer;
	}
	else {
		fontRenderer = it->second;
	}

	this->fontRenderer = fontRenderer;

	this->texture = fontRenderer->RenderText(text);

	this->pos = position;

	this->text = text;
}

GE::Texture* GUIText::getTexture()
{
	return this->texture;
}

glm::vec2 GUIText::getPosition()
{
	return this->pos;
}

void GUIText::setPosition(glm::vec2 newPosition)
{
	this->pos = newPosition;
}

void GUIText::offsetPosition(glm::vec2 offset)
{
	this->pos += offset;
}

void GUIText::setText(std::string text)
{
	if (this->text == text)
		return;
	this->texture = this->fontRenderer->RenderText(text);
}

GUIText::~GUIText()
{
	delete this->texture;
}
