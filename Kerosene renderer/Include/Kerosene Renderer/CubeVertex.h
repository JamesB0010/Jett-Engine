#pragma once
#include "ShaderUtils.h"
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	//note that cube for skybox has its own vertex struct
	//Major difference is there is no clour or uv
	//this is because cubemao is sampled from cube verticies
class CubeVertex
{
public:
	float x, y, z;

	CubeVertex();

	CubeVertex(float x, float y, float z);
};
}

