#include "../Include/Jett Engine/LerpPackageProcessor.h"

void LerpPackageProcessor::AddPackage(LerpPackage* newPackage)
{
	this->packageVector.push_back(newPackage);
}

void LerpPackageProcessor::Update()
{
	for (int i = 0; i < this->packageVector.size(); i++) {
		this->ProcessPackage(this->packageVector[i], i);
	}
}

void LerpPackageProcessor::ProcessPackage(LerpPackage* pkg, int i)
{
	this->LerpValue(pkg);
	this->RemovePackageAtIndexIfCompleted(pkg, i);
}

void LerpPackageProcessor::RemovePackageAtIndexIfCompleted(LerpPackage* pkg, int i)
{
	if (pkg->current == 1) {
		auto it = this->packageVector.begin() + i;
		this->packageVector.erase(it);

		pkg->RunFinalCallback();
		
		if (pkg->destroyWhenFinished)
			delete pkg;
			
	}
}

void LerpPackageProcessor::LerpValue(LerpPackage* pkg)
{
	float packagePercentage = pkg->current;
	this->UpdateCurrentLerpPercentage(pkg);

	if (packagePercentage == 0) {
		pkg->RunStartCallback();
		pkg->RunStepCallback();
	}
	else
		pkg->RunStepCallback();
}

void LerpPackageProcessor::UpdateCurrentLerpPercentage(LerpPackage* pkg)
{
	pkg->elapsedTime += Time::deltaTime / 1000;

	pkg->current = pkg->elapsedTime / pkg->timeToLerp;

	//clamp to between 0 and 1
	pkg->current = pkg->current < 0 ? 0 : pkg->current;
	pkg->current = pkg->current > 1 ? 1 : pkg->current;
}
