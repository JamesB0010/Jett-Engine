#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
	this->origin = origin;
	this->direction = direction;
}

glm::vec3 Ray::GetPointAlongRay(float t) const
{
	if (t < 0)
		throw BadParameter("You cannot provide a negative t value");

	return this->origin + t * this->direction;
}
