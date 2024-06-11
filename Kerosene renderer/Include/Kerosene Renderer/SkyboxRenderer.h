#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include "Camera.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
#include "CubeVertex.h"

namespace GE {
	class SkyboxRenderer {
	public:
		SkyboxRenderer(std::string front_fname, std::string back_fname, std::string right_fname, std::string left_fname, std::string top_fname, std::string bottom_fname);

		~SkyboxRenderer();

		void draw(Camera* cam);

		void destroy();

		GLuint getskyboxSamplerId() {
			return this->samplerId;
		}

		GLuint getSkyboxCubeMapName() {
			return this->skyboxCubeMapName;
		}

	private:
		void CreateCubemap(std::vector<std::string> filenames);

		void CreateCubeVBO();

		void CreateSkyboxProgram();

	private:
		GLuint skyboxCubeMapName;

		//member fields
		//This member stores the program object that contains the shaders
		GLuint skyboxProgramId;

		//this member stores the attribute to select into the pipeline 
		//to link the triangle verticies to the pipeline
		GLint vertexLocation;

		//this member stores the triangle vertex buffer object containing the verticies
		//transferred from this code to the graphics memory
		GLuint vboSkybox;

		//GLSL uniform variables for the transformation, view and projection matricies
		GLuint viewUniformId;
		GLint projectionUniformId;
		GLuint samplerId;
	};
}