#include "ObjectPicker.h"
#include "Include/Jett Engine/GameEngine.h"
#include "../Kerosene renderer/MeshBoundingBox.h"

ObjectPicker::ObjectPicker()
{
	MemberFuncButtonListener<ObjectPicker>* leftMouseListener = new MemberFuncButtonListener<ObjectPicker>(this, &ObjectPicker::OnMouseUp);
	GE::GameEngine::AddMouseUpEventListener(SDL_BUTTON_LEFT, leftMouseListener);

	MemberFuncKeyListener<ObjectPicker>* gKeyListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnGKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_G, gKeyListener);

	MemberFuncButtonListener<ObjectPicker>* rightMouseListener = new MemberFuncButtonListener<ObjectPicker>(this, &ObjectPicker::OnRightMouseUp);
	GE::GameEngine::AddMouseUpEventListener(SDL_BUTTON_RIGHT, rightMouseListener);

	MemberFuncKeyListener<ObjectPicker>* XKeyListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnXKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_X, XKeyListener);

	MemberFuncKeyListener<ObjectPicker>* YKeyListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnYKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_Y, YKeyListener);

	MemberFuncKeyListener<ObjectPicker>* ZKeyListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnZKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_Z, ZKeyListener);

	MemberFuncKeyListener<ObjectPicker>* jKeyUpListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnJUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_J, jKeyUpListener);

	MemberFuncKeyListener<ObjectPicker>* hKeyUpListener = new MemberFuncKeyListener<ObjectPicker>(this, &ObjectPicker::OnHKeyUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_H, hKeyUpListener);

	this->windowDimentions = GE::GameEngine::getWindowDimentions();
}

void ObjectPicker::OnMouseUp()
{
	if (!Enabled)
		return;
	if (this->mode != PickerMode::Default) {
		this->mode = PickerMode::Default;
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), this->windowDimentions.x / 2, this->windowDimentions.y / 2);
		GE::GameEngine::SetFpsCamMouseOwnership(true);
		this->currentAxis = Axis::None;
		this->AmountToMoveInCurrentAxis = 0.0f;
	}
	if (this->mode == PickerMode::MovingObject) {
		//put down object
		this->movingObjectOriginalPos = this->SelectedVisualMesh->getPosition();
		return;
	}
	else if (this->mode == PickerMode::ScalingObject) {
		this->scalingObjectOriginalScale = this->SelectedVisualMesh->getScale();
		return;
	}
	else if (this->mode == PickerMode::RotatingObject) {
		this->rotatingObjectOriginalRot = this->SelectedVisualMesh->getRotation();
	}
	using namespace GE;

	glm::vec3 rayOrigin = GameEngine::camera->getPos();

	glm::vec3 rayDirection = GameEngine::camera->getLookDir();

	Ray ray(rayOrigin, rayDirection);

	this->SelectedVisualMesh = nullptr;

	//get a list of drawable objects to enumerate through
	for (GameObject* object : GameEngine::findVisualMeshGameObjects()) {
		VisualMesh* visualMesh = object->FindComponentExact<VisualMesh>();

		if (visualMesh == nullptr)
			continue;

		MeshBoundingBox boundingBox = visualMesh->getBoundingBox();

		
		//calculate Xmin and Xmax
		float TXmin = (boundingBox.minMaxX.x - rayOrigin.x) / rayDirection.x;
		
		float TXmax = (boundingBox.minMaxX.y - rayOrigin.x) / rayDirection.x;

		if (TXmin > TXmax)
			std::swap(TXmin, TXmax);

		//calculate TYmin and TYMax

		float TYmin = (boundingBox.minMaxY.x - rayOrigin.y) / rayDirection.y;

		float TYmax = (boundingBox.minMaxY.y - rayOrigin.y) / rayDirection.y;

		if (TYmin > TYmax)
			std::swap(TYmin, TYmax);
		//calculate Zmin and ZMax

		float TZmin = (boundingBox.minMaxZ.x - rayOrigin.z) / rayDirection.z;

		float TZmax = (boundingBox.minMaxZ.y - rayOrigin.z) / rayDirection.z;

		if (TZmin > TZmax)
			std::swap(TZmin, TZmax);

		float tMin = (TXmin > TYmin) ? TXmin : TYmin; //Get Greatest Min
		float tMax = (TXmax < TYmax) ? TXmax : TYmax; //Get Smallest Max

		bool intersectionDetected = true;

		if (TXmin > TYmax || TYmin > TXmax)
			intersectionDetected = false;

		if (tMin > TZmax || TZmin > tMax)
			intersectionDetected = false;

		if (intersectionDetected) {
			this->SelectedVisualMesh = visualMesh;
			this->selectedGameObject = object;
			this->objectSelected = true;
			this->movingObjectOriginalPos = visualMesh->getPosition();
			this->scalingObjectOriginalScale = visualMesh->getScale();
			this->rotatingObjectOriginalRot = visualMesh->getRotation();
		}
	}
}

