#pragma once
#include "GameComponent.h"
#include <iostream>
#include <Kerosene Renderer/ModelRenderer.h>
#include <Kerosene Renderer/Model.h>
#include <Kerosene Renderer/Texture.h>
#include "UsefulStructs.h"
#include "GameObject.h"
#include "../../../Kerosene renderer/MeshBoundingBox.h"
namespace GE {
	class GameEngine;
}
class VisualMesh : public DrawableGameComponent
{
private:
	ModelRenderer* modelRenderer = nullptr;

protected:
	GE::Model* model = nullptr;

	GE::Camera* camera;

	GE::Texture* texture;
	

	friend class GE::GameEngine;

	friend class GameObject;

public:
	MeshBoundingBox getBoundingBox() {
		return this->model->getMeshBoundingBox() + this->getPosition();
	}

	GLuint getProgramId() {
		return this->modelRenderer->getProgramId();
	}

	void SetModelRendererTexture(GE::Texture* texture) {
		this->modelRenderer->setTexture(texture);
	}

protected:
	VisualMesh();
public:
	VisualMesh(const char* modelPath, const char* texturePath, GE::Camera* cam, glm::vec3 position = glm::vec3(0,0,-40), ModelRenderer* modelRenderer = nullptr);

	void Draw() override;
	static VisualMesh* CreateHeapVisualMeshFromDaeWithIncludedTextureReference(std::string modelPath, GE::Camera* cam, glm::vec3 position = glm::vec3(0,0,-40));

	static VisualMesh* CreateHeapVisMeshFromVertexPosVectorAndTexture(std::vector<glm::vec3> vertexPositions, std::string texturePath, GE::Camera* cam,std::string vertShaderSourceCode = "", std::string fragShaderSourceCode = "");

	void setPos(glm::vec3 newPos);

	void setRotation(glm::vec3 newRot);

	void setScale(glm::vec3 newScale);

	void setTransform(GE::PosRotScale transform);

	glm::vec3 getRotation();

	glm::vec3 getPosition();

	glm::vec3 getScale() {
		return glm::vec3(this->modelRenderer->getScaleX(), this->modelRenderer->getScaleY(), this->modelRenderer->getScaleZ());
	}

	glm::mat4 getTransformMatrix() {
		return this->modelRenderer->CalculateTransformationMatrixForThisObject();
	}

	~VisualMesh();
};

