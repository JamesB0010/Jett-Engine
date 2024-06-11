#include "../Include/Kerosene Renderer/Terrain.h"

GE::Terrain::Terrain(std::string imageAsPath, float heightScaling = 10.0f, float scaling = 100.0f)
{
	//1. Load the heightmap image from a file.
	SDL_Surface* heightMap = IMG_Load(imageAsPath.c_str());

	//2 Get the resolution of the heightmap image as it defines the verticies in the terrain.
	//Each pixel in image corresponds to a 
	//vertex
	int height = heightMap->h;
	int width = heightMap->w;

	//3. Create a pointer variable as a means of accessing the pixels
	//in the image. Pixel colour defines the Y of a vertex
	unsigned char* imageData = (unsigned char*)heightMap->pixels;

	//4. Get the number of channels as need this to work out
	//where red colour is in a specific pixel
	int channels = heightMap->format->BytesPerPixel;

	//5. Vectors for storing the verticies and indicies (of verticies)
	std::vector<GE::Vertex> verticies;
	std::vector<GLuint> indicies;

	//6. These loops create the verticies for the terrain
	//size of terrain is based on heightmap resolution width and height
	float x, y, z, u, v;
	for (int row = 0; row < height; row++) {
		//for each vertex in a row
		for (int column = 0; column < width; ++column) {
			//Find the red pixel for the vertex. (row * width) + column works out
			//the vertex and * channels finds the byte corresponding to red for the vertex. A pixel may be made up of more than one byte for example
			//each channel could a byte so a pixel colour would be three bytes
			unsigned char redPixel = imageData[((row * width) + column) * channels];

			//Calculate the vertex x, y, z based on column and row
			//(x and z value), y is determined from the amount of red
			//no red means height would be 0, all red means height would be maximum
			//note x, y, z are scaled values
			x = (float)column / (float)(width - 1) * scaling;
			z = (float)row / (float)(height - 1) * scaling;
			y = redPixel / 255.0f * heightScaling;

			//calculate the uv based where the vertex is within the 
			//yerrain in terms of its column and row
			//remember uv values are in the range of 0 to 1
			u = (float)column / (float)(width - 1);
			v = (float)(height - row) / (float)(height - 1);

			//Add the vertex to the vector. Verctor will be used
			//to create the vertex buffer
			verticies.push_back(Vertex::CreateVertexFromCoordinatesAndUV(glm::vec3(x, y, z), glm::vec2(u, v)));
		}
	}

	//these loops create the indiceis for the index buffer
	unsigned int c, n;
	for (int row = 0; row < height - 1; row++) {
		for (int column = 0; column < width - 1; column++) {
			c = row * width + column;
			n = (row + 1) * width + column;

			indicies.push_back(c);
			indicies.push_back(n);
			indicies.push_back(c + 1);
			indicies.push_back(c + 1);
			indicies.push_back(n);
			indicies.push_back(n + 1);
		}
	}

	//create vertex buffer
	glGenBuffers(1, &this->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), verticies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create the index buffer. Same as creating vertex buffer except
	//type is GL_ELEMENT_ARRAY_BUFFER and use the indicies vector
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	indexCount = indicies.size();

	SDL_FreeSurface(heightMap);
}

GLuint GE::Terrain::getVerticies() const
{
	return this->vertexBufferObject;
}

GLuint GE::Terrain::getIndicies() const
{
	return this->indexBufferObject;
}

GLuint GE::Terrain::getIndexCount() const
{
	return this->indexCount;
}
