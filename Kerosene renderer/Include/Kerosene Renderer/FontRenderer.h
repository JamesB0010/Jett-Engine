#pragma once
#include "Texture.h"
#include "../../../SDL2_ttf-2.22.0/include/SDL_ttf.h"
#include <iostream>

class FontRenderCreationError : public std::exception
{
	const char* message;
public:
	FontRenderCreationError(const char* msg) : message(msg) {}
	const char* what() {
		return message;
	}
};

class FontConversionError : public std::exception
{
	const char* message;
public:
	FontConversionError(const char* msg) : message(msg) {}
	const char* what() {
		return message;
	}
};

class FontRenderer
{
	static bool Ready;
	TTF_Font* font = nullptr;
	SDL_Color fontColor = { 255,255,255 };
public:
	FontRenderer(std::string fontPath, int size);
	FontRenderer(TTF_Font* font);
	~FontRenderer();
	static void InitTTF();
	//creates a texture ofbject for the text based on font and size
	GE::Texture* RenderText(std::string text);
	static void ShutDownTTF();

	static bool isReadyToDraw();
};

