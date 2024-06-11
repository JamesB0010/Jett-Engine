#pragma once
#include <glm/glm.hpp>
#include <exception>
class BadParameter :
	public std::exception
{
	const char* message;
public:
	BadParameter(const char* msg) : message(msg) {}
	const char* what() {
		return message;
	}
};


class Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

public:

	Ray(glm::vec3 origin, glm::vec3 direction);

	glm::vec3 GetPointAlongRay(float t) const;
};

