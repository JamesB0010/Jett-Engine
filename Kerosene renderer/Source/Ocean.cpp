#include "../Include/Kerosene Renderer/Ocean.h"


Ocean::Ocean()
{
	glGenBuffers(1, &this->uvCoordsLocation);

	glGenBuffers(1, &this->vertexBuffer);
	//generate verticies

	float triangleSide = heightWidth / this->segments;
	for(int row = 0; row < this->segments + 1; row++){
		for (int col = 0; col < this->segments + 1; col++) {
			this->verticies.push_back(glm::vec3(
				col * triangleSide,
				0.0f,
				row * -triangleSide
			));

			this->uvCoords.push_back(glm::vec2((col * triangleSide) / 1000, (row * triangleSide) / 1000));
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->uvCoordsLocation);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->uvCoords.size(), this->uvCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &this->indexBufferObject);
	//generate indicies

	//when we loop through our verticies which are quads we want to split them into tris

	for (int row = 0; row < segments; ++row) {
		for (int col = 0; col < segments; ++col) {
			int index = row * (segments + 1) + col;

			//top triangle
			this->indicies.push_back(index);
			this->indicies.push_back(index + (segments + 1) + 1);
			this->indicies.push_back(index + (segments + 1));

			//bottom triangle
			this->indicies.push_back(index);
			this->indicies.push_back(index + 1);
			this->indicies.push_back(index + (segments + 1) + 1);
		}
	}

}

GLuint Ocean::getVerticies() const
{
	return this->vertexBuffer;
}

GLuint Ocean::getIndiciesId() const
{
	return this->indexBufferObject;
}

GLuint Ocean::getUvCoordsLocation() const
{
	return this->uvCoordsLocation;
}

const glm::vec3* Ocean::getVertexData() const
{
	return this->verticies.data();
}

int Ocean::getVertexCount() const
{
	return this->verticies.size();
}

const unsigned int* Ocean::getIndicies() const
{
	return this->indicies.data();
}

int Ocean::getIndexCount() const
{
	return this->indicies.size();
}

Ocean::~Ocean()
{
}