void ObjectPicker::OnGKeyUp()
{
	if (!Enabled)
		return;
	if (this->objectSelected && this->mode == PickerMode::Default) {
		if (this->SelectedVisualMesh != nullptr) {
			this->mode = PickerMode::MovingObject;
			GE::GameEngine::SetFpsCamMouseOwnership(false);

			//find if were looking down the Z or X Axis;
			float alignmentWithXAxis;
			float alignmentWithZAxis;

			glm::vec3 ZAxis(0, 0, 1);
			glm::vec3 XAxis(1, 0, 0);

			glm::vec3 cameraDirection = GE::GameEngine::camera->getLookDir();

			alignmentWithXAxis = glm::dot(cameraDirection, XAxis);
			alignmentWithZAxis = glm::dot(cameraDirection, ZAxis);

			this->MostAlignedXYAxis = abs(alignmentWithXAxis) > abs(alignmentWithZAxis) ? Axis::X : Axis::Z;

			if (this->MostAlignedXYAxis == Axis::X) {
				if (alignmentWithXAxis < 0) {
					this->MostAlignedAxisDirection = -1;
				}
				else {
					MostAlignedAxisDirection = 1;
				}
			}
			else {
				if (alignmentWithZAxis < 0) {
					this->MostAlignedAxisDirection = -1;
				}
				else if (alignmentWithZAxis) {
					this->MostAlignedAxisDirection = 1;
				}
			}
		}
		else
			this->objectSelected = false;
	}
}

void ObjectPicker::OnHKeyUp()
{
	if (!Enabled)
		return;
	if (this->objectSelected && this->mode == PickerMode::Default) {
		if (this->SelectedVisualMesh != nullptr) {
			this->mode = PickerMode::RotatingObject;
			GE::GameEngine::SetFpsCamMouseOwnership(false);

			//find if were looking down the Z or X Axis;
			float alignmentWithXAxis;
			float alignmentWithZAxis;

			glm::vec3 ZAxis(0, 0, 1);
			glm::vec3 XAxis(1, 0, 0);

			glm::vec3 cameraDirection = GE::GameEngine::camera->getLookDir();

			alignmentWithXAxis = glm::dot(cameraDirection, XAxis);
			alignmentWithZAxis = glm::dot(cameraDirection, ZAxis);

			this->MostAlignedXYAxis = abs(alignmentWithXAxis) > abs(alignmentWithZAxis) ? Axis::X : Axis::Z;

			if (this->MostAlignedXYAxis == Axis::X) {
				if (alignmentWithXAxis < 0) {
					this->MostAlignedAxisDirection = -1;
				}
				else {
					MostAlignedAxisDirection = 1;
				}
			}
			else {
				if (alignmentWithZAxis < 0) {
					this->MostAlignedAxisDirection = -1;
				}
				else if (alignmentWithZAxis) {
					this->MostAlignedAxisDirection = 1;
				}
			}
		}
		else
			this->objectSelected = false;
	}
}

