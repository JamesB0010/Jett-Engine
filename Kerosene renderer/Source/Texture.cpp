#include "../Include/Kerosene Renderer/Texture.h"

std::unordered_map<Uint32, GE::I_TextureFormatMapper*> GE::TextureFormatMapper::strategies = std::unordered_map<Uint32, I_TextureFormatMapper*>{
};

GE::Texture::Texture(std::string filename)
{
	this->width = 0;
	this->height = 0;

	this->textureName = 0;

	this->LoadTextureIntoOpenGL(filename);
}

GE::Texture::Texture()
{
}

GE::Texture* GE::Texture::CreateTextureFromGLTextureName(GLuint textureName, glm::vec2 widthHeight)
{
	Texture* texture = new Texture();
	texture->textureName = textureName;
	texture->format = SDL_PIXELFORMAT_RGBA32;
	texture->width = widthHeight.x;
	texture->height = widthHeight.y;
	return texture;
}

int GE::Texture::getWidth()
{
	return this->width;
}

int GE::Texture::getHeight()
{
	return this->height;
}

Uint32 GE::Texture::getFormat()
{
	return this->format;
}

GLuint GE::Texture::getTextureName()
{
	return this->textureName;
}

void GE::Texture::setWidth(int width)
{
	this->width = width;
}

void GE::Texture::setHeight(int height)
{
	this->height = height;
}

GE::Texture::~Texture()
{
	glDeleteTextures(1, &this->textureName);
}

void GE::Texture::LoadTextureIntoOpenGL(std::string filename)
{
	//Load texture data from file
	SDL_Surface* surfaceImage = IMG_Load(filename.c_str());

	if (surfaceImage == nullptr)
		return;

	SetTextureDimentionsAndFormatFromSurface(surfaceImage);

	this->format = GE::TextureFormatMapper::MapSDLFormatToGLFormat(this->format);

	//Create a texture name for the texture
	glGenTextures(1, &this->textureName);

	//Select created texture for subsequent texture operations
	//to setup the texture for OpenGl
	glBindTexture(GL_TEXTURE_2D, this->textureName);

	// copy the pixel data from the SDL_Surface object to the OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

	//configure how the texture will be manipulated when it needs to be
	//reduced or increased (magnified) when rendering onto an object
	//GL_LINEAR is weighted average of the colours around the texture co-ords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	//release the suface object and memory associated with it
	//as no longer need it
	SDL_FreeSurface(surfaceImage);
}

void GE::Texture::SetTextureDimentionsAndFormatFromSurface(SDL_Surface* surfaceImage)
{
	SetTextureDimentionsFromSurface(surfaceImage);

	//Get the format from the SDL object, will be in SDL Image's format
	this->format = surfaceImage->format->format;
}

void GE::Texture::SetTextureDimentionsFromSurface(SDL_Surface* surfaceImage)
{
	this->width = surfaceImage->w;
	this->height = surfaceImage->h;
}
