#include "../Include/Kerosene Renderer/InstancedModel.h"

InstancedModel::InstancedModel(GE::Model* model, glm::vec3* translationBuffer, int numberOfInstances)
{
	this->model = model;

	this->translationBuffer = translationBuffer;

	this->numberOfInstances = numberOfInstances;
}
