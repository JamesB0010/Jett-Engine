#include "../Include/Kerosene Renderer/Renderer.h"

void GE::Renderer::DisplayShaderCompilerError(GLuint shaderId)
{
	GLint messageLength = GetLogMessageLengthByShaderId(shaderId);

	if (messageLength > 1)
		OutputShaderCompilationErrorMessage(messageLength, shaderId);
}

GLint GE::Renderer::GetLogMessageLengthByShaderId(GLuint id)
{
	GLint messageLength = 0;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

	return messageLength;
}

void GE::Renderer::OutputShaderCompilationErrorMessage(const GLint& messageLength, const GLuint& shaderId)
{
	GLchar* Msg = GetShaderCompilationErrorMessage(messageLength, shaderId);

	PrintThenReleaseShaderCompilationErrorMessage(Msg);
}


GLchar* GE::Renderer::GetShaderCompilationErrorMessage(const GLint& messageLength, const GLuint& shaderId)
{
	//create a character buffer to store the error message characters
	GLchar* Msg = new GLchar[messageLength + 1];

	//Get the error message characters from the OpenGl internal log
	//into the Msg buffer
	glGetShaderInfoLog(shaderId, messageLength, NULL, Msg);

	return Msg;
}

void GE::Renderer::PrintThenReleaseShaderCompilationErrorMessage(GLchar* Msg)
{
	std::cerr << "Error compiling shader" << Msg << std::endl;

	delete[] Msg;
}
