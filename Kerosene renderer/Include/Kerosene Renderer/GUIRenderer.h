#pragma once
#include <glm/glm.hpp>
#include "GUIText.h"
#include "GUIImage.h"
#include "Texture.h"
#include "ShaderUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include "TriangleRenderer.h"
#include "../../../Engine/Include/Jett Engine/GameComponent.h"
#include <vector>


struct GuiElement {

	virtual GE::Texture* GetTextureFromElement() = 0;

	virtual glm::vec2 getPosition() = 0;

	virtual ~GuiElement() {

	}

protected:
	GuiElement() {

	}
};

struct TextUIElement : public GuiElement {
	GUIText* element;

	TextUIElement(GUIText* text) {
		this->element = text;
	}

	~TextUIElement() {
		delete this->element;
	}

	GE::Texture* GetTextureFromElement() override {
		return this->element->getTexture();
	}

	glm::vec2 getPosition() override {
		return this->element->getPosition();
	}


};

struct ImageUIElement : public GuiElement {
	GUIImage* element;

	ImageUIElement(GUIImage* image) {
		this->element = image;
	}

	~ImageUIElement() {
		delete this->element;
	}

	GE::Texture* GetTextureFromElement() override {
		return this->element->getTexture();
	}

	glm::vec2 getPosition() override {
		return this->element->getPosition();
	}
};

class GUIRenderer: public DrawableGameComponent
{
	GLuint programId;
	GLuint vertexLocation;
	GLuint vertexUVLocation;
	GLuint projectionUniformId;
	GLuint samplerId;

	glm::highp_mat4 projectionMat;

	std::vector<GuiElement*> guiElements;

	bool drawEnabled = true;

public:
	GUIRenderer(glm::vec2 windowDimentions);
	~GUIRenderer();

	//Draws a texture object. Will use initially for
	//drawing text and use generically for other
	//types of 2D hud oriented objects
	void AddTextToGUI(GUIText* guiText);
	void AddImageToGUI(GUIImage* guiImg);
	void ToggleEnabled();
private:
	void Draw() override;
	void DrawTexture(glm::vec2 position, GE::Texture* texture);
};

