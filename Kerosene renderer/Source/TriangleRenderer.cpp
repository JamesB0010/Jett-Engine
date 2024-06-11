#include "../Include/Kerosene Renderer/TriangleRenderer.h"

namespace GE {
	Vertex triangleVerticies[] = {
		Vertex::CreateVertexFromCoordinatesAndUV(glm::vec3( - 1.0f, 0.0f, 0.0f), glm::vec4(0.254f, 1.0f, 0.120f, 1.0f)),
		Vertex::CreateVertexFromCoordinatesAndUV(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.11f, 0.04f, 1.0f)),
		Vertex::CreateVertexFromCoordinatesAndUV(glm::vec3(0.0f, 1.0f, 0.0f),glm::vec4(0.90f, 1.0f, 1.0f, 1.0f))
	};
}

GE::TriangleRenderer::TriangleRenderer()
{
	this->InitializePosRotAndScaleAttributes();
}

void GE::TriangleRenderer::InitializePosRotAndScaleAttributes() {
	this->posX = this->posY = this->posZ = 0.0f;
	this->rotX = this->rotY = this->rotZ = 0.0f;
	this->scaleX = this->scaleY = this->scaleZ = 1.0f;
}

GE::TriangleRenderer::~TriangleRenderer()
{
}

//creates and compiles the shaders, creates the project and links it,
//and creates the vertex buffer object. Could be argued that method
//is not too cohesive
void GE::TriangleRenderer::Init()
{
	GLint vertexShader;

	const GLchar* V_ShaderCode[] = {
		"#version 140\n"
		"in vec3 vertexPos3D;\n"
		"in vec4 vColour; \n"
		"out vec4 fColour; \n"
		"uniform mat4 viewMat; \n"
		"uniform mat4 projMat; \n"
		"uniform mat4 transformMat; \n"
		"void main(){\n"
		"gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D.x, vertexPos3D.y, vertexPos3D.z, 1);\n"
		"fColour = vColour; \n"
		"}\n" };

	ShaderCreationalData vertexShaderCreationalData;
	vertexShaderCreationalData.shaderIdPointer = &vertexShader;
	vertexShaderCreationalData.shaderCode = V_ShaderCode;
	vertexShaderCreationalData.shaderType = GL_VERTEX_SHADER;


	GLint fragmentShader;

	const GLchar* F_ShaderCode[] = {
		"#version 140\n"
		"in vec4 fColour;\n"
		"out vec4 fragmentColour;\n"
		"void main()\n"
		"{\n"
		"fragmentColour = fColour;\n"
		"}\n"
	};

	ShaderCreationalData fragmentShaderCreationalData;
	fragmentShaderCreationalData.shaderIdPointer = &fragmentShader;
	fragmentShaderCreationalData.shaderCode = F_ShaderCode;
	fragmentShaderCreationalData.shaderType = GL_FRAGMENT_SHADER;

	

	try {
		this->CreatePipelineShaders(std::vector<ShaderCreationalData>{vertexShaderCreationalData, fragmentShaderCreationalData});
	}
	catch (ShaderCompilationError err) {
		std::cout << err.what();
		return;
	}

	SendProgramObjectAndVertexBufferToRam(vertexShader, fragmentShader);
}

float GE::TriangleRenderer::getPosX() const
{
	return this->posX;
}

float GE::TriangleRenderer::getPosY() const
{
	return this->posY;
}

float GE::TriangleRenderer::getPosZ() const
{
	return this->posZ;
}

float GE::TriangleRenderer::getRotX() const
{
	return this->rotX;
}

float GE::TriangleRenderer::getRotY() const
{
	return this->rotY;
}

float GE::TriangleRenderer::getRotZ() const
{
	return this->rotZ;
}

float GE::TriangleRenderer::getScaleX() const
{
	return this->scaleX;
}

float GE::TriangleRenderer::getScaleY() const
{
	return this->scaleY;
}

float GE::TriangleRenderer::getScaleZ() const
{
	return this->scaleZ;
}

void GE::TriangleRenderer::setPos(float x, float y, float z)
{
	this->posX = x;
	this->posY = y;
	this->posZ = z;
}

void GE::TriangleRenderer::setRotation(float rx, float ry, float rz)
{
	this->rotX = rx;
	this->rotY = ry;
	this->rotZ = rz;
}

void GE::TriangleRenderer::setScale(float sx, float sy, float sz)
{
	this->scaleX = sx;
	this->scaleY = sy;
	this->scaleZ = sz;
}

void GE::TriangleRenderer::CreatePipelineShaders(std::vector<ShaderCreationalData> shaderCreationalDatas)
{
	for (int i = 0; i < shaderCreationalDatas.size(); ++i) {
		*shaderCreationalDatas[i].shaderIdPointer = this->CreateShader(shaderCreationalDatas[i].shaderType, shaderCreationalDatas[i].shaderCode);
	}
}




GLint GE::TriangleRenderer::CreateShader(int shaderType, const GLchar* const * shaderCode)
{
	GLuint shader = glCreateShader(shaderType);


	//copy the source to OpenGl ready for compilation
	glShaderSource(shader, 1, shaderCode, NULL);

	glCompileShader(shader);

	
	if (this->CheckIfShaderCompiledOk(shader) != GL_TRUE)
		ShaderNotCompiledOkay(shader);
	return shader;
}



const GLint GE::TriangleRenderer::CheckIfShaderCompiledOk(GLint shader)
{
	GLint isShaderCompiledOk = GL_FALSE;

	//Get the compile status from OpenGl
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiledOk);

	return isShaderCompiledOk;
}

