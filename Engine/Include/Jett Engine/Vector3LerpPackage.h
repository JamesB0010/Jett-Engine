#pragma once
#include "LerpPackage.h"
class Vector3LerpPackage: public LerpPackage
{
	glm::vec3 start;
	glm::vec3 target;

	glm::vec3 Lerp();

public:
	LerpStep<glm::vec3> stepCallback;
	LerpStep<glm::vec3> startCallback;


	Vector3LerpPackage(glm::vec3 start, glm::vec3 target, LerpStep<glm::vec3> startCallback, LerpStep<glm::vec3> stepCallback, PackageProcessed finalCb, float timeToLerp = 1.0f, bool destroyWhenComplete = true);

	void AddToProcessor(LerpPackageProcessor* processor) override;

	void RunStepCallback() override;

	void RunStartCallback() override;

	void swapStartAndTarget() override;

	LerpPackage* Clone() override;

	void setStart(glm::vec3);

	void setTarget(glm::vec3);
};

