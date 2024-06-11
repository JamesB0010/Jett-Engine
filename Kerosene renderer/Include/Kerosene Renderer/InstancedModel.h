#pragma once
#include "Model.h"
struct InstancedModel
{
	GE::Model* model = nullptr;

	glm::vec3* translationBuffer = nullptr;

	int numberOfInstances;

	InstancedModel(GE::Model* model, glm::vec3* translationBuffer, int numberOfInstances);
};

