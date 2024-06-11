#include "../Include/Kerosene Renderer/reflectiveModelRenderer.h"
void reflectiveModelRenderer::Init(const  GLchar* const *vertexShaderCode, const GLchar* const* fragmentShaderCode)
{
	std::cout << "Reflective model renderer" << std::endl;
	GLint vertexShader;

	if (vertexShaderCode == nullptr) {
		const GLchar* vShadeCode[] = { this->defaultVertShader.c_str()};

		vertexShaderCode = vShadeCode;
	};

	GE::TriangleRenderer::ShaderCreationalData vertexShaderCreationalData;
	vertexShaderCreationalData.shaderIdPointer = &vertexShader;
	vertexShaderCreationalData.shaderCode = vertexShaderCode;
	vertexShaderCreationalData.shaderType = GL_VERTEX_SHADER;


	GLint fragmentShader;

	if (fragmentShaderCode == nullptr) {
		const GLchar* fragShadeCode[] = { this->defaultFragShader.c_str() };

		fragmentShaderCode = fragShadeCode;
	};


	GE::TriangleRenderer::ShaderCreationalData fragmentShaderCreationalData;
	fragmentShaderCreationalData.shaderIdPointer = &fragmentShader;
	fragmentShaderCreationalData.shaderCode = fragmentShaderCode;
	fragmentShaderCreationalData.shaderType = GL_FRAGMENT_SHADER;



	try {
		ModelRenderer::CreatePipelineShaders(std::vector<GE::TriangleRenderer::ShaderCreationalData>{vertexShaderCreationalData, fragmentShaderCreationalData});
	}
	catch (GE::ShaderCompilationError err) {
		std::cout << err.what();
		return;
	}

	SendProgramObjectAndVertexBufferToRam(vertexShader, fragmentShader);

	}

void reflectiveModelRenderer::Draw(GE::Camera* cam, GE::Model* model, bool backfaceCullingEnabled)
{
	glUseProgram(this->programId);
	if (backfaceCullingEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	glUseProgram(this->programId);

	glm::mat4 transformationMat = ModelRenderer::CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into the rendering

	this->ModelRenderer::SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender(model->getVbo());

	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, model->getVerticies());

	glActiveTexture(GL_TEXTURE0);
//	std::cout << glGetError() << std::endl;
	glUniform1i(this->skyboxSamplerId, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxCubeMapName);

	//Draw the model
	glDrawArrays(GL_TRIANGLES, 0, model->getNumVerticies());

	CleanUpAfterRender();

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void reflectiveModelRenderer::Draw(GE::Camera* cam, GE::Terrain* terrain)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 transformationMat = ModelRenderer::CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	glUseProgram(this->programId);
	//select the program into the rendering

	this->ModelRenderer::SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender(terrain->getVerticies());


	glActiveTexture(GL_TEXTURE0);
	glUniform1i(this->skyboxSamplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->getIndicies());
	//Draw the model
	glDrawElements(GL_TRIANGLES, terrain->getIndexCount(), GL_UNSIGNED_INT, nullptr);

	CleanUpAfterRender();
}

void reflectiveModelRenderer::Draw(GE::Camera* cam, GE::Skydome* skydome)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::mat4 transformationMat = glm::translate(transformationMat, glm::vec3(cam->getPosX(), cam->getPosY(), cam->getPosZ()));
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	glUseProgram(this->programId);
	//select the program into the rendering

	this->ModelRenderer::SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);
	glUniform3f(this->cameraPosUniformName, cam->getPosX(), cam->getPosY(), cam->getPosZ());

	GetReadyToRender(skydome->getVerticies());


	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skydome->getIndicies());
	//Draw the model
	glDrawElements(GL_TRIANGLES, skydome->getIndexCount(), GL_UNSIGNED_INT, nullptr);

	CleanUpAfterRender();

	glEnable(GL_DEPTH_TEST);
}


void reflectiveModelRenderer::GetReadyToRender(GLuint vbo)
{
	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexPos3DLocation);

	//Define the structure of a vertex for OpenGL to select values from vertex bufer
	//and store in vertexPos2DLocation attribute
	glVertexAttribPointer(this->vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, position.x));

	//enable the attribute to be passed colours from the vertex buffer object
	glEnableVertexAttribArray(this->vertexUVLocation);

	//Define the structure of a vertex for OpenGL to selecr values from vertex buffer
	//and store in vColour attribute
	glVertexAttribPointer(this->vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, uv.x));

	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, normal.x));
}

void reflectiveModelRenderer::CleanUpAfterRender()
{
	//Unselect the attribute from the context
	glDisableVertexAttribArray(this->vertexPos3DLocation);

	glDisableVertexAttribArray(this->vertexUVLocation);

	glDisableVertexAttribArray(2);

	//glDisableVertexAttribArray(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//unselect vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unselect the program from the context
	glUseProgram(0);

	glDisable(GL_CULL_FACE);
}



void reflectiveModelRenderer::SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader)
{
	this->ModelRenderer::CreateExcecutableProgramObject(vertexShader, fragmentShader);

	this->ModelRenderer::RetrieveVertexPos3DGLAttrib();

	this->RetrieveVUVColourGLAttrib();

	this->ModelRenderer::GetUniformIdsForViewProjectionAndTransformMatricies();
	glUseProgram(this->programId);

	std::vector<std::string> filenames{
		"../../Assets/Skybox/SeaSkybox/Right.png",
		"../../Assets/Skybox/SeaSkybox/Left.png",
		"../../Assets/Skybox/SeaSkybox/Top.png",
		"../../Assets/Skybox/SeaSkybox/Bottom.png",
		"../../Assets/Skybox/SeaSkybox/Front.png",
		"../../Assets/Skybox/SeaSkybox/Back.png"
	};

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

		format = GE::TextureFormatMapper::MapSDLFormatToGLFormat(format);

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



	this->skyboxSamplerId = glGetUniformLocation(this->programId, "skySampler");


	//this->GetUniformIdForSampler();

	this->cameraPosUniformName = glGetUniformLocation(programId, "CameraPosition");

	this->ModelRenderer::CreateAndSendVertexBufferToRam();
	glUseProgram(0);
}

void reflectiveModelRenderer::GetUniformIdForSampler()
{
	this->samplerId = glGetUniformLocation(this->programId, "sampler");
}

void reflectiveModelRenderer::RetrieveVUVColourGLAttrib()
{
	this->vertexUVLocation = glGetAttribLocation(this->programId, "vUV");

	if (this->vertexUVLocation == -1)
		std::cerr << "Problem getting vUV" << std::endl;
}


void reflectiveModelRenderer::setTexture(GE::Texture* _tex)
{
	this->texture = _tex;
}
