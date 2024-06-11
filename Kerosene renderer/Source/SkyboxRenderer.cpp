#pragma once
#include "../Include/Kerosene Renderer/SkyboxRenderer.h"
using namespace GE;
const float SIDE = 1.0f;

CubeVertex cube[36] = {
	// Front face
	CubeVertex(-SIDE,  SIDE, -SIDE),
	CubeVertex(-SIDE, -SIDE, -SIDE),
	CubeVertex(SIDE, -SIDE, -SIDE),

	CubeVertex(SIDE, -SIDE, -SIDE),
	CubeVertex(SIDE,  SIDE, -SIDE),
	CubeVertex(-SIDE, SIDE, -SIDE),

	// Back face
	CubeVertex(-SIDE,  SIDE, SIDE),
	CubeVertex(-SIDE, -SIDE, SIDE),
	CubeVertex(SIDE, -SIDE, SIDE),

	CubeVertex(SIDE, -SIDE, SIDE),
	CubeVertex(SIDE,  SIDE, SIDE),
	CubeVertex(-SIDE, SIDE, SIDE),

	// Left face
	CubeVertex(-SIDE, -SIDE, SIDE),
	CubeVertex(-SIDE,  SIDE, SIDE),
	CubeVertex(-SIDE,  SIDE, -SIDE),

	CubeVertex(-SIDE,  SIDE, -SIDE),
	CubeVertex(-SIDE, -SIDE, -SIDE),
	CubeVertex(-SIDE, -SIDE,  SIDE),

	// Right face
	CubeVertex(SIDE, -SIDE, SIDE),
	CubeVertex(SIDE,  SIDE, SIDE),
	CubeVertex(SIDE,  SIDE, -SIDE),

	CubeVertex(SIDE,  SIDE, -SIDE),
	CubeVertex(SIDE, -SIDE, -SIDE),
	CubeVertex(SIDE, -SIDE,  SIDE),

	// Top face
	CubeVertex(-SIDE, SIDE,  SIDE),
	CubeVertex(SIDE, SIDE,  SIDE),
	CubeVertex(SIDE, SIDE, -SIDE),

	CubeVertex(SIDE,  SIDE, -SIDE),
	CubeVertex(-SIDE, SIDE, -SIDE),
	CubeVertex(-SIDE, SIDE,  SIDE),

	// Bottom face
	CubeVertex(-SIDE, -SIDE,  SIDE),
	CubeVertex(SIDE, -SIDE,  SIDE),
	CubeVertex(SIDE, -SIDE, -SIDE),

	CubeVertex(SIDE, -SIDE, -SIDE),
	CubeVertex(-SIDE, -SIDE, -SIDE),
	CubeVertex(-SIDE, -SIDE,  SIDE),
};

GE::SkyboxRenderer::SkyboxRenderer(std::string front_fname, std::string back_fname, std::string right_fname, std::string left_fname, std::string top_fname, std::string bottom_fname)
{
	std::vector<std::string> filenames;

	filenames.push_back("../GameProject/" + front_fname);
	filenames.push_back("../GameProject/" + back_fname);
	filenames.push_back("../GameProject/" + left_fname);
	filenames.push_back("../GameProject/" + right_fname);
	filenames.push_back("../GameProject/" + top_fname);
	filenames.push_back("../GameProject/" + bottom_fname);

	this->CreateCubemap(filenames);
	this->CreateCubeVBO();
	this->CreateSkyboxProgram();
}

GE::SkyboxRenderer::~SkyboxRenderer()
{
}

void GE::SkyboxRenderer::draw(Camera* cam)
{
	bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);

	glm::mat4 camView = cam->getViewMatrix();
	glm::mat4 projection = cam->getProjectionMatrix();

	//zero out translation as just interested in rotation to make
	//sure skybox is oriented around the origin
	camView[3][0] = 0.0f;
	camView[3][1] = 0.0f;
	camView[3][2] = 0.0f;

	//select the program into the rendering context
	glUseProgram(this->skyboxProgramId);

	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSkybox);

	//set the uniforms in the shader
	glUniformMatrix4fv(this->viewUniformId, 1, GL_FALSE, glm::value_ptr(camView));
	glUniformMatrix4fv(this->projectionUniformId, 1, GL_FALSE, glm::value_ptr(projection));

	//Enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexLocation);

	//define the structure of a vertex for OpenGl to select values from vertex buffer
	//and store in vertexLocation attribute. Note, there is no attribute pointer for uvs
	//as for a skybox the texture co-ordinates are taken from the vertex as using a 
	//cube map
	glVertexAttribPointer(this->vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));

	//select the texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(this->samplerId, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxCubeMapName);

	//draw the model
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));

	//unselect the attribute from the context
	glDisableVertexAttribArray(this->vertexLocation);

	//unselect the program from the context
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (isDepthTestEnable)
		glEnable(GL_DEPTH_TEST);
}

