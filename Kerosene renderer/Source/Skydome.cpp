#include "../Include/Kerosene Renderer/Skydome.h"

namespace GE {
	Skydome::Skydome() {
		// 1. Define the dome in terms of resolution
		// and size (radius).  Note, some of these
		// constants would be better as parameters
		// to the skydome constructor to configure
		// skydome at runtime, for example

		// Used to determine extent of dome in terms
		// of 90 degrees (radians)
		constexpr float PI_Div2 = M_PI / 2.0f;


		// Define extents of skydome in terms of numbers
		// of vertices horizontally and vertically
		constexpr int horizontalResolution = 16;
		constexpr int verticalResolution = 16;

		// Radius for sphere
		constexpr int radius = 10;

		// Store the generated vertices
		std::vector<Vertex> verticies;

		// Angle between each horizontal vertex in radians
		// Angles in radians are expressed in terms of PI
		// This code makes use of math.h 
		constexpr float azimuthStep = 2.0f * M_PI / horizontalResolution;


		// Texture extent.  This would be changed depending
		// on the skydome texture.  For example, some textures
		// are meant to be wrapped on a sphere, not a dome.
		// If you use a sphere percentage of 1.0, then the 
		// bottom part of the texture appears at the based 
		// of the dome.  In this case, change texturePercentage
		// to 0.5f so v ranges from 0 to 0.5 and only
		// top half of texture is shown
		constexpr float texturePercentage = 1.0f;


		// Determines the extent of the dome in terms of a
		// percentage of PIDIV2.  1.0 means 100% of PIDIV2
		// or 90 degrees.  2.0 means 200% of PIDIV2 which
		// results in a sphere
		constexpr float spherePercentage = 1.0f;


		// Starting point for vertices.  Vertices start from top
		float elevation = PI_Div2;


		// Calculate the vertical step between vertices
		// based 90 degree * sphere percentage
		constexpr float elevationStep = spherePercentage * PI_Div2 / verticalResolution;


		// Steps horizontally and vertically across uv range 0 to 1
		constexpr float uStep = 1.0f / horizontalResolution;
		constexpr float vStep = 1.0f / verticalResolution;

		// Starting point for v texture co-ord.  Bottom of texture
		float v = 0.0f;

		// 2. Determine the vertices horizontally for each elevation
		// from the top to bottom.

		for (int vert = 0; vert < verticalResolution + 1; ++vert) {
			// Calculate horizontal length for elevation, 0 at top to radius at 0 degrees
			float length = radius * cos(elevation);
			// Calculate y position
			float y = radius * sin(elevation);

			float u = 0.0f;
			float angle = 0;
			// Calculate horizontal vertices for elevation
			for (int horiz = 0; horiz < horizontalResolution + 1; ++horiz) {
				// Calculate xz position of vertex based on length
				// and angle horizontally
				float x = length * sin(angle);
				float z = length * cos(angle);


				// Add new vertex
				verticies.push_back(Vertex::CreateVertexFromCoordinatesAndUV(
					glm::vec3(x, y, z),
					glm::vec2(u, v * texturePercentage)
					)
				);

				angle += azimuthStep;

				u += uStep;

			}

			elevation -= elevationStep;
			v += vStep;
		}
// 3. Determine the indices

		std::vector<int> indicies;


// This code generates indices in quads, as two triangles,
// from top of dome down.  Quads at the top and bottom
// appear as triangles
// 
//	(offset + horiz + 1)                           ._   . (offset + horiz)
//                                                 | // |
//  (offset + (horizResolution + 1) + horiz + 1)   .   -. (offset + horizResolution + (horiz + 1))

		for (int vert = 0; vert < verticalResolution; ++vert) {
			int offset = vert * (horizontalResolution + 1);
			for (int horiz = 0; horiz < horizontalResolution; ++horiz) {
		// Left triangle of quad
				indicies.push_back(offset + horiz);
				indicies.push_back(offset + horiz + 1);
				indicies.push_back(offset + (horizontalResolution + 1) + (horiz + 1));

		// Right triangle
				indicies.push_back(offset + (horizontalResolution + 1) + (horiz + 1));
				indicies.push_back(offset + horizontalResolution + (horiz + 1));
				indicies.push_back(offset + horiz);



			}
		}
		// 4 .Create the vertex buffer.  Same as creating vertex buffer for model
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), verticies.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// 5. Create the index buffer.  Same way as creating a vertex buffer except
		// type is GL_ELEMENT_ARRAY_BUFFER and use the indices vector
		glGenBuffers(1, &this->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		// 6. Store the number of indices as need this for rendering
		// the terrain
		this->indexCount = indicies.size();
	}
	GLuint Skydome::getVerticies() const
	{
		return this->vbo;
	}
	GLuint Skydome::getIndicies() const
	{
		return this->ibo;
	}
	GLuint Skydome::getIndexCount() const
	{
		return this->indexCount;
	}
}