void GE::TriangleRenderer::ShaderNotCompiledOkay(const GLuint& shader)
{
	std::cerr << "Unable to compile shader" << std::endl;

	this->DisplayShaderCompilerError(shader);

	const char* mychar = { "shader failed to compile" };
	throw ShaderCompilationError(mychar);
}

void GE::TriangleRenderer::SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader)
{
	this->CreateExcecutableProgramObject(vertexShader, fragmentShader);

	this->RetrieveVertexPos3DGLAttrib();

	this->RetrieveVColourGLAttrib();

	this->GetUniformIdsForViewProjectionAndTransformMatricies();

	this->CreateAndSendVertexBufferToRam();
}


void GE::TriangleRenderer::CreateExcecutableProgramObject(const GLint& vertexShader, const GLint& fragmentShader)
{
	//create the program object
	this->programId = glCreateProgram();

	//attach shaders to the program object
	glAttachShader(this->programId, vertexShader);
	glAttachShader(this->programId, fragmentShader);

	//now link the program to create an excecutable program we 
	//use to render the object
	//program excecutable will exist in graphics memory
	glLinkProgram(this->programId);

	//check for linking errors
	GLint isProgramLinked = GL_FALSE;
	glGetProgramiv(this->programId, GL_LINK_STATUS, &isProgramLinked);
	if (isProgramLinked != GL_TRUE)
		std::cerr << "Failed to link program" << std::endl;
}
void GE::TriangleRenderer::RetrieveVertexPos3DGLAttrib()
{
	//now get a link to the vertexPos2D so we can link the attribute
	//to our verticies when rendering
	vertexPos3DLocation = glGetAttribLocation(this->programId, "vertexPos3D");

	//check for errors
	if (vertexPos3DLocation == -1)
		std::cerr << "Problem getting vertex3DPos" << std::endl;
}

void GE::TriangleRenderer::RetrieveVColourGLAttrib()
{
	this->vertexColorLocation = glGetAttribLocation(this->programId, "vColour");

	if (this->vertexColorLocation == -1)
		std::cerr << "Problem getting vColour" << std::endl;
}

void GE::TriangleRenderer::GetUniformIdsForViewProjectionAndTransformMatricies() {
	//Get uniform id in shader so C++ program can send data to it
	this->viewUniformId = glGetUniformLocation(this->programId, "viewMat");
	this->projectionUniformId = glGetUniformLocation(this->programId, "projMat");
	//Transformation matrix uniform
	this->transformuniformId = glGetUniformLocation(this->programId, "transformMat");
}

void GE::TriangleRenderer::CreateAndSendVertexBufferToRam()
{
	//create the vertex buffer object
	glGenBuffers(1, &this->vboTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboTriangle);

	//Transfer verticies to graphics memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerticies), triangleVerticies, GL_STATIC_DRAW);

	//tidy up after setting up buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void GE::TriangleRenderer::Update()
{
}

void GE::TriangleRenderer::Draw(Camera* cam)
{	
	glUseProgram(this->programId);

	glm::mat4 transformationMat = CalculateTransformationMatrixForThisObject();
	glm::mat4 viewMat = cam->getViewMatrix();
	glm::mat4 projectionMat = cam->getProjectionMatrix();

	//select the program into the rendering

	this->SetShaderUniforms(&transformationMat, &viewMat, &projectionMat);

	GetReadyToRender();

	//Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleVerticies) / sizeof(GE::Vertex));

	CleanUpAfterRender();


}

glm::mat4 GE::TriangleRenderer::CalculateTransformationMatrixForThisObject() {
	glm::mat4 transformationMat = glm::mat4(1.0f);

	transformationMat = glm::translate(transformationMat, glm::vec3(this->posX, this->posY, this->posZ));
	transformationMat = glm::rotate(transformationMat, glm::radians(this->rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	transformationMat = glm::rotate(transformationMat, glm::radians(this->rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationMat = glm::rotate(transformationMat, glm::radians(this->rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	transformationMat = glm::scale(transformationMat, glm::vec3(this->scaleX, this->scaleY, this->scaleZ));
	return transformationMat;
}

void GE::TriangleRenderer::SetShaderUniforms(glm::mat4* transformationMat, glm::mat4* viewMat, glm::mat4* projectionMat) {
	glUniformMatrix4fv(this->transformuniformId, 1, GL_FALSE, glm::value_ptr(*transformationMat));
	glUniformMatrix4fv(this->viewUniformId, 1, GL_FALSE, glm::value_ptr(*viewMat));
	glUniformMatrix4fv(this->projectionUniformId, 1, GL_FALSE, glm::value_ptr(*projectionMat));
}


void GE::TriangleRenderer::GetReadyToRender()
{
	//select the vertex buffer object into the context
	glBindBuffer(GL_ARRAY_BUFFER, this->vboTriangle);

	//enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexPos3DLocation);

	//Define the structure of a vertex for OpenGL to select values from vertex bufer
	//and store in vertexPos2DLocation attribute
	glVertexAttribPointer(this->vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), nullptr);

	//enable the attribute to be passed colours from the vertex buffer object
	glEnableVertexAttribArray(this->vertexColorLocation);

	//Define the structure of a vertex for OpenGL to selecr values from vertex buffer
	//and store in vColour attribute
	glVertexAttribPointer(this->vertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}

void GE::TriangleRenderer::CleanUpAfterRender()
{
	//Unselect the attribute from the context
	glDisableVertexAttribArray(this->vertexPos3DLocation);

	//unselect vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unselect the program from the context
	glUseProgram(0);
}
void GE::TriangleRenderer::Destroy()
{
	glDeleteProgram(this->programId);

	glDeleteBuffers(1, &this->vboTriangle);
}
