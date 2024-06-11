#include "../Include/Kerosene Renderer/OceanModelRenderer.h"

void OceanModelRenderer::Init(Ocean* ocean)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	this->ocean = ocean;

	glBindBuffer(GL_ARRAY_BUFFER, ocean->getVerticies());
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ocean->getVertexCount(), ocean->getVertexData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ocean->getIndiciesId());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * ocean->getIndexCount(), ocean->getIndicies(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	GLint vertexShader;
	GLint fragmentShader;

	//Load shaders from files
	std::string KeroseneRendererDirectory = "../../../Kerosene renderer/";
	std::string v_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/Ocean.vs");
	std::string f_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/Ocean.fs");

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
	}

	this->SendProgramObjectAndVertexBufferToRam(vertexShader, fragmentShader);


	this->timeUniformId = glGetUniformLocation(this->programId, "TimePassed");

	this->viewVecUniformId = glGetUniformLocation(this->programId, "ViewDir");

	this->WaveDirections = glGetUniformLocation(this->programId, "WaveDirections");

	this->samplerId = glGetUniformLocation(this->programId, "U_Sampler");

	glUseProgram(this->programId);

	int numberOfDirections = 32;

	std::unique_ptr<GLfloat[]> directions (new float[numberOfDirections * 2]);

	for (int i = 0; i < numberOfDirections; i += 2) {
		float randX = dist(eng);
		float randY = dist(eng);

		
		glm::vec2 tempDirection = glm::normalize(glm::vec2(randX, randY));

		//std::cout << tempDirection.x << "    " << tempDirection.y << std::endl;

		directions[i] = tempDirection.x;
		directions[i + 1] = tempDirection.y;
	}


	glUniform2fv(this->WaveDirections, 1, &directions[0]);

	glUseProgram(0);

}

void OceanModelRenderer::Draw(GE::Camera* cam, GE::Texture* texture)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(this->programId);

	glBindBuffer(GL_ARRAY_BUFFER, this->ocean->getVerticies());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->ocean->getUvCoordsLocation());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureName());

	glUniform1i(this->samplerId, 0);


	glm::mat4 transformationMat = CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into the rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	glUniform1ui(this->timeUniformId, Time::timeSinceStart);

	glm::mat4 inversed = glm::inverse(viewMat);

	glm::vec3 viewDirection = glm::inverse(viewMat)[3];
	//viewDirection = glm::normalize(viewDirection);
	viewDirection = glm::normalize(cam->getLookDir());

	glUniform3f(this->viewVecUniformId, viewDirection.x, viewDirection.y, viewDirection.z);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ocean->getIndiciesId());
	//set uniforms
	//Draw the model
	glDrawElements(GL_TRIANGLES, this->ocean->getIndexCount(), GL_UNSIGNED_INT, 0);

	this->CleanUpAfterRender();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OceanModelRenderer::SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader)
{
	this->CreateExcecutableProgramObject(vertexShader, fragmentShader);

	this->GetUniformIdsForViewProjectionAndTransformMatricies();
}

void OceanModelRenderer::GetReadyToRender(GLuint vbo)
{
	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexPos3DLocation);

	//Define the structure of a vertex for OpenGL to select values from vertex bufer
	//and store in vertexPos2DLocation attribute
	glVertexAttribPointer(this->vertexPos3DLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
}

void OceanModelRenderer::CleanUpAfterRender()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}
