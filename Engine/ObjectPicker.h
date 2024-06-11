#pragma once
#include <SDL.h>
#include "InputEventsCallbackBinds.h"
#include <iostream>
#include "Ray.h"
#include "Include/Jett Engine/VisualMesh.h"
#include <iostream>
class ObjectPicker
{
	enum class PickerMode {
		MovingObject,
		ScalingObject,
		RotatingObject,
		Default
	};

	enum class Axis {
		X,
		Y,
		Z,
		None
	};

	Axis currentAxis = Axis::None;

	bool Enabled = true;

	float AmountToMoveInCurrentAxis = 0.0f;

	float AmountToScaleInCurrentAxis = 0.0f;

	float AmountToRotateInCurrentAxis = 0.0f;

	PickerMode mode = PickerMode::Default;
	VisualMesh* SelectedVisualMesh = nullptr;
	GameObject* selectedGameObject = nullptr;

	bool objectSelected = false;

	glm::vec3 movingObjectOriginalPos;
	glm::vec3 scalingObjectOriginalScale;
	glm::vec3 rotatingObjectOriginalRot;

	glm::vec2 windowDimentions;

	Axis MostAlignedXYAxis = Axis::Z;
	int MostAlignedAxisDirection;

	int loopAroundScreenCountX = 0;
	int loopAroundScreenCountY = 0;

public:

	ObjectPicker();

	void OnMouseUp();

	void OnGKeyUp();

	void OnHKeyUp();

	void OnRightMouseUp();

	void OnXKeyUp();

	void OnYKeyUp();

	void OnZKeyUp();

	void OnJUp();

	bool isObjectSelected();

	inline void setEnabled(bool newState) {
		this->Enabled = newState;
	}

	GameObject* GetSelectedObject() {
		return this->selectedGameObject;
	}


	void Update();
	void WrapMouseVertical(int mouseY, int mouseX);
	void WrapMouseHorizontal(int mouseY, int mouseX);
	void WrapMouseInWindow(int mouseY, int mouseX);
	void SetSelectedGameObject(GameObject* gameObject);
};

