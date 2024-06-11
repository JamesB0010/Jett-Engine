#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <SDL_image.h>
#include <vector>
#include "TriangleRenderer.h"

namespace GE {
	class Terrain {
	public:
		Terrain(std::string imageAsPath, float heightScaling, float scaling);

		//This class has accessors as the
		//terrain is hard coded in terms of scale
		//and heightmap file
		//you should modify the class to make 
		//easy to configure at runtime

		//getVerticies works in the same way as the 
		//Model get verticies
		GLuint getVerticies() const;

		//getIndicies gets the index buffer object
		//as need to bind to pipeline in order to
		//render using index buffer
		GLuint getIndicies() const;

		//number of indicies in the index buffer objecy
		GLuint getIndexCount() const;
	private:
		GLuint vertexBufferObject, indexBufferObject, indexCount;
	};
}
