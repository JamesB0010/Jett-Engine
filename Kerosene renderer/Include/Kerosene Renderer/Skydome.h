#pragma once
#include "Model.h"
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "../../../Engine/Include/Jett Engine/GameComponent.h"


namespace GE {
	class Skydome
	{
	public:
		Skydome();


		GLuint getVerticies() const;

		GLuint getIndicies() const;

		GLuint getIndexCount() const;

	private:
		GLuint vbo, ibo, indexCount;
	};
}


