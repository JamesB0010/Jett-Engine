#pragma once
#include "GameComponent.h"
#include <iostream>
#include <Kerosene Renderer/ModelRenderer.h>
#include <Kerosene Renderer/Texture.h>
#include "UsefulStructs.h"
#include<Kerosene Renderer/Skydome.h>
#include "GameObject.h"
#include <Kerosene Renderer/ShaderUtils.h>

namespace GE {
	class GameEngine;
}
class SkysphereComponent : public DrawableGameComponent
{
private:
	ModelRenderer* modelRenderer = nullptr;

	GE::Skydome* skydome = nullptr;

	GE::Camera* camera;

	GE::Texture* texture;

	void Draw() override;

	friend class GE::GameEngine;

	friend class GameObject;

public:
	SkysphereComponent(const char* texturePath, GE::Camera* cam);

};

