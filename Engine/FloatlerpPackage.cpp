#include "FloatlerpPackage.h"
#include "./Include/Jett Engine/LerpPackageProcessor.h"
#include "./Include/Jett Engine/GlobalLerpProcessor.h"

float FloatLerpPackage::Lerp()
{
    //lerp calculation from https://graphicscompendium.com/opengl/22-interpolation
    return this->start * (1.0f - this->current) + this->target * this->current;
}

FloatLerpPackage::FloatLerpPackage(float start, float target, LerpStep<float> startCallback, LerpStep<float> stepCallback, PackageProcessed finalCb, float timeToLerp, bool destroyWhenComplete)
{
	this->start = start;
	this->target = target;
	this->timeToLerp = timeToLerp;
	this->startCallback = startCallback;
	this->stepCallback = stepCallback;
	this->finalCallback = finalCb;
	this->destroyWhenFinished = destroyWhenComplete;
}

void FloatLerpPackage::AddToProcessor(LerpPackageProcessor* processor)
{
	processor->AddPackage(this);
}

void FloatLerpPackage::RunStepCallback()
{
	this->stepCallback(this->Lerp());
}

void FloatLerpPackage::RunStartCallback()
{
	this->startCallback(this->Lerp());
}

void FloatLerpPackage::swapStartAndTarget()
{
	std::swap(this->start, this->target);
}

LerpPackage* FloatLerpPackage::Clone()
{
	FloatLerpPackage* clone = new FloatLerpPackage(
		this->start,
		this->target,
		this->startCallback,
		this->stepCallback,
		this->finalCallback,
		this->timeToLerp,
		this->destroyWhenFinished
	);

	clone->timeToLerp = this->timeToLerp;
	clone->elapsedTime = this->elapsedTime;
	clone->current = this->current;

	return clone;
}

void FloatLerpPackage::setStart(float newStart)
{
	this->start = newStart;
}

void FloatLerpPackage::setTarget(float newTarget)
{
	this->target = newTarget;
}
