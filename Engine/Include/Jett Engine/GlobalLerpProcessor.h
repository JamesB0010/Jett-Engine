#pragma once
#include <iostream>
#include "LerpPackage.h"
#include <vector>
#include "LerpPackageProcessor.h"

class GlobalLerpProcessor
{
private: 
	static GlobalLerpProcessor reference;

	GlobalLerpProcessor();
public:
	static void AddLerpPackage(LerpPackage* pkg);
	static void Update();

private:
	LerpPackageProcessor* lerpProcessor = nullptr;
};

