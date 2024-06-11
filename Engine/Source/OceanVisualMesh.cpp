#include "../Include/Jett Engine/OceanVisualMesh.h"

OceanVisualMesh::OceanVisualMesh() {

}

void OceanVisualMesh::Draw()
{
	this->modelRenderer->Draw(this->camera,  this->texture);
}

OceanVisualMesh* OceanVisualMesh::heapAllocateNewOceanVisualMesh(GE::Camera* cam)
{
	OceanVisualMesh* visualMesh = new OceanVisualMesh();

	visualMesh->camera = cam;

	visualMesh->ocean = new Ocean();

	visualMesh->modelRenderer = new OceanModelRenderer();

	visualMesh->texture = new GE::Texture("../../Assets/Assets/Water/waterGreen.jpg");

	visualMesh->modelRenderer->Init(visualMesh->ocean);
	visualMesh->modelRenderer->setTexture(visualMesh->texture);
	visualMesh->modelRenderer->setPos(0, 0, 0);

	return visualMesh;
}

void OceanVisualMesh::setTransform(GE::PosRotScale transform)
{
	this->setPos(transform.position);
	this->setRotation(transform.rotation);
	this->setScale(transform.scale);
}


void OceanVisualMesh::setPos(glm::vec3 newPos)
{
	this->modelRenderer->setPos(newPos.x, newPos.y, newPos.z);
}

void OceanVisualMesh::setRotation(glm::vec3 newRot)
{
	this->modelRenderer->setRotation(newRot.x, newRot.y, newRot.z);
}

void OceanVisualMesh::setScale(glm::vec3 scale)
{
	this->modelRenderer->setScale(scale.x, scale.y, scale.z);
}
