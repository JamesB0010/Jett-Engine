#pragma once
#include <iostream>
#include "InputEventsCallbackBinds.h"
#include <unordered_map>

class ContentDrawer
{
	bool drawerOpen = false;
	bool contentDrawerMoving = false;
	//chached model path refer
	std::unordered_map<int, std::string> indexToCachedModelPath;
public:

	void setDrawerOpen(bool newState) {
		this->drawerOpen = newState;
	}

	void setDrawerMoving(bool newState) {
		this->contentDrawerMoving = newState;
	}
	ContentDrawer();

	void SpaceBarUp();

	void MouseLeftUp();
	void moveDrawerDown();

	void AddIndexAndModelFileName(int index, std::string cachedFileName) {
		this->indexToCachedModelPath[index] = cachedFileName;
	};
};

