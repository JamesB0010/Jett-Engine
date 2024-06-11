#pragma once
#include "VisualMesh.h"
#include <Kerosene Renderer/Ocean.h>
#include <Kerosene Renderer/OceanModelRenderer.h>
class OceanVisualMesh : public VisualMesh
{
	void Draw() override;

	OceanModelRenderer* modelRenderer = nullptr;

	Ocean* ocean = nullptr;

	OceanVisualMesh();

public:
	static OceanVisualMesh* heapAllocateNewOceanVisualMesh(GE::Camera* cam);

	void setTransform(GE::PosRotScale transform);

	void setPos(glm::vec3 newPos);

	void setRotation(glm::vec3 newRot);

	void setScale(glm::vec3 scale);
};

