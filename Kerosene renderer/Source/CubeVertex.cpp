#include "../Include/Kerosene Renderer/CubeVertex.h"

GE::CubeVertex::CubeVertex()
{
	this->x = this->y = this->z = 0.0f;
}

GE::CubeVertex::CubeVertex(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
