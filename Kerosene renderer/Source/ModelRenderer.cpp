#include "../Include/Kerosene Renderer/ModelRenderer.h"
void ModelRenderer::Init(const  GLchar* const *vertexShaderCode, const GLchar* const* fragmentShaderCode)
{
	GLint vertexShader;

	const GLchar* vShadeCode[1];
	if (vertexShaderCode == nullptr) {
		vShadeCode[0] = { this->defaultVertShader.c_str()};

		vertexShaderCode = vShadeCode;
	};

	ShaderCreationalData vertexShaderCreationalData;
	vertexShaderCreationalData.shaderIdPointer = &vertexShader;
	vertexShaderCreationalData.shaderCode = vertexShaderCode;
	vertexShaderCreationalData.shaderType = GL_VERTEX_SHADER;


	GLint fragmentShader;

	const GLchar* fragShadeCode[1];
	if (fragmentShaderCode == nullptr) {
		fragShadeCode[0] = { this->defaultFragShader.c_str() };

		fragmentShaderCode = fragShadeCode;
	};


	ShaderCreationalData fragmentShaderCreationalData;
	fragmentShaderCreationalData.shaderIdPointer = &fragmentShader;
	fragmentShaderCreationalData.shaderCode = fragmentShaderCode;
	fragmentShaderCreationalData.shaderType = GL_FRAGMENT_SHADER;



	try {
		this->CreatePipelineShaders(std::vector<ShaderCreationalData>{vertexShaderCreationalData, fragmentShaderCreationalData});
	}
	catch (GE::ShaderCompilationError err) {
		std::cout << err.what();
		return;
	}

	SendProgramObjectAndVertexBufferToRam(vertexShader, fragmentShader);
}

void ModelRenderer::Draw(GE::Camera* cam, GE::Model* model, bool backfaceCullingEnabled)
{
	if (backfaceCullingEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	glUseProgram(this->programId);

	glm::mat4 transformationMat = CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into the rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender(model->getVbo());

	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, model->getVerticies());

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	//Draw the model
	glDrawArrays(GL_TRIANGLES, 0, model->getNumVerticies());

	CleanUpAfterRender();
}


void ModelRenderer::Draw(GE::Camera* cam, GE::Terrain* terrain)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 transformationMat = CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	glUseProgram(this->programId);
	//select the program into the rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender(terrain->getVerticies());


	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->getIndicies());
	//Draw the model
	glDrawElements(GL_TRIANGLES, terrain->getIndexCount(), GL_UNSIGNED_INT, nullptr);

	CleanUpAfterRender();
}

void ModelRenderer::Draw(GE::Camera* cam, GE::Skydome* skydome)
{
//	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glUseProgram(this->programId);


	glm::mat4 transformationMat = glm::mat4(1.0f);
	transformationMat *= glm::translate(transformationMat,cam->getPos());
	transformationMat *= glm::scale(transformationMat, glm::vec3(this->scaleX, this->scaleY, this->scaleZ));
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into the rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender(skydome->getVerticies());

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());


	//select the vertex buffer object into the context


	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skydome->getIndicies());
	//Draw the model

	glDrawElements(GL_TRIANGLES, skydome->getIndexCount(), GL_UNSIGNED_INT, nullptr);
	CleanUpAfterRender();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}


void ModelRenderer::GetReadyToRender(GLuint vbo)
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

void ModelRenderer::CleanUpAfterRender()
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



void ModelRenderer::SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader)
{
	this->CreateExcecutableProgramObject(vertexShader, fragmentShader);

	this->RetrieveVertexPos3DGLAttrib();

	this->RetrieveVUVColourGLAttrib();

	this->GetUniformIdsForViewProjectionAndTransformMatricies();

	this->GetUniformIdForSampler();

	this->cameraPosUniformName = glGetUniformLocation(programId, "CameraPosition");

	this->CreateAndSendVertexBufferToRam();
}

void ModelRenderer::GetUniformIdForSampler()
{
	this->samplerId = glGetUniformLocation(this->programId, "sampler");
}

void ModelRenderer::RetrieveVUVColourGLAttrib()
{
	this->vertexUVLocation = glGetAttribLocation(this->programId, "vUV");

	if (this->vertexUVLocation == -1)
		std::cerr << "Problem getting vUV" << std::endl;
}


void ModelRenderer::setTexture(GE::Texture* _tex)
{
	this->texture = _tex;
}