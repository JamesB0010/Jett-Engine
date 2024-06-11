#include "../Include/Jett Engine/LvlFileReader.h"

LvlFileReader::LvlFileReader(std::string fileToRead) {
	using namespace rapidxml;

	//all the file formatter does is convert forward shashes that would confuse
	//the xmlDocument parser to backslashes
	LvlFileFormatter* fileFormatter = new LvlFileFormatter(fileToRead);

	this->doc = new xml_document<>();

	file = fileFormatter->getFileText();

	delete fileFormatter;

	char* fileAsNullTerminatingCharArray = &file[0];
	this->doc->parse<0>(fileAsNullTerminatingCharArray);
}

LvlFileReader::~LvlFileReader()
{
	delete doc;
}

void LvlFileReader::SpecifyObjectRenderingMode(std::string objectName, renderModes renderMode) {
	auto it = this->objectRenderingModes.find(objectName);

	//check if this object has been found
	if (it == this->objectRenderingModes.end()) {
		this->objectRenderingModes.insert({ objectName, renderMode });
		return;
	}
	this->objectRenderingModes[objectName] = renderMode;
}

void LvlFileReader::AddGameObjectsToEngine(GE::GameEngine* engine)
{
	//the game object folder is a node in the xml scene file
	//all the game objects of the scene are children to the game objects folder
	xml_node<>* gameObjectsFolder = this->doc->first_node()->first_node();
	auto numOfGameObjects = rapidxml::count_children(gameObjectsFolder);

	bool noObjectsInScene = numOfGameObjects <= 0;
	if (noObjectsInScene)
		return;

	for (int i = 0; i < numOfGameObjects; ++i) {
		this->AddGameObjectToEngine(gameObjectsFolder, i, engine);
	}

	GE::Model::ClearCachedModels();
}


void LvlFileReader::AddGameObjectToEngine(xml_node<>* gameObjectsFolder, int distanceFromFirstNode, GE::GameEngine* engine) {
	xml_node<>* currentGameObject = TraverseToNode(gameObjectsFolder->first_node(), distanceFromFirstNode);

	GE::PosRotScale transform = this->GetObjectTransform(currentGameObject);

	std::string modelFilePath = GetDaeFilepath(currentGameObject);


	//get the renderMode
	auto it = this->objectRenderingModes.find(modelFilePath);

	//if a render mode is found then
	if (it != this->objectRenderingModes.end()) {
		//input object into scene appropriate to rendering mode
		if (it->second == renderModes::instanced) {
			this->AddNewInstance(it->first, transform, engine);
			return;
		}
	}

	//by default (no special rendering mode found) just make a visual mesh and add the object to the scene
	GameObject* object = new GameObject(modelFilePath);

	CreateAndAttachVisualMesh(object, modelFilePath, transform, engine);

	engine->AddGameObject(object);
}

void LvlFileReader::AddNewInstance(std::string filename, GE::PosRotScale transform, GE::GameEngine* engine)
{
	//check if this is the first instance of this model
	auto it = this->instanceRenderedGameObjects.find(filename);
	if (it == this->instanceRenderedGameObjects.end()) {
		//first of its kind
		GameObject* object = new GameObject("Instanced mesh " + filename);

		//create an instanced visual mesh component
		InstancedVisualMesh* visMesh = InstancedVisualMesh::HeapInstancedVisMeshIncludedTextureRef("../../Assets/" + filename, engine->camera);
		//add the first transform to the visual mesh
		visMesh->AddInstancePosition(transform.position);

		visMesh->setTransform(transform);
		visMesh->setPos(glm::vec3(0, 0, 0));

		//add component to game object
		object->AddComponent<InstancedVisualMesh>(visMesh);
		//add to engine
		engine->AddGameObject(object);

		this->instanceRenderedGameObjects[filename] = object;
		return;
	}
	//already exists

	GameObject* object = it->second;

	//get the instanced mesh component
	auto visualMesh = object->FindComponent<InstancedVisualMesh>();

	//add this new instance to the instanced mesh
	visualMesh->AddInstancePosition(transform.position);
}



