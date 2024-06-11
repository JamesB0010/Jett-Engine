#include "../Include/Kerosene Renderer/GUIRenderer.h"

GUIRenderer::GUIRenderer(glm::vec2 windowDimentions)
{
	//Load shader source from files. 
	//Text shaders are different to regular shaders because they only
	//transform based on projection
	std::string KeroseneRendererDirectory = "../../../Kerosene renderer/";
	std::string v_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/GUI.vs");
	std::string f_shader_source = GE::loadShaderSourceCode(KeroseneRendererDirectory + "Shaders/GUI.fs");

	//Rest of code is the same as model shader
	//due to the unique way OpenGl handles shader source, OpenGL expects
	//an array of strings. In this case, create an array of the loaded source code strings and pass to coompile Program for compilation
	const GLchar* v_source_Array[] = { v_shader_source.c_str() };
	const GLchar* f_source_array[] = { f_shader_source.c_str() };

	//compile shaders into a program
	if (!GE::compileProgram(v_source_Array, f_source_array, &this->programId))
		std::cerr << "Problem building billboard program. Check console log for more information" << std::endl;

	//now get a link to the vertexPos3D so we can link the attribute
	this->vertexLocation = glGetAttribLocation(this->programId, "vertexPos3D");

	//check for errors
	if (this->vertexLocation == -1)
		std::cerr << "Problem getting vertex3DPos" << std::endl;

	//now link to the vUV attribute
	this->vertexUVLocation = glGetAttribLocation(this->programId, "vUV");

	//check for errors
	if (this->vertexUVLocation == -1)
		std::cerr << "Problem getting vUV" << std::endl;

	//create orthographic camera

	this->projectionMat = glm::ortho(0.0f, (float)windowDimentions.x, (float)windowDimentions.y, 0.0f);
}

void GUIRenderer::AddTextToGUI(GUIText* guiText)
{
	this->guiElements.push_back(new TextUIElement(guiText));
}

void GUIRenderer::AddImageToGUI(GUIImage* guiImage) {
	this->guiElements.push_back(new ImageUIElement(guiImage));
}

void GUIRenderer::ToggleEnabled()
{
	this->drawEnabled = !this->drawEnabled;
}

void GUIRenderer::DrawTexture(glm::vec2 position, GE::Texture* texture)
{
	using namespace GE;
	//get the width and height of the text in pixels
	int textureW = texture->getWidth();
	int textureH = texture->getHeight();

	auto newVertex = Vertex::CreateVertexFromCoordinatesAndUV;

	//A quad is created on the fly for the text in screen co-ordinates, not
	//world co-ordinates
	GLuint vboTextQuad;

	Vertex textureQuad[] = {
		newVertex(glm::vec3(position.x, position.y, 0), glm::vec2(0.0f, 1.0f)),
		newVertex(glm::vec3(position.x, position.y + textureH, 0), glm::vec2(0.0f, 0.0f)),
		newVertex(glm::vec3(position.x + textureW, position.y + textureH, 0), glm::vec2(1.0f, 0.0f)),

		newVertex(glm::vec3(position.x + textureW, position.y + textureH, 0), glm::vec2(1.0f, 0.0f)),
		newVertex(glm::vec3(position.x + textureW, position.y, 0), glm::vec2(1.0f, 1.0f)),
		newVertex(glm::vec3(position.x, position.y, 0), glm::vec2(0.0f, 1.0f))
	};

	//Create the vertex buffer for the quad
	glGenBuffers(1, &vboTextQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);

	//Transfer verticies to graphics memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureQuad), textureQuad, GL_STATIC_DRAW);

	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setup pipeline for object
	//position of the text on the screen. Note that position is in screen co-ords
	//select the program into the rendering context
	glUseProgram(this->programId);

	glUniformMatrix4fv(this->projectionUniformId, 1, GL_FALSE, glm::value_ptr(this->projectionMat));

	//Enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexLocation);
	//select the vertex buffer into the context
	glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);

	//Define the structure of a vertex forOpenGl to select values from vertex buffer
	//and store in vertexPos2DLocation attribute
	glVertexAttribPointer(this->vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, position.x));

	//Enable the attribute to be passed verticies from the vertex buffer object
	glEnableVertexAttribArray(this->vertexUVLocation);
	//define where the vertex specification will find the color data and how much
	glVertexAttribPointer(this->vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, uv.x));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(this->samplerId, 0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureName());

	//Draw the model
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//tidy up pipeline
	glBindTexture(GL_TEXTURE_2D, 0);

	//Unselect the attribute from the context
	glDisableVertexAttribArray(this->vertexLocation);
	glDisableVertexAttribArray(this->vertexUVLocation);

	//Unselect the program from the context
	glUseProgram(0);

	//Release the vertex buffer
	glDeleteBuffers(1, &vboTextQuad);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void GUIRenderer::Draw()
{
	if (this->drawEnabled == false)
		return;

	for (int i = 0; i < this->guiElements.size(); ++i) {
		this->DrawTexture(this->guiElements[i]->getPosition(), this->guiElements[i]->GetTextureFromElement());
	}
}

GUIRenderer::~GUIRenderer()
{
	for (int i = this->guiElements.size(); i > 0; --i) {
		delete this->guiElements[i];
	}
}
