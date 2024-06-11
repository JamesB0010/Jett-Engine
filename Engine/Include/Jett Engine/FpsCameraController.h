#pragma once
#include <Kerosene Renderer/Camera.h>
#include <SDL.h>
#include "../../InputEventsCallbackBinds.h"
class FpsCameraController
{
	GE::Camera* theCam;
	Uint32 oldTicks;
	bool ownMouse = true;

	bool enabled = true;

public:
	FpsCameraController(GE::Camera* cam);

	void OnMouseUp();

	void OnEscUp();

	void OnTabUp();

	void Update();

	inline void setEnabled(bool newState) {
		this->enabled = newState;
	}

	void setMouseOwnership(bool newOwnership) {
		this->ownMouse = newOwnership;
	}
};

