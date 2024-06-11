#include "../Include/Jett Engine/GlobalLerpProcessor.h"
GlobalLerpProcessor GlobalLerpProcessor::reference = GlobalLerpProcessor();

GlobalLerpProcessor::GlobalLerpProcessor()
{
	this->lerpProcessor = new LerpPackageProcessor();
}

void GlobalLerpProcessor::AddLerpPackage(LerpPackage* pkg)
{
	try {
		pkg->AddToProcessor(GlobalLerpProcessor::reference.lerpProcessor);
	}
	catch (std::exception nullReference) {
		GlobalLerpProcessor::reference = GlobalLerpProcessor();
		pkg->AddToProcessor(GlobalLerpProcessor::reference.lerpProcessor);
	}
}

void GlobalLerpProcessor::Update()
{
	GlobalLerpProcessor::reference.lerpProcessor->Update();
}
