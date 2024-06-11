#pragma once
#include "./Include/Jett Engine/LerpPackage.h"

class FloatLerpPackage : public LerpPackage
{
	float start;
	float target;

	float Lerp();

public:
	LerpStep<float> stepCallback;
	LerpStep<float> startCallback;


	FloatLerpPackage(float start, float target, LerpStep<float> startCallback, LerpStep<float> stepCallback, PackageProcessed finalCb, float timeToLerp = 1.0f, bool destroyWhenComplete = true);

	void AddToProcessor(LerpPackageProcessor* processor) override;

	void RunStepCallback() override;

	void RunStartCallback() override;

	void swapStartAndTarget() override;

	LerpPackage* Clone() override;

	void setStart(float newStart);

	void setTarget(float newTarget);
};