void GE::SkyboxRenderer::destroy()
{
	glDeleteProgram(this->skyboxProgramId);

	glDeleteBuffers(1, &this->vboSkybox);

	glDeleteTextures(1, &this->skyboxCubeMapName);
}

void GE::SkyboxRenderer::CreateCubemap(std::vector<std::string> filenames)
{
	//create a name for the cubemap
	glGenTextures(1, &this->skyboxCubeMapName);

	//select the cubemap as the target to subsequent texture operations
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxCubeMapName);

	//load texture data from files into cubemap
	for (int faceNum = 0; faceNum < 6; faceNum++) {
		//load texture data from file
		SDL_Surface* surfaceImage = IMG_Load(filenames[faceNum].c_str());

		//check it was loaded okay
		if (surfaceImage == nullptr)
			return;

	//Get the format from the SDL Object will be in SDL Images format
		GLenum format = surfaceImage->format->format;

		format = TextureFormatMapper::MapSDLFormatToGLFormat(format);

		//Copy the pixel data from the SDL_Surface object to the OpenGLtexture
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0, format, surfaceImage->w, surfaceImage->h, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

		//Release the surface object and memory associated with it
		//as no longer need it
		SDL_FreeSurface(surfaceImage);
	}

	//configure how the texture will be manipulated when it needs to be reduced or increased (magnified)
	//when rendering onto an object. GL_LINEAR is weighted average of the colours around the texture co-ords
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Clamp the texture edge to deal with inaccuracies. If the xyz co-ords go over
	//under 0/1, then sampler will wrap a texture resulting in artefacting at the edges
	//this sets the cubemap to use clamping to repeat the last line of the texture
	//to deal with innaccuracies
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void GE::SkyboxRenderer::CreateCubeVBO()
{
	//create the vertex buffer object
	glGenBuffers(1, &this->vboSkybox);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSkybox);

	//transfer vertieices to graphics memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//release from pipeline 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GE::SkyboxRenderer::CreateSkyboxProgram()
{
	//the shader code as described in lecutuew
	const GLchar* V_ShaderCode[] = {
		"#version 140\n"
		"in vec3 vertexPos3D;\n"
		"out vec3 texCoord;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main(){\n"
		"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
		"v = projection * view * v\n;"
		"gl_Position = v;\n"
		"texCoord = vertexPos3D;\n"
		"}\n"
	};

	//Fragmant shader source code
	const GLchar* F_ShaderCode[] = {
		"#version 140\n"
		"in vec3 texCoord;\n"
		"uniform samplerCube sampler;\n"
		"out vec4 fragmentColour;\n"
		"void main()\n"
		"{\n"
		"fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);\n"
		"}\n"
	};


	bool result = compileProgram(V_ShaderCode, F_ShaderCode, &this->skyboxProgramId);

	//check result
	if (!result) {
		std::cerr << "Failed to create SkyboxRenderer program. Check console for errors" << std::endl;
		return;
	}

	//Now get a link to the vertexPos3D so we can link the attribute
	//to our verticies when rendering
	this->vertexLocation = glGetAttribLocation(this->skyboxProgramId, "vertexPos3D");

	//check for errors
	if (this->vertexLocation == -1) 
		std::cerr << "Problem getting vertex3DPos" << std::endl;

	//Link the uniforms to the member fields
	this->viewUniformId = glGetUniformLocation(skyboxProgramId, "view");
	this->projectionUniformId = glGetUniformLocation(skyboxProgramId, "projection");
	this->samplerId = glGetUniformLocation(this->skyboxProgramId, "sampler");
	
}
