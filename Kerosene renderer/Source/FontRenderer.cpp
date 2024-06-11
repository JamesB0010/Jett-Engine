#include "../Include/Kerosene Renderer/FontRenderer.h"
bool FontRenderer::Ready = false;

FontRenderer::FontRenderer(std::string fontPath, int size) {
	this->font = TTF_OpenFont(fontPath.c_str(), size);

	if (this -> font == nullptr)
	{
		throw FontRenderCreationError("Error creating the font");
		return;
	}
}

FontRenderer::FontRenderer(TTF_Font* font)
{
	this->font = font;
}

void FontRenderer::InitTTF() {
	TTF_Init();
}

GE::Texture* FontRenderer::RenderText(std::string text) {
	//render text into texture
	SDL_Surface* surfaceImage = TTF_RenderText_Solid(this->font, text.c_str(), this->fontColor);

	//convert texture into opengl texture form
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0);

	if (converted != nullptr) {
		SDL_FreeSurface(surfaceImage);

		surfaceImage = converted;
	}
	else {
		std::string error("Error converting font " + text + "!" + SDL_GetError());
		throw FontConversionError(error.c_str());
		return nullptr;
	}

	//get the dimentions needed for opengl
	int width = surfaceImage->w;
	int height = surfaceImage->h;

	Uint32 format = GL_RGBA;

	//flip surface pixels
	//first check if surface needs lockking
	if (SDL_MUSTLOCK(surfaceImage)) {
		SDL_LockSurface(surfaceImage);
	}

	//calculate number of bytes in a row
	int bytesPerRow = width * surfaceImage->format->BytesPerPixel;

	//create a temporary buffer for a row of pixels
	char* temp = new char[bytesPerRow];

	//exchange rows of pixels from top to bottom
	for (int y = 0; y < height / 2; ++y) {
		//Copy bottom row into temp
		memcpy(temp, ((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), bytesPerRow);

		//copy top row to bottom row
		memcpy(((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), (char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow);

		//copy temp to top row
		memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow);
	}

	//delete temp buffer 
	delete[] temp;


	GLuint textureName;

	//create a texture name for the texture
	glGenTextures(1, &textureName);

	//select created texture for subsequent texture operations
	//to setup the texture for opengl
	glBindTexture(GL_TEXTURE_2D, textureName);

	//copy the pixel data from the SDL_Surface object to the OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

	//configure how the texture will be manipulated when it needs to be 
	//reduced or increased (magnified) when rendering onto an object
	//GL_LINEAR is weighted average of the colours around the texture co-ords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Release the suface object and memory associated with it 
	SDL_FreeSurface(surfaceImage);

	//return the texture object

	return GE::Texture::CreateTextureFromGLTextureName(textureName, glm::vec2(width, height));
}

void FontRenderer::ShutDownTTF() {
	TTF_Quit();
}

bool FontRenderer::isReadyToDraw() {
	return FontRenderer::Ready;
}

FontRenderer::~FontRenderer()
{
	TTF_CloseFont(this->font);
}
