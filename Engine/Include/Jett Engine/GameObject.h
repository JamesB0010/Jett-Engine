#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "GameComponent.h"
#include "VisualMesh.h"
#include "SkysphereComponent.h"

class ComponentNotFound :
	public std::exception
{
	const char* message;
public:
	ComponentNotFound(const char* msg) : message(msg) {}
	const char* what() {
		return message;
	}
};

namespace GE {
	class GameEngine;
}

class GameObject
{
	std::vector<GameComponent*> components;

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0,0,0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	std::string name;

	virtual void Awake();

	virtual void Start();

	virtual void Update();

public:
	~GameObject();
	GameObject();

	GameObject(std::string name);

	void RenderComponents();

	template<typename Component>
	//typename std::enable_if<std::is_same<Component, GameComponent>::value>::type
	void AddComponent(Component* component) {
		this->components.push_back(component);
	}


	template<typename Component>
	Component* FindComponent() {
		Component* returnRef = nullptr;

		for (int i = 0; i < this->components.size(); ++i) {

			bool typeMatches = typeid(*this->components[i]) == typeid(Component);
			bool inheritsFrom = dynamic_cast<Component*>(this->components[i]);
			if (typeMatches || inheritsFrom) {
				returnRef = (Component*)this->components[i];
			}
		}

		return returnRef;
	}

	template<typename Component>
	Component* FindComponentExact() {
		Component* returnRef = nullptr;

		for (int i = 0; i < this->components.size(); ++i) {

			bool typeMatches = typeid(*this->components[i]) == typeid(Component);
			if (typeMatches) {
				returnRef = (Component*)this->components[i];
			}
		}

		return returnRef;
	}

	template<typename Component>
	std::vector<Component*> FindComponents() {
		std::vector<Component*> components;

		for (int i = 0; i < this->components.size(); ++i) {
			bool typeMatches = typeid(*this->components[i]) == typeid(Component);
			bool inheritsFrom = dynamic_cast<Component*>(this->components[i]);
			if (typeMatches || inheritsFrom)
				components.push_back((Component*)this->components[i]);
		}

		return components;
	};

	std::string getName();

	bool doesObjectHaveDrawableComponents();

	friend class GE::GameEngine;
};
