#include "../Include/Jett Engine/InstancedVisualMesh.h"

void InstancedVisualMesh::Draw()
{
	this->modelRenderer->Draw(this->camera, InstancedModel(this->model, this->instanceOffsets.data(), this->instanceOffsets.size()));
}

InstancedVisualMesh* InstancedVisualMesh::HeapInstancedVisMeshIncludedTextureRef(std::string modelPath, GE::Camera* cam)
{
	InstancedVisualMesh* visualMesh = new InstancedVisualMesh();

	visualMesh->camera = cam;

	visualMesh->model = new GE::Model();

	try {
		visualMesh->model->loadFromFileWithIntegratedReferencedTexture(modelPath.c_str());
	}
	catch (GE::ModelImportError err) {
		std::cerr << err.what() << std::endl;
	}

	std::string textureFolderPath;

	for (int i = modelPath.size() - 1; i >= 0; --i) {
		if (modelPath[i] == '/') {
			textureFolderPath = modelPath.substr(0, i + 1);
			break;
		}
	};

	visualMesh->texture = new GE::Texture(textureFolderPath + visualMesh->model->getTextureFilepath());

	visualMesh->modelRenderer = new InstancedModelRenderer();

	visualMesh->modelRenderer->Init();
	visualMesh->modelRenderer->setTexture(visualMesh->texture);
	visualMesh->modelRenderer->setPos(0,0,0);

	return visualMesh;
}

void InstancedVisualMesh::AddInstancePosition(glm::vec3 position)
{
	this->instanceOffsets.push_back(position);
}

void InstancedVisualMesh::setTransform(GE::PosRotScale transform)
{
	this->setPos(transform.position);
	this->setRotation(transform.rotation);
	this->setScale(transform.scale);
}

void InstancedVisualMesh::setPos(glm::vec3 newPos)
{
	this->modelRenderer->setPos(newPos.x, newPos.y, newPos.z);
}

void InstancedVisualMesh::setRotation(glm::vec3 newRot)
{
	this->modelRenderer->setRotation(newRot.x, newRot.y, newRot.z);
}

void InstancedVisualMesh::setScale(glm::vec3 newScale)
{
	this->modelRenderer->setScale(newScale.x, newScale.y, newScale.z);
}
