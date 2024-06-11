#pragma once
#include <GL/glew.h>
#include <vector>
#include "Renderer.h"
#include <vector>
class Ocean
{
	float heightWidth = 1000;
	float segments = 300;

	GLuint vertexBuffer, indexBufferObject, uvCoordsLocation;

	std::vector<glm::vec3> verticies;

	std::vector<glm::vec2> uvCoords;

	std::vector<unsigned int> indicies;

public:
	Ocean();

	GLuint getVerticies() const;


	GLuint getIndiciesId() const;

	GLuint getUvCoordsLocation() const;

	const glm::vec3* getVertexData()const;

	int getVertexCount() const;

	const unsigned int* getIndicies() const;

	int getIndexCount() const;

	~Ocean();
};

