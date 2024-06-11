#pragma once
#include "VisualMesh.h"
#include <vector>
#include <Kerosene Renderer/RenderModes.h>
#include <Kerosene Renderer/InstancedModel.h>
#include <Kerosene Renderer/InstancedModelRenderer.h>
class InstancedVisualMesh :public VisualMesh
{
private:
	std::vector<glm::vec3> instanceOffsets;
	void Draw() override;

	InstancedModelRenderer* modelRenderer = nullptr;

public:
	static InstancedVisualMesh* HeapInstancedVisMeshIncludedTextureRef(std::string modelPath, GE::Camera* cam);

	void AddInstancePosition(glm::vec3 position);

	void setTransform(GE::PosRotScale transform);

	void setPos(glm::vec3 newPos);

	void setRotation(glm::vec3 newRot);

	void setScale(glm::vec3 newScale);
};

