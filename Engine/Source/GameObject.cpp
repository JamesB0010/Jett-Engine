#include "../Include/Jett Engine/GameObject.h"
#include "../Include/Jett Engine/GameEngine.h"

GameObject::GameObject() {
	this->name = std::string("Un-named game object");
}

GameObject::GameObject(std::string name) {
	this->name = name;
}

void GameObject::Awake()
{
}

void GameObject::Start()
{
	for (int i = 0; i < this->components.size(); ++i) {
		this->components[i]->Start();
	}
}

void GameObject::Update()
{
	for (int i = 0; i < this->components.size(); ++i) {
		this->components[i]->Update();
	}
}

GameObject::~GameObject()
{
	this->components.clear();
}

std::string GameObject::getName() {
	return this->name;
}

bool GameObject::doesObjectHaveDrawableComponents()
{
	DrawableGameComponent* comp = this->FindComponent<DrawableGameComponent>();

	return !(comp == nullptr);
}

void GameObject::RenderComponents() {
	std::vector<DrawableGameComponent*> drawableComponents = this->FindComponents<DrawableGameComponent>();

	for (int i = 0; i < drawableComponents.size(); ++i) {
		((DrawableGameComponent*)drawableComponents[i])->Draw();
	}
}
