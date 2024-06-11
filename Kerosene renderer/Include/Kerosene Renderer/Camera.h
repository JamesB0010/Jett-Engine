#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GE {

	struct CameraViewVectors {
		glm::vec3 pos;
		glm::vec3 forwardDir;
		glm::vec3 up;
	};

	struct CameraProjectionData {
		float fov;
		float aspectRatio;
		float nearClip;
		float farClip;
	};



	class Camera {

		CameraViewVectors cameraVectors;

		glm::vec3 lookDir;

		float pitch, yaw;

		CameraProjectionData cameraProjectionData;

		glm::mat4 viewMatrix;

		glm::mat4 projectionMatrix;
	public:
		Camera(CameraViewVectors* camViewMatricies, CameraProjectionData* camProjectionData)
		: cameraVectors(*camViewMatricies), cameraProjectionData(*camProjectionData)
		{
			this->UpdateCamMatricies();
			this->lookDir = this->cameraVectors.forwardDir;
			pitch = yaw = 0.0f;
		}

		virtual ~Camera();

		//Accessor methods
		float getPosX() const;

		float getPosY() const;

		float getPosZ() const;

		glm::vec3 getPos() const;

		glm::vec3 getForwardDir() const;

		glm::vec3 getLookDir() const;

		glm::vec3 getUpDir() const;

		glm::mat4 getViewMatrix() const;

		glm::mat4 getProjectionMatrix() const;

		float getPitch() const;

		float getYaw() const;

		//Mutator methods

		void setPitch(float newPitch);

		void setYaw(float newYaw);

		void setPosX(float newX);

		void setPosY(float newY);

		void setPosZ(float newZ);

		void setPos(float newX, float newY, float newZ);

		void setPos(glm::vec3 newPos);

		void setForwardDir(glm::vec3 newForwardDir);

		void setUpDir(glm::vec3 newUp);

		void setFov(float newFov);

		void setAspectRatio(float newAspectRatio);

		void setNearClip(float newNearClip);

		void setFarClip(float newFarClip);

	private:
		/// <summary>
		/// This recalculates the camera view and projection based 
		/// on the values stored in the class' member fields
		/// </summary>
		void UpdateCamMatricies();
	};
}
