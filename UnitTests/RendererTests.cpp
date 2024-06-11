#include "pch.h"
#include "CppUnitTest.h"
#include <Kerosene Renderer/Camera.h>
#include <Kerosene Renderer/Texture.h>
#include <glm/glm.hpp>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	
namespace RendererTests
{

	TEST_CLASS(CameraTests)
	{
	public:
		
		GE::CameraViewVectors camViewVectors = {
			glm::vec3(0.0f, 0.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};

		GE::CameraProjectionData camProjectionData = {
			45.0f,
			640.0f / 480.0f,
			0.01f,
			100.0f
		};

		GE::Camera* camera = new GE::Camera(&camViewVectors, &camProjectionData);

		//Setup Tests

		TEST_METHOD(HasCameraCreatedSuccessfully)
		{
			/*
			bool instanceCreated = this->camera != nullptr;

			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(this->camProjectionData.fov), this->camProjectionData.aspectRatio, this->camProjectionData.nearClip, this->camProjectionData.farClip);
			bool projectionMatrixAssigned = this->camera->getProjectionMatrix() == expectedProjMatrix;

			glm::mat4 expectedViewMatrix = glm::lookAt(glm::vec3(this->camera->getPosX(), this->camera->getPosY(), this->camera->getPosZ()), this->camera->getLookTarget(), this->camera->getUpDir());
			bool viewMatrixAssigned = this->camera->getViewMatrix() == expectedViewMatrix;

			bool cameraCreatedSuccessfully = instanceCreated && projectionMatrixAssigned && viewMatrixAssigned;
			Assert::IsTrue(cameraCreatedSuccessfully);*/
		}

		//modification tests

		TEST_METHOD(GetSetPosX) {
			float testValue = 10.0f;
			this->camera->setPosX(testValue);
			Assert::IsTrue(this->camera->getPosX() == testValue);
		}

		TEST_METHOD(GetSetPosY) {
			float testValue = 10.0f;
			this->camera->setPosY(testValue);
			Assert::IsTrue(this->camera->getPosY() == testValue);
		}

		TEST_METHOD(GetSetPosZ) {
			float testValue = 10.0f;
			this->camera->setPosZ(testValue);
			Assert::IsTrue(this->camera->getPosZ() == testValue);
		}

		TEST_METHOD(SetPos) {
			float testX = 2.0f;
			float testY = 4.0f;
			float testZ = 6.0f;

			this->camera->setPos(testX, testY, testZ);
			
			bool xComponentAsExpected = this->camera->getPosX() == testX;
			bool yComponentAsExpected = this->camera->getPosY() == testY;
			bool zComponentAsExpected = this->camera->getPosZ() == testZ;

			Assert::IsTrue(xComponentAsExpected && yComponentAsExpected && zComponentAsExpected);
		}
		TEST_METHOD(SetLookTarget) {
			/*glm::vec3 lookAtVal(1, 3, 6);
			this->camera->setTarget(lookAtVal);
			Assert::IsTrue(this->camera->getLookTarget() == lookAtVal);*/
		}

		TEST_METHOD(SetUpDir) {
			glm::vec3 upTargetVal(3, 4, 7);
			this->camera->setUpDir(upTargetVal);
			Assert::IsTrue(this->camera->getUpDir() == upTargetVal);
		}

		TEST_METHOD(setFov) {
			float newFov = 60.0f;
			this->camera->setFov(newFov);
			
			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(newFov), this->camProjectionData.aspectRatio, this->camProjectionData.nearClip, this->camProjectionData.farClip);
			Assert::IsTrue(this->camera->getProjectionMatrix() == expectedProjMatrix);
		}

		TEST_METHOD(SetAspect) {
			float aspectRatio = 1920.0f / 1080.0f;
			this->camera->setAspectRatio(aspectRatio);

			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(this->camProjectionData.fov), aspectRatio, this->camProjectionData.nearClip, this->camProjectionData.farClip);
			Assert::IsTrue(this->camera->getProjectionMatrix() == expectedProjMatrix);
		}

		TEST_METHOD(SetNearClip) {
			float nearClip = 1;
			this->camera->setNearClip(nearClip);

			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(this->camProjectionData.fov), this->camProjectionData.aspectRatio, nearClip, this->camProjectionData.farClip);
			Assert::IsTrue(this->camera->getProjectionMatrix() == expectedProjMatrix);
		}

		TEST_METHOD(SetFarClip) {
			float farClip = 100;
			this->camera->setFarClip(farClip);

			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(this->camProjectionData.fov), this->camProjectionData.aspectRatio, this->camProjectionData.nearClip, farClip);
			Assert::IsTrue(this->camera->getProjectionMatrix() == expectedProjMatrix);
		}

		TEST_METHOD(viewMatrixUpdates) {
			//Set pos
			float testX = 2.0f;
			float testY = 4.0f;
			float testZ = 6.0f;

			this->camera->setPos(testX, testY, testZ);

			bool xComponentAsExpected = this->camera->getPosX() == testX;
			bool yComponentAsExpected = this->camera->getPosY() == testY;
			bool zComponentAsExpected = this->camera->getPosZ() == testZ;

			////set target
			//glm::vec3 lookAtVal(1, 3, 6);
			//this->camera->setTarget(lookAtVal);

			////set up vector
			//glm::vec3 upTargetVal(3, 4, 7);
			//this->camera->setUpDir(upTargetVal);

			//glm::mat4 expected(glm::lookAt(glm::vec3(this->camera->getPosX(), this->camera->getPosY(), this->camera->getPosZ()), this->camera->getLookTarget(),  this->camera->getUpDir()));
			//Assert::IsTrue(this->camera->getViewMatrix() == expected);
		}


		TEST_METHOD(ProjMatrixUpdatesCorrectly) {
			//set fov 
			float newFov = 60.0f;
			this->camera->setFov(newFov);
			

			//set aspect ratio
			float aspectRatio = 1920.0f / 1080.0f;
			this->camera->setAspectRatio(aspectRatio);

			//set near
			float nearClip = 1;
			this->camera->setNearClip(nearClip);


			//set far
			float farClip = 100;
			this->camera->setFarClip(farClip);

			glm::mat4 expectedProjMatrix = glm::perspective(glm::radians(newFov), aspectRatio, nearClip, farClip);

			Assert::IsTrue(this->camera->getProjectionMatrix() == expectedProjMatrix);
			
		}
	};

	/*TEST_CLASS(TextureFormatMapper) {
	public:
		TEST_METHOD(RGBA32) {
			Uint32  format = GE::TextureFormatMapper::MapSDLFormatToGLFormat(SDL_PIXELFORMAT_RGBA32);
			Assert::IsTrue(format == GL_RGBA);
		};

		TEST_METHOD(RGB24) {
			Uint32 format = GE::TextureFormatMapper::MapSDLFormatToGLFormat(SDL_PIXELFORMAT_RGB24);
			Assert::IsTrue(format == GL_RGB);
		}

		TEST_METHOD(Default) {
			Uint32 format = GE::TextureFormatMapper::MapSDLFormatToGLFormat(SDL_PIXELFORMAT_RGB888);
			Assert::IsTrue(GL_RGB);
		}
	};*/
}
