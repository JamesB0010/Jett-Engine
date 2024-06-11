#pragma once
#include <string>
#include <rapidxml.hpp>
#include "lvlFileFormatter.h"
#include <sstream>

#include <rapidxml_utils.hpp>
#include <regex>
#include <array>
#include "UsefulStructs.h"
#include "InstancedVisualMesh.h"
#include <Kerosene Renderer/RenderModes.h>

using namespace rapidxml;
class LvlFileReader
{
	xml_document<>* doc = nullptr;

	std::unordered_map < std::string, renderModes> objectRenderingModes;

	std::unordered_map <std::string, GameObject*> instanceRenderedGameObjects;

public:
	LvlFileReader(std::string fileToRead);

	~LvlFileReader();

	void SpecifyObjectRenderingMode(std::string objectName, renderModes renderMode);

	void AddGameObjectsToEngine(GE::GameEngine* engine);

private:
	void AddGameObjectToEngine(xml_node<>* gameObjectsFolder, int distanceFromFirstNode, GE::GameEngine* engine);
	void AddNewInstance(std::string filename, GE::PosRotScale transform, GE::GameEngine* engine);
	void CreateAndAttachVisualMesh(GameObject* object, std::string& modelFilePath, GE::PosRotScale& transform, GE::GameEngine* engine);
	static xml_node<>* TraverseToNode(xml_node<>* currentNode, int traverseDepth);
	GE::PosRotScale GetObjectTransform(xml_node<>* currentGameObject);
	static glm::vec3 getObjectTranslation(xml_node<>* node);
	static glm::vec3 getObjectRotation(xml_node<>* node);
	static glm::vec3 getObjectScale(xml_node<>* node);
	static glm::vec3 StringVec3ToFloatVec3(std::array<std::string, 3>  translationComponents);
	static std::array<std::string, 3> SeparateVec3String(std::string vec3String);

	static std::string GetDaeFilepath(xml_node<>* node);

	std::string file;
};

