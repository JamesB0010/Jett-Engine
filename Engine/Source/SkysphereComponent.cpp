#include "../Include/Jett Engine/SkysphereComponent.h"
#include "../Include/Jett Engine/GameEngine.h"

void SkysphereComponent::Draw()
{
	this->modelRenderer->Draw(this->camera, this->skydome);
}

SkysphereComponent::SkysphereComponent(const char* texturePath, GE::Camera* cam)
{
	this->camera = cam;

	this->skydome = new GE::Skydome();

	this->texture = new GE::Texture(texturePath);

	this->modelRenderer = new ModelRenderer();


	std::string vertShaderString = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/skydome.vs");
	std::string fragShaderString = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/skydome.fs");

	const GLchar* vertShader[] = {vertShaderString.c_str()};
	const GLchar* fragShader[] = {fragShaderString.c_str()};

	this->modelRenderer->Init(vertShader, fragShader);
	this->modelRenderer->setTexture(this->texture);
	this->modelRenderer->setScale(5, 5, 5);
}
