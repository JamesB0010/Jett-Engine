#include "../Include/Kerosene Renderer/Camera.h"

GE::Camera::~Camera()
{
}

float GE::Camera::getPosX() const
{
	return this->cameraVectors.pos.x;
}

float GE::Camera::getPosY() const
{
	return this->cameraVectors.pos.y;
}

float GE::Camera::getPosZ() const
{
	return this->cameraVectors.pos.z;
}

glm::vec3 GE::Camera::getPos() const
{
	return this->cameraVectors.pos;
}

glm::vec3 GE::Camera::getForwardDir() const
{
	return this->cameraVectors.forwardDir;
}

glm::vec3 GE::Camera::getLookDir() const
{
	return this->lookDir;
}


glm::vec3 GE::Camera::getUpDir() const
{
	return this->cameraVectors.up;
}

glm::mat4 GE::Camera::getViewMatrix() const
{
	return this->viewMatrix;
}

glm::mat4 GE::Camera::getProjectionMatrix() const
{
	return this->projectionMatrix;
}

float GE::Camera::getPitch() const
{
	return this->pitch;
}

float GE::Camera::getYaw() const
{
	return this->yaw;
}

void GE::Camera::setPitch(float newPitch)
{
	this->pitch = newPitch;

	if (this->pitch > 70.0f)
		this->pitch = 70.0f;
	if (this->pitch < -70.0f)
		this->pitch = -70.0f;


	this->UpdateCamMatricies();
}

void GE::Camera::setYaw(float newYaw)
{
	this->yaw = newYaw;

	this->UpdateCamMatricies();
}

void GE::Camera::setPosX(float newX)
{
	this->cameraVectors.pos = glm::vec3(newX, this->cameraVectors.pos.y, this->cameraVectors.pos.z);

	this->UpdateCamMatricies();
}

void GE::Camera::setPosY(float newY)
{
	this->cameraVectors.pos = glm::vec3(this->cameraVectors.pos.x, newY, this->cameraVectors.pos.z);

	this->UpdateCamMatricies();
}

void GE::Camera::setPosZ(float newZ)
{
	this->cameraVectors.pos = glm::vec3(this->cameraVectors.pos.x, this->cameraVectors.pos.y, newZ);

	this->UpdateCamMatricies();
}

void GE::Camera::setPos(float newX, float newY, float newZ)
{
	this->cameraVectors.pos = glm::vec3(newX, newY, newZ);

	this->UpdateCamMatricies();
}

void GE::Camera::setPos(glm::vec3 newPos)
{
	this->cameraVectors.pos = newPos;

	this->UpdateCamMatricies();
}

void GE::Camera::setForwardDir(glm::vec3 newTarget)
{
	this->cameraVectors.forwardDir = newTarget;

	this->UpdateCamMatricies();
}

void GE::Camera::setUpDir(glm::vec3 newUp)
{
	this->cameraVectors.up = newUp;

	this->UpdateCamMatricies();
}

void GE::Camera::setFov(float newFov)
{
	this->cameraProjectionData.fov = newFov;

	this->UpdateCamMatricies();
}

void GE::Camera::setAspectRatio(float newAspectRatio)
{
	this->cameraProjectionData.aspectRatio = newAspectRatio;

	this->UpdateCamMatricies();
}

void GE::Camera::setNearClip(float newNearClip)
{
	this->cameraProjectionData.nearClip = newNearClip;

	this->UpdateCamMatricies();
}

void GE::Camera::setFarClip(float newFarClip)
{
	this->cameraProjectionData.farClip = newFarClip;

	this->UpdateCamMatricies();
}


void GE::Camera::UpdateCamMatricies()
{
	glm::mat4 rollPitchYawMat = glm::mat4(1.0f);

	//calculate rotation matrix
	//note -yaw because rotations in OpenGL are counter clockwise
	rollPitchYawMat = glm::rotate(rollPitchYawMat, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	rollPitchYawMat = glm::rotate(rollPitchYawMat, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

	//Calculate new look at direction based on default dorward direction
	this->lookDir = rollPitchYawMat * glm::vec4(this->cameraVectors.forwardDir, 0.0f);

	this->viewMatrix = glm::lookAt(this->cameraVectors.pos, this->cameraVectors.pos + this->lookDir, this->cameraVectors.up);

	this->projectionMatrix = glm::perspective(
		glm::radians(this->cameraProjectionData.fov), 
		this->cameraProjectionData.aspectRatio, 
		this->cameraProjectionData.nearClip, 
		this->cameraProjectionData.farClip
	);
}
