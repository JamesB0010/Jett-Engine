#include "../Include/Jett Engine/VisualMesh.h"
#include "../Include/Jett Engine/GameEngine.h"

void VisualMesh::Draw()
{
	this->modelRenderer->Draw(this->camera, this->model);
}

VisualMesh::VisualMesh() {

}


VisualMesh::VisualMesh(const char* modelPath, const char* texturePath, GE::Camera* cam, glm::vec3 position, ModelRenderer* modelRenderer)
{
	this->camera = cam;

	this->model = new GE::Model();

	try {
		this->model->loadFromFile(modelPath);
	}
	catch (GE::ModelImportError err) {
		std::cout << err.what() << std::endl;
	}

	this->texture = new GE::Texture(texturePath);
	if (modelRenderer == nullptr) {
		this->modelRenderer = new ModelRenderer();
		this->modelRenderer->Init();
	}
	else {
		this->modelRenderer = modelRenderer;
	}

	this->modelRenderer->setTexture(this->texture);
	this->modelRenderer->setPos(position.x, position.y, position.z);
}

VisualMesh* VisualMesh::CreateHeapVisualMeshFromDaeWithIncludedTextureReference(std::string modelPath, GE::Camera* cam, glm::vec3 position)
{
	VisualMesh* visualMesh = new VisualMesh();

	visualMesh->camera = cam;

	visualMesh->model = new GE::Model();

	try {
		visualMesh->model->loadFromFileWithIntegratedReferencedTexture(modelPath.c_str());
	}
	catch (GE::ModelImportError err) {
		std::cout << err.what() << std::endl;
	}

	//std::cout << modelPath << std::endl;

	std::string textureFolderPath;

	for (int i = modelPath.size() - 1; i >= 0; --i) {
		if (modelPath[i] == '/') {
			textureFolderPath = modelPath.substr(0, i + 1);
			break;
		}
	}

	//std::cout << textureFolderPath << std::endl;

	visualMesh->texture = new GE::Texture(textureFolderPath + visualMesh->model->getTextureFilepath());

	visualMesh->modelRenderer = new ModelRenderer();

	visualMesh->modelRenderer->Init();
	visualMesh->modelRenderer->setTexture(visualMesh->texture);
	visualMesh->modelRenderer->setPos(position.x, position.y, position.z);

	return visualMesh;
}

VisualMesh* VisualMesh::CreateHeapVisMeshFromVertexPosVectorAndTexture(std::vector<glm::vec3> vertexPositions, std::string texturePath, GE::Camera* cam, std::string vertShaderSourceCode, std::string fragShaderSourceCode)
{
	VisualMesh* visualMesh = new VisualMesh();

	visualMesh->camera = cam;

	visualMesh->model = new GE::Model();

	visualMesh->model->createFromVertexPosVector(vertexPositions);

	visualMesh->texture = new GE::Texture(texturePath);

	visualMesh->modelRenderer = new ModelRenderer();

	if (fragShaderSourceCode == "") {
		if (vertShaderSourceCode == "") {
			visualMesh->modelRenderer->Init();
		}
		else {
			const GLchar* vertCode[] = { vertShaderSourceCode.c_str() };
			visualMesh->modelRenderer->Init(vertCode);
		}
	}
	else {
		if (vertShaderSourceCode == "") {
			const GLchar* fragCode[] = { fragShaderSourceCode.c_str() };
			visualMesh->modelRenderer->Init(nullptr, fragCode);
		}
		else {
			const GLchar* vertCode[] = { vertShaderSourceCode.c_str() };
			const GLchar* fragCode[] = { fragShaderSourceCode.c_str() };
			visualMesh->modelRenderer->Init(vertCode, fragCode);
		}
	}

	visualMesh->modelRenderer->setTexture(visualMesh->texture);
	visualMesh->modelRenderer->setPos(0, 0, 0);

	return visualMesh;
}

void VisualMesh::setPos(glm::vec3 newPos)
{
	this->modelRenderer->setPos(newPos.x, newPos.y, newPos.z);
}

void VisualMesh::setRotation(glm::vec3 newRot)
{
	this->modelRenderer->setRotation(newRot.x, newRot.y, newRot.z);
}

void VisualMesh::setScale(glm::vec3 newScale) {
	this->modelRenderer->setScale(newScale.x, newScale.y, newScale.z);
}

void VisualMesh::setTransform(GE::PosRotScale transform)
{
	this->setPos(transform.position);
	this->setRotation(transform.rotation);
	this->setScale(transform.scale);
}

glm::vec3 VisualMesh::getRotation()
{
	return glm::vec3(this->modelRenderer->getRotX(), this->modelRenderer->getRotY(), this->modelRenderer->getRotZ());
}

glm::vec3 VisualMesh::getPosition()
{
	return glm::vec3(this->modelRenderer->getPosX(), this->modelRenderer->getPosY(), this->modelRenderer->getPosZ());
}

VisualMesh::~VisualMesh()
{
}
