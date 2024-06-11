#pragma once
#include <glm/glm.hpp>
#include <functional>
class GlobalLerpProcessor;
class LerpPackageProcessor;

template<typename T>
using LerpStep = std::function<void(T)>;

class LerpPackage 
{
public:
	typedef std::function<void(LerpPackage*)> PackageProcessed;
	PackageProcessed finalCallback;
	bool destroyWhenFinished = true;
	float timeToLerp;
	float elapsedTime = 0.0f;
	float current = 0.0f;

	void ResetTiming();
	virtual void AddToProcessor(LerpPackageProcessor* processor) = 0;

	virtual void RunStepCallback() = 0;

	virtual void RunStartCallback() = 0;

	void RunFinalCallback();

	virtual void swapStartAndTarget() = 0;

	virtual LerpPackage* Clone() = 0;
};