void ObjectPicker::OnRightMouseUp()
{
	if (!Enabled)
		return;

	if (this->objectSelected && this->mode != PickerMode::Default) {
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), this->windowDimentions.x / 2, this->windowDimentions.y / 2);
		GE::GameEngine::SetFpsCamMouseOwnership(true);
		this->currentAxis = Axis::None;
		this->AmountToMoveInCurrentAxis = 0.0f;
	}
	if (this->objectSelected && this->mode == PickerMode::MovingObject) {
		this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos);
		this->mode = PickerMode::Default;
	}
	else if (this->objectSelected && this->mode == PickerMode::ScalingObject) {
		this->SelectedVisualMesh->setScale(this->scalingObjectOriginalScale);
		this->mode = PickerMode::Default;
	}
	else if (this->objectSelected && this->mode == PickerMode::RotatingObject) {
		this->SelectedVisualMesh->setRotation(this->rotatingObjectOriginalRot);
		this->mode = PickerMode::Default;
	}
}

void ObjectPicker::OnXKeyUp()
{
	if (!Enabled)
		return;
	if (this->mode == PickerMode::MovingObject || this->mode == PickerMode::RotatingObject) {
		this->currentAxis = Axis::X;
	}
}

void ObjectPicker::OnYKeyUp()
{
	if (!Enabled)
		return;
	if (this->mode == PickerMode::MovingObject || this->mode == PickerMode::RotatingObject) {
		this->currentAxis = Axis::Y;
	}
}

void ObjectPicker::OnZKeyUp()
{
	if (!Enabled)
		return;
	if (this->mode == PickerMode::MovingObject || this->mode == PickerMode::RotatingObject) {
		this->currentAxis = Axis::Z;
	}
}

void ObjectPicker::OnJUp()
{
	if (!Enabled)
		return;

	if (this->objectSelected && this->mode == PickerMode::Default) {
		if (this->SelectedVisualMesh != nullptr) {
			this->mode = PickerMode::ScalingObject;
			GE::GameEngine::SetFpsCamMouseOwnership(false);
		}
		else
			this->objectSelected = false;
	}
}

bool ObjectPicker::isObjectSelected()
{
	return this->objectSelected;
}

