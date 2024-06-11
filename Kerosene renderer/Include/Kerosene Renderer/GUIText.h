#pragma once
#include <string>
#include "FontRenderer.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <functional>

//font size pair is simply a struct containing a ttf font path and a size 
//it is used later in the fontRenderers unorderedMap 
struct FontSizePair {
	std::string font;
	int fontSize;

	FontSizePair(std::string font, int size) {
		this->font = font;
		this->fontSize = size;
	}

	//equality operator to be used by the map
	bool operator ==(const FontSizePair& other) const {
		return this->font == other.font && this->fontSize == other.fontSize;
	}
};

//creating a template specialisation for hashing a fontsize pair
//this is also used by the fontRenderers map
namespace std {
	template<>
	struct hash<FontSizePair> {
		std::size_t operator()(const FontSizePair& pair) const {
			//combine the hash of font and font size to create a unique hash for font size pair
			std::size_t hash1 = std::hash<std::string>{}(pair.font);
			std::size_t hash2 = std::hash<int>{}(pair.fontSize);
			return hash1 ^ (hash2 << 1); // Example combining hash function
		}
	};
}

class GUIText
{
	static std::unordered_map<FontSizePair, FontRenderer*> fontRenderers;
	FontRenderer* fontRenderer = nullptr;
	GE::Texture* texture = nullptr;
	glm::vec2 pos = glm::vec2(0, 0);
	std::string text;
public:
	GUIText(glm::vec2 position, std::string text, std::string font, int size);
	GE::Texture* getTexture();
	glm::vec2 getPosition();
	void setPosition(glm::vec2 newPosition);
	void offsetPosition(glm::vec2 offset);

	void setText(std::string text);
	~GUIText();
};

