#include "../Include/Jett Engine/LerpPackage.h"
#include "../Include/Jett Engine/GlobalLerpProcessor.h"
#include "../Include/Jett Engine/LerpPackageProcessor.h"

void LerpPackage::RunFinalCallback() {
	this->finalCallback(this);
}

void LerpPackage::ResetTiming() {
	this->elapsedTime = 0.0f;
}