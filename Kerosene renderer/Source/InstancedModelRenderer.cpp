#include "../Include/Kerosene Renderer/InstancedModelRenderer.h"

void InstancedModelRenderer::Init()
{
	GLint vertexShader;
	GLint fragmentShader;

	//Load shaders from files
	std::string KeroseneRendererDirectory = "../../../Kerosene renderer/";
	std::string v_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/InstancedMesh.vs");
	std::string f_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/default.fs");

	const GLchar* v_source_array[] = { v_shader_source.c_str() };
	const GLchar* f_source_array[] = { f_shader_source.c_str() };

	ShaderCreationalData vertexShaderCreationalData;
	vertexShaderCreationalData.shaderIdPointer = &vertexShader;
	vertexShaderCreationalData.shaderCode = v_source_array;
	vertexShaderCreationalData.shaderType = GL_VERTEX_SHADER;


	ShaderCreationalData fragShaderCreationalData;
	fragShaderCreationalData.shaderIdPointer = &fragmentShader;
	fragShaderCreationalData.shaderCode = f_source_array;
	fragShaderCreationalData.shaderType = GL_FRAGMENT_SHADER;
	
	try {
		this->CreatePipelineShaders(std::vector<ShaderCreationalData>{vertexShaderCreationalData, fragShaderCreationalData});
	}
	catch (GE::ShaderCompilationError err) {
		std::cerr << err.what() << std::endl;
		return;
	}

	this->SendProgramObjectAndVertexBufferToRam(vertexShader, fragmentShader);

}

void InstancedModelRenderer::Draw(GE::Camera* cam, InstancedModel instancedModel)
{
	glEnable(GL_CULL_FACE);
	glUseProgram(this->programId);

	glm::mat4 transformationMat = CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat, instancedModel.translationBuffer, instancedModel.numberOfInstances);

	GetReadyToRender(instancedModel.model->getVbo());

	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, instancedModel.model->getVerticies());

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(this->samplerId, 0);

	glBindTexture(GL_TEXTURE_2D, this->texture->getTextureName());

	//Draw the model
	glDrawArraysInstanced(GL_TRIANGLES, 0, instancedModel.model->getNumVerticies(), instancedModel.numberOfInstances);

	CleanUpAfterRender();
}

void InstancedModelRenderer::RetrieveTranslationOffsets()
{
	this->translationOffsetsLocation = glGetUniformLocation(this->programId, "translateOffsets");
}


void InstancedModelRenderer::SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader)
{
	this->CreateExcecutableProgramObject(vertexShader, fragmentShader);

	this->RetrieveVertexPos3DGLAttrib();

	this->RetrieveVUVColourGLAttrib();

	this->RetrieveTranslationOffsets();

	this->GetUniformIdsForViewProjectionAndTransformMatricies();

	this->GetUniformIdForSampler();

	this->CreateAndSendVertexBufferToRam();
}

void InstancedModelRenderer::SetShaderUniforms(glm::mat4* transformationMat, glm::mat4* viewMat, glm::mat4* projectionMat, glm::vec3* translateOffsets, int numberOfInstances)
{
	TriangleRenderer::SetShaderUniforms(transformationMat, viewMat, projectionMat);

	GLfloat* offsets = new GLfloat[numberOfInstances * 3];

	//convert array of vec3's to an array of floats
	for (int i = 0; i < numberOfInstances; ++i) {
		offsets[i * 3] = translateOffsets[i].x;
		offsets[(i * 3) + 1] = translateOffsets[i].z;
		offsets[(i * 3) + 2] = translateOffsets[i].y;
	}

	glUniform3fv(this->translationOffsetsLocation, numberOfInstances, offsets);

}