void LvlFileReader::CreateAndAttachVisualMesh(GameObject* object, std::string& modelFilePath, GE::PosRotScale& transform, GE::GameEngine* engine)
{
	VisualMesh* visualMesh = VisualMesh::CreateHeapVisualMeshFromDaeWithIncludedTextureReference("../../Assets/" + modelFilePath, engine->getCamera());
	visualMesh->setTransform(transform);

	object->AddComponent<VisualMesh>(visualMesh);
}

xml_node<>* LvlFileReader::TraverseToNode(xml_node<>* currentNode, int traverseDepth)
{
	if (traverseDepth == 0)
		return currentNode;

	return TraverseToNode(currentNode->next_sibling(), --traverseDepth);
}

GE::PosRotScale LvlFileReader::GetObjectTransform(xml_node<>* currentGameObject)
{
	glm::vec3 objectTranslation = LvlFileReader::getObjectTranslation(currentGameObject);

	glm::vec3 objectRotation = LvlFileReader::getObjectRotation(currentGameObject);

	glm::vec3 objectScale = LvlFileReader::getObjectScale(currentGameObject);

	GE::PosRotScale transform = {
		objectTranslation,
		objectRotation,
		objectScale
	};
	return transform;
}



glm::vec3 LvlFileReader::getObjectTranslation(xml_node<>* node)
{
	xml_attribute<>* translationAttribute = node->first_attribute()->next_attribute()->next_attribute();
	std::string translationAsString = translationAttribute->value();


	std::array<std::string, 3> translationComponents = SeparateVec3String(translationAsString);


	return StringVec3ToFloatVec3(translationComponents) *= 2;
}

glm::vec3 LvlFileReader::getObjectRotation(xml_node<>* node)
{
	xml_attribute<>* rotationAttribute = node->first_attribute()->next_attribute()->next_attribute()->next_attribute();
	std::string rotationAsString = rotationAttribute->value();

	std::array<std::string, 3> rotationComponents = SeparateVec3String(rotationAsString);

	glm::vec3 returnVal = StringVec3ToFloatVec3(rotationComponents);

	returnVal.x = (returnVal.x * (180 / M_PI)) - 90;
	returnVal.y = returnVal.y * (180 / M_PI);
	returnVal.z = returnVal.z * (180 / M_PI);

	return returnVal;
}

glm::vec3 LvlFileReader::getObjectScale(xml_node<>* node)
{
	xml_attribute<>* scaleAttribute = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->next_attribute();
	std::string scaleAsString = scaleAttribute->value();

	std::array<std::string, 3> scaleComponents = SeparateVec3String(scaleAsString);

	return StringVec3ToFloatVec3(scaleComponents) *= 2;
}

glm::vec3 LvlFileReader::StringVec3ToFloatVec3(std::array<std::string, 3> translationComponents)
{
	glm::vec3 translation = glm::vec3(0, 0, 0);

	translation.x = std::stof(translationComponents[0]);
	translation.y = std::stof(translationComponents[1]);
	translation.z = std::stof(translationComponents[2]);

	return translation;
}

std::array<std::string, 3> LvlFileReader::SeparateVec3String(std::string vec3String)
{
	std::istringstream translationStringStream(vec3String);
	
	std::array<std::string, 3> translationComponents;
	for (int i = 0; i < 3; ++i) {
		translationStringStream >> translationComponents[i];
	}

	return translationComponents;
}

std::string LvlFileReader::GetDaeFilepath(xml_node<>* node)
{
	std::string rawName = node->first_node()->first_attribute()->next_attribute()->value();

	std::regex findBackSlashes("\\\\");

	return std::regex_replace(rawName, findBackSlashes, "/");
}
