#pragma once
#include "GameEngine.h"
#include "LerpPackage.h"
#include <vector>
#include <algorithm>

class LerpPackageProcessor
{
private:
	std::vector<LerpPackage*> packageVector;

	const float moveTowardsTarget = 1.0f;

public:

	void AddPackage(LerpPackage* newPackage);

	void Update();

private:
	void ProcessPackage(LerpPackage* pkg, int i);

	void RemovePackageAtIndexIfCompleted(LerpPackage* pkg, int i);

	void LerpValue(LerpPackage* pkg);

	void UpdateCurrentLerpPercentage(LerpPackage* pkg);
};

