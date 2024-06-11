#include "ObjectDeleter.h"
#include "Include/Jett Engine/GameEngine.h"

ObjectDeleter::ObjectDeleter() {
	this->objectPicker = GE::GameEngine::getObjectPicker();

	MemberFuncKeyListener<ObjectDeleter>* DelKeyListener = new MemberFuncKeyListener<ObjectDeleter>(this, &ObjectDeleter::OnDelKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_DELETE, DelKeyListener);
}

void ObjectDeleter::OnDelKeyUp()
{
	if (this->objectPicker.lock()->isObjectSelected()) {
		GE::GameEngine::DeleteObject(this->objectPicker.lock()->GetSelectedObject());
	}
}
