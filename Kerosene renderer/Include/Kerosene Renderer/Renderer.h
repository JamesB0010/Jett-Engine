#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <glm/glm.hpp>
#include "Camera.h"

namespace GE {

		class ShaderCompilationError :
			public std::exception
		{
			const char* message;
		public:
			ShaderCompilationError(const char* msg) : message(msg) {}
			const char* what() {
				return message;
			}
		};


	struct Vertex {
		glm::vec3 position;

		//clour
		glm::vec2 uv;

		glm::vec3 normal = glm::vec3(0,1,0);

		//Constructors
		Vertex() {
			this->position.x = this->position.y = this->position.z = 0.0f;
			this->uv = glm::vec2();
		}
		static Vertex CreateVertexFromCoordinatesAndUVAndNormal(glm::vec3 position, glm::vec2 uvCoord, glm::vec3 normal) {
			using namespace std;
			Vertex vertex = Vertex::CreateVertexFromCoordinatesAndUV(position, uvCoord);
			vertex.normal = normal;
			return vertex;
		}

		static Vertex CreateVertexFromCoordinatesAndUV(glm::vec3 position, glm::vec2 colour) {
			using namespace std;
			Vertex vertex = Vertex::CreateVertexFromCoordinates(position);
			vertex.uv = colour;
			return vertex;
		};
		static Vertex CreateVertexFromCoordinates(glm::vec3 position) {
			using namespace std;
			Vertex vertex;
			vertex.position = position;
			return vertex;
		}

	private:
		Vertex(glm::vec3 position)
			: position(position) {
			this->uv.x = this->uv.y = 0.0f;
		}
	};

class Renderer
{
protected:
	//this stores the program objcect that contains the shaders
	GLuint programId;

	//this stores the attribute to select into the pipeline
	//to link the trangle verticies to the pipeline
	GLint vertexPos3DLocation;

	GLint normalLocation;

	//this stores the trangle vertex buffer object containing the verticies
	//transferred from this code to the graphics memory
	GLuint vboTriangle;
public:
	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Draw(Camera* cam) = 0;

	virtual void Destroy() = 0;

	void DisplayShaderCompilerError(GLuint shaderId);

	GLint GetLogMessageLengthByShaderId(GLuint id);

	void OutputShaderCompilationErrorMessage(const GLint& messageLength, const GLuint& shaderId);

	GLchar* GetShaderCompilationErrorMessage(const GLint& messageLength, const GLuint& shaderId);

	void PrintThenReleaseShaderCompilationErrorMessage(GLchar* Msg);
};

}

