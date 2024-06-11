#include "../Include/Jett Engine/Vector3LerpPackage.h"
#include "../Include/Jett Engine/LerpPackageProcessor.h"
#include "../Include/Jett Engine/GlobalLerpProcessor.h"

glm::vec3 Vector3LerpPackage::Lerp()
{
	//lerp calculation from https://graphicscompendium.com/opengl/22-interpolation
	return this->start * (1.0f - this->current) + this->target * this->current;
}

Vector3LerpPackage::Vector3LerpPackage(glm::vec3 start, glm::vec3 target, LerpStep<glm::vec3> startCallback, LerpStep<glm::vec3> stepCallback, PackageProcessed finalCb, float timeToLerp, bool destroyWhenComplete)
{
	this->start = start;
	this->target = target;
	this->timeToLerp = timeToLerp;
	this->startCallback = startCallback;
	this->stepCallback = stepCallback;
	this->finalCallback = finalCb;
	this->destroyWhenFinished = destroyWhenComplete;
}

void Vector3LerpPackage::AddToProcessor(LerpPackageProcessor* processor)
{
	processor->AddPackage(this);
}

void Vector3LerpPackage::RunStepCallback()
{
	this->stepCallback(this->Lerp());
}

void Vector3LerpPackage::RunStartCallback()
{
	this->startCallback(this->Lerp());
}

void Vector3LerpPackage::swapStartAndTarget()
{
	std::swap(this->start, this->target);
}

LerpPackage* Vector3LerpPackage::Clone()
{

	Vector3LerpPackage* clone = new Vector3LerpPackage(
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

void Vector3LerpPackage::setStart(glm::vec3 newStart) {
	this->start = newStart;
}

void Vector3LerpPackage::setTarget(glm::vec3 newTarget) {
	this->target = newTarget;
}