void ObjectPicker::Update()
{
	if (!Enabled)
		return;

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	if (this->mode == PickerMode::MovingObject && this->currentAxis != Axis::None) {
		switch (this->currentAxis) {
		case Axis::X:
			if (this->MostAlignedXYAxis == Axis::Z) {
				this->AmountToMoveInCurrentAxis = ((this->windowDimentions.x / 2) - mouseX + (this->loopAroundScreenCountX * this->windowDimentions.x / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos + glm::vec3(this->AmountToMoveInCurrentAxis * this->MostAlignedAxisDirection, 0, 0));
			}
			else if (this->MostAlignedXYAxis == Axis::X) {
				this->AmountToMoveInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos + glm::vec3(this->AmountToMoveInCurrentAxis * this->MostAlignedAxisDirection, 0, 0));
			}
			break;
		case Axis::Y:
			this->AmountToMoveInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
			WrapMouseInWindow(mouseY, mouseX);
			this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos + glm::vec3(0, this->AmountToMoveInCurrentAxis, 0));
			break;
		case Axis::Z:
			if (this->MostAlignedXYAxis == Axis::Z) {
				this->AmountToMoveInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos + glm::vec3(0, 0, this->AmountToMoveInCurrentAxis * this->MostAlignedAxisDirection));
			}
			else if (this->MostAlignedXYAxis == Axis::X) {
				this->AmountToMoveInCurrentAxis = ((this->windowDimentions.x / 2) - mouseX + (this->loopAroundScreenCountX * this->windowDimentions.x / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setPos(this->movingObjectOriginalPos + glm::vec3(0, 0, this->AmountToMoveInCurrentAxis * -this->MostAlignedAxisDirection));
			};
			break;
		}
	}
	else if (this->mode == PickerMode::ScalingObject) {
		this->AmountToScaleInCurrentAxis = -((this->windowDimentions.x / 2) - mouseX + (this->loopAroundScreenCountX * this->windowDimentions.x / 2)) / 1000;
		WrapMouseInWindow(mouseY, mouseX);
		this->SelectedVisualMesh->setScale(this->scalingObjectOriginalScale + (glm::vec3(1, 1, 1) * this->AmountToScaleInCurrentAxis));
	}
	else if (this->mode == PickerMode::RotatingObject && this->currentAxis != Axis::None) {
		switch (this->currentAxis) {
		case Axis::X:
			if (this->MostAlignedXYAxis == Axis::Z) {
				this->AmountToRotateInCurrentAxis = ((this->windowDimentions.x / 2) - mouseX + (this->loopAroundScreenCountX * this->windowDimentions.x / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setRotation(this->movingObjectOriginalPos + glm::vec3(this->AmountToRotateInCurrentAxis * this->MostAlignedAxisDirection, 0, 0));
			}
			else if (this->MostAlignedXYAxis == Axis::X) {
				this->AmountToRotateInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setRotation(this->movingObjectOriginalPos + glm::vec3(this->AmountToRotateInCurrentAxis * this->MostAlignedAxisDirection, 0, 0));
			}
			break;
		case Axis::Y:
			this->AmountToRotateInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
			WrapMouseInWindow(mouseY, mouseX);
			this->SelectedVisualMesh->setRotation(this->movingObjectOriginalPos + glm::vec3(0, this->AmountToRotateInCurrentAxis, 0));
			break;
		case Axis::Z:
			if (this->MostAlignedXYAxis == Axis::Z) {
				this->AmountToRotateInCurrentAxis = ((this->windowDimentions.y / 2) - mouseY + (this->loopAroundScreenCountY * this->windowDimentions.y / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setRotation(this->movingObjectOriginalPos + glm::vec3(0, 0, this->AmountToRotateInCurrentAxis * this->MostAlignedAxisDirection));
			}
			else if (this->MostAlignedXYAxis == Axis::X) {
				this->AmountToRotateInCurrentAxis = ((this->windowDimentions.x / 2) - mouseX + (this->loopAroundScreenCountX * this->windowDimentions.x / 2)) / 10;
				WrapMouseInWindow(mouseY, mouseX);
				this->SelectedVisualMesh->setRotation(this->movingObjectOriginalPos + glm::vec3(0, 0, this->AmountToRotateInCurrentAxis * -this->MostAlignedAxisDirection));
			};
			break;
		}
	}
}

void ObjectPicker::WrapMouseVertical(int mouseY, int mouseX)
{
	if (mouseY == 0) {
		//wrap
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), mouseX, this->windowDimentions.y / 2);
		this->loopAroundScreenCountY++;
	}
	if (mouseY >= this->windowDimentions.y - 1) {
		//wrap
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), mouseX, this->windowDimentions.y / 2);
		this->loopAroundScreenCountY--;
	}
}

void ObjectPicker::WrapMouseHorizontal(int mouseY, int mouseX)
{
	if (mouseX == 0) {
		//wrap
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), this->windowDimentions.x / 2, mouseY);
		this->loopAroundScreenCountX++;
	}
	if (mouseX >= this->windowDimentions.x - 1) {
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), this->windowDimentions.x / 2, mouseY);
		this->loopAroundScreenCountX--;
	}
}

void ObjectPicker::WrapMouseInWindow(int mouseY, int mouseX)
{
	WrapMouseHorizontal(mouseY, mouseX);
	WrapMouseVertical(mouseY, mouseX);
}

void ObjectPicker::SetSelectedGameObject(GameObject* gameObject)
{
	glm::vec3 rayOrigin = GE::GameEngine::camera->getPos();

	glm::vec3 rayDirection = GE::GameEngine::camera->getLookDir();

	Ray ray(rayOrigin, rayDirection);

	this->SelectedVisualMesh = nullptr;

	VisualMesh* visualMesh = gameObject->FindComponent<VisualMesh>();
	this->SelectedVisualMesh = visualMesh;
	this->selectedGameObject = gameObject;
	this->objectSelected = true;
	this->movingObjectOriginalPos = visualMesh->getPosition();
	this->scalingObjectOriginalScale = visualMesh->getScale();
	this->rotatingObjectOriginalRot = visualMesh->getRotation();
}
