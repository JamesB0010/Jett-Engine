#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace GE {


	//abstract base class representing a format mapper
	class I_TextureFormatMapper {
	public:
		virtual int getGLFormat() const = 0;
		virtual ~I_TextureFormatMapper() {};
	};

	//below are the different mappers
	class RGBA32Format : public I_TextureFormatMapper {
	public:
		int getGLFormat() const override {
			return GL_RGBA;
		};
	};

	class RGB24Format : public I_TextureFormatMapper {
	public:
		int getGLFormat() const override {
			return GL_RGB;
		};
	};

	class DefaultFormat : public I_TextureFormatMapper {
	public:
		int getGLFormat() const override {
			return GL_RGB;
		}
	};

	//a factory class for creating the mappers
	class TextureFormatMapper {
		static std::unordered_map<Uint32, I_TextureFormatMapper*> strategies;

	public:
		static void init() {
			strategies.insert(std::pair<Uint32, I_TextureFormatMapper*>(SDL_PIXELFORMAT_RGB24, new  RGB24Format));
			strategies.insert(std::pair<Uint32, I_TextureFormatMapper*>(SDL_PIXELFORMAT_RGBA32, new  RGBA32Format));
		}
		static int MapSDLFormatToGLFormat(Uint32 sdlFormat) {
			auto it = strategies.find(sdlFormat);
			if (it != strategies.end()) {
				return strategies[sdlFormat]->getGLFormat();
			}
			else {
				GE::DefaultFormat defaultFormat;
				return defaultFormat.getGLFormat();
			}

			
		}
	};


	class Texture
	{
		int width, height;

		Uint32 format;

		GLuint textureName;
	public:
		Texture(std::string filename);

		Texture();

		static Texture* CreateTextureFromGLTextureName(GLuint textureName, glm::vec2 widthHeight);

		int getWidth();

		int getHeight();

		Uint32 getFormat();

		GLuint getTextureName();

		void setWidth(int width);

		void setHeight(int height);

		virtual ~Texture();

	private:
		void LoadTextureIntoOpenGL(std::string filename);
		void SetTextureDimentionsAndFormatFromSurface(SDL_Surface* surfaceImage);
		void SetTextureDimentionsFromSurface(SDL_Surface* surfaceImage);
	};
};