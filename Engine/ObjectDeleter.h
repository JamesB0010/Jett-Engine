#pragma once
#include "ObjectPicker.h"
#include "InputEventsCallbackBinds.h"
class ObjectDeleter
{
	std::weak_ptr<ObjectPicker> objectPicker;

public:
	ObjectDeleter();

	void OnDelKeyUp();
};